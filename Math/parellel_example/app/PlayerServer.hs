-- GenServer 패턴을 하스켈로 구현한 예제
--
-- Elixir GenServer 대응 관계:
--   TQueue PlayerMsg      ↔  프로세스 메일박스
--   TVar PlayerState      ↔  GenServer state
--   call  (MVar 응답)     ↔  GenServer.call  (동기, 응답 대기)
--   cast  (메시지만 전송) ↔  GenServer.cast  (비동기, fire-and-forget)
--   Registry              ↔  Registry / :via 이름 등록
--   Async 스레드          ↔  GenServer 프로세스

module Main where

import Control.Concurrent.Async   (async, mapConcurrently_)
import Control.Concurrent.MVar    (newEmptyMVar, putMVar, takeMVar, MVar)
import Control.Concurrent.STM
import Control.Exception          (SomeException, try)
import Control.Monad              (forM_, forever, when)
import Data.Map.Strict            (Map)
import qualified Data.Map.Strict as Map
import Data.Time.Clock            (getCurrentTime, diffUTCTime)

-- ============================================================
-- 도메인 타입
-- ============================================================

data Position = Position { posX :: Int, posY :: Int }
  deriving (Show, Eq)

data PlayerState = PlayerState
  { playerName :: String
  , playerHp   :: Int
  , playerPos  :: Position
  , playerLog  :: [String]
  } deriving (Show)

initPlayer :: String -> PlayerState
initPlayer name = PlayerState
  { playerName = name
  , playerHp   = 100
  , playerPos  = Position 0 0
  , playerLog  = []
  }

-- ============================================================
-- 메시지 타입 — Elixir GenServer의 handle_call / handle_cast 구분
-- ============================================================

data PlayerMsg
  -- Call: 응답이 필요한 동기 요청 (MVar로 결과 회신)
  = GetStatus  (MVar PlayerState)
  | GetHp      (MVar Int)
  -- Cast: 응답 불필요한 비동기 요청
  | MoveTo     Position
  | TakeDamage Int
  | HealHp     Int
  | ReceiveChat String String   -- from, message
  | Shutdown

-- ============================================================
-- Registry — 플레이어 이름 → 메일박스 매핑
-- Elixir의 Registry / GenServer 이름 등록에 대응
-- ============================================================

type Mailbox  = TQueue PlayerMsg
type Registry = TVar (Map String Mailbox)

newRegistry :: IO Registry
newRegistry = newTVarIO Map.empty

register :: Registry -> String -> Mailbox -> IO ()
register reg name box = atomically $
  modifyTVar reg (Map.insert name box)

unregister :: Registry -> String -> IO ()
unregister reg name = atomically $
  modifyTVar reg (Map.delete name)

lookupPlayer :: Registry -> String -> IO (Maybe Mailbox)
lookupPlayer reg name = Map.lookup name <$> readTVarIO reg

-- ============================================================
-- GenServer 인터페이스 — call / cast
-- ============================================================

-- cast: 비동기 메시지 전송 (응답 없음)
cast :: Mailbox -> PlayerMsg -> IO ()
cast box msg = atomically $ writeTQueue box msg

-- call: 동기 메시지 전송 → 응답 대기
-- MVar를 편지봉투처럼 넣어 보내고, 서버가 채워주면 꺼냄
call :: Mailbox -> (MVar a -> PlayerMsg) -> IO a
call box mkMsg = do
  replyVar <- newEmptyMVar
  atomically $ writeTQueue box (mkMsg replyVar)
  takeMVar replyVar

-- 레지스트리를 통한 call / cast (이름으로 조회)
callByName :: Registry -> String -> (MVar a -> PlayerMsg) -> IO (Maybe a)
callByName reg name mkMsg = do
  mbox <- lookupPlayer reg name
  case mbox of
    Nothing  -> return Nothing
    Just box -> Just <$> call box mkMsg

castByName :: Registry -> String -> PlayerMsg -> IO Bool
castByName reg name msg = do
  mbox <- lookupPlayer reg name
  case mbox of
    Nothing  -> return False
    Just box -> cast box msg >> return True

-- ============================================================
-- GenServer 루프 — handle_call / handle_cast에 해당
-- ============================================================

playerLoop :: TVar PlayerState -> Mailbox -> IO ()
playerLoop stateVar box = loop
  where
    loop = do
      msg <- atomically $ readTQueue box
      cont <- handleMsg msg
      when cont loop

    handleMsg :: PlayerMsg -> IO Bool

    -- handle_call: GetStatus
    handleMsg (GetStatus replyVar) = do
      st <- readTVarIO stateVar
      putMVar replyVar st
      return True

    -- handle_call: GetHp
    handleMsg (GetHp replyVar) = do
      st <- readTVarIO stateVar
      putMVar replyVar (playerHp st)
      return True

    -- handle_cast: MoveTo
    handleMsg (MoveTo pos) = do
      atomically $ modifyTVar stateVar $ \st -> st
        { playerPos = pos
        , playerLog = playerLog st ++ ["이동 → (" ++ show (posX pos) ++ "," ++ show (posY pos) ++ ")"]
        }
      return True

    -- handle_cast: TakeDamage
    handleMsg (TakeDamage dmg) = do
      atomically $ modifyTVar stateVar $ \st ->
        let newHp = max 0 (playerHp st - dmg)
        in st { playerHp   = newHp
              , playerLog  = playerLog st ++ ["피해 " ++ show dmg ++ " → HP " ++ show newHp]
              }
      return True

    -- handle_cast: HealHp
    handleMsg (HealHp amount) = do
      atomically $ modifyTVar stateVar $ \st ->
        let newHp = min 100 (playerHp st + amount)
        in st { playerHp  = newHp
              , playerLog = playerLog st ++ ["회복 " ++ show amount ++ " → HP " ++ show newHp]
              }
      return True

    -- handle_cast: ReceiveChat
    handleMsg (ReceiveChat from msg) = do
      atomically $ modifyTVar stateVar $ \st -> st
        { playerLog = playerLog st ++ ["[" ++ from ++ "]: " ++ msg]
        }
      return True

    -- handle_cast: Shutdown → 루프 종료
    handleMsg Shutdown = do
      st <- readTVarIO stateVar
      putStrLn $ "[" ++ playerName st ++ "] 서버 종료"
      return False

-- ============================================================
-- 플레이어 서버 시작 — Elixir의 GenServer.start_link에 대응
-- ============================================================

startPlayer :: Registry -> String -> IO Mailbox
startPlayer reg name = do
  stateVar <- newTVarIO (initPlayer name)
  box      <- newTQueueIO
  register reg name box
  _ <- async $ do
    playerLoop stateVar box
    unregister reg name
  return box

-- ============================================================
-- 시나리오: 여러 플레이어가 동시에 서버에 접속해 행동
-- ============================================================

-- 각 플레이어의 행동 시뮬레이션 (독립적 병렬 실행)
simulatePlayer :: Registry -> String -> [(Int, Int)] -> [Int] -> IO ()
simulatePlayer reg name moves damages = do
  box <- startPlayer reg name

  -- 이동 시퀀스 (cast, 비동기)
  forM_ moves $ \(x, y) ->
    cast box (MoveTo (Position x y))

  -- 피해 시퀀스 (cast, 비동기)
  forM_ damages $ \dmg ->
    cast box (TakeDamage dmg)

  -- 상태 조회 (call, 동기 — 모든 cast 처리 이후 순서 보장)
  finalSt <- call box GetStatus
  putStrLn $ "\n=== " ++ name ++ " 최종 상태 ==="
  putStrLn $ "HP: " ++ show (playerHp finalSt)
  putStrLn $ "위치: (" ++ show (posX (playerPos finalSt)) ++ ", " ++ show (posY (playerPos finalSt)) ++ ")"
  putStrLn $ "로그:"
  mapM_ (\l -> putStrLn $ "  " ++ l) (playerLog finalSt)

  cast box Shutdown

-- 파티 브로드캐스트 — 등록된 모든 플레이어에게 메시지
broadcast :: Registry -> String -> String -> IO ()
broadcast reg from msg = do
  players <- Map.toList <$> readTVarIO reg
  forM_ players $ \(_, box) ->
    cast box (ReceiveChat from msg)

-- ============================================================
-- main
-- ============================================================

main :: IO ()
main = do
  putStrLn "=== GenServer 스타일 플레이어 서버 ==="

  reg <- newRegistry

  start <- getCurrentTime

  -- 3명의 플레이어를 병렬로 시뮬레이션
  mapConcurrently_ id
    [ simulatePlayer reg "Aria"
        [(1,0),(2,0),(3,0)]  -- 이동
        [15, 20]              -- 피해

    , simulatePlayer reg "Bard"
        [(0,1),(0,2)]
        [30]

    , simulatePlayer reg "Cain"
        [(1,1),(2,2),(3,3)]
        [10, 5, 25]
    ]

  end <- getCurrentTime
  putStrLn $ "\n전체 처리 시간: " ++ show (diffUTCTime end start)
  putStrLn "모든 플레이어 처리 완료"
