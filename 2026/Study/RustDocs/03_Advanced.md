# 03. 고급 — 동시성, async, 매크로, unsafe/FFI, 설계 패턴

> 목표: 멀티스레드/비동기 서비스, C/C++ 코드와의 연동, 타입 시스템을 활용한 설계를 실무 수준으로 다룬다.

---

## 1. 라이프타임 심화 & Borrow Checker와 싸우지 않는 법

### 자주 마주치는 상황과 해법

**(1) 메서드 안에서 self의 두 필드를 동시에 빌리기**

```rust
struct World { players: Vec<String>, log: Vec<String> }

impl World {
    // ❌ self.log_msg()가 &mut self 전체를 빌려서 충돌
    // fn bad(&mut self) {
    //     for p in &self.players { self.log_msg(p); }
    // }

    // ✅ 필드 단위로 빌리면 컴파일러가 분리해서 인식 (split borrow)
    fn good(&mut self) {
        let log = &mut self.log;
        for p in &self.players {
            log.push(format!("saw {p}"));
        }
    }
}
```

**(2) 벡터의 두 원소를 동시에 가변 참조**

```rust
let mut v = vec![1, 2, 3, 4];
// let a = &mut v[0]; let b = &mut v[1];   // ❌
let (left, right) = v.split_at_mut(2);
std::mem::swap(&mut left[0], &mut right[0]);
v.swap(0, 3);                               // 단순 교환이면 이걸로 충분
```

**(3) 소유권을 잠시 빼오기: `std::mem::take` / `replace` / `Option::take`**

```rust
struct Machine { state: State, buffer: Vec<u8> }
enum State { Idle, Running(String) }

impl Machine {
    fn flush(&mut self) -> Vec<u8> {
        std::mem::take(&mut self.buffer)    // buffer를 빈 Vec로 바꾸고 기존 값을 반환
    }
    fn stop(&mut self) {
        let old = std::mem::replace(&mut self.state, State::Idle);
        if let State::Running(name) = old { println!("stopped {name}"); }
    }
}
```

**(4) 그래프/양방향 링크 → 인덱스 기반 arena**

C++에서는 노드끼리 raw 포인터로 서로를 가리켰지만, Rust에서는 노드를 `Vec`에 넣고 **인덱스**로 연결하는 것이 정석입니다.

```rust
type NodeId = usize;

struct Graph {
    nodes: Vec<Node>,
}
struct Node {
    name: String,
    edges: Vec<NodeId>,
    parent: Option<NodeId>,
}

impl Graph {
    fn add(&mut self, name: &str, parent: Option<NodeId>) -> NodeId {
        let id = self.nodes.len();
        self.nodes.push(Node { name: name.into(), edges: vec![], parent });
        if let Some(p) = parent { self.nodes[p].edges.push(id); }
        id
    }
}
```

> 삭제가 잦다면 `slotmap`, `generational-arena` crate (ABA 문제 방지용 세대 번호 포함).

**(5) 자기 참조 struct** — 안전한 Rust로는 직접 만들 수 없습니다. 인덱스/오프셋 저장으로 바꾸거나, 소유 데이터와 참조를 분리하세요.

### 고급 라이프타임 표기

```rust
// 여러 라이프타임: 반환값은 'a 에만 묶임
fn pick<'a, 'b>(x: &'a str, _y: &'b str) -> &'a str { x }

// T가 'a 보다 오래 살아야 함
struct Wrapper<'a, T: 'a> { r: &'a T }

// HRTB: 모든 라이프타임에 대해 동작하는 클로저
fn call_with<F>(f: F) where F: for<'a> Fn(&'a str) -> &'a str {
    println!("{}", f("hello"));
}
```

---

## 2. ⭐ 동시성 — "Fearless Concurrency"

> **C++ 고민:** "mutex를 잠그지 않고 공유 데이터를 건드리는 실수, 어떻게 막지?"
> → Rust의 `Mutex<T>`는 **데이터를 소유**합니다. 락을 잡지 않으면 데이터에 접근할 방법 자체가 없습니다.

```cpp
// C++: mutex와 데이터가 분리되어 있음 → 실수 가능
std::mutex m;
std::vector<int> data;
void push(int x) {
    std::lock_guard<std::mutex> lk(m);
    data.push_back(x);
}
void oops(int x) { data.push_back(x); }   // 컴파일 됨, data race
```

```rust
// Rust
use std::sync::{Arc, Mutex};
use std::thread;

let data = Arc::new(Mutex::new(Vec::new()));

let mut handles = vec![];
for i in 0..4 {
    let data = Arc::clone(&data);
    handles.push(thread::spawn(move || {
        let mut guard = data.lock().unwrap();   // MutexGuard (lock_guard). 스코프 끝에서 unlock
        guard.push(i);
    }));
}
for h in handles { h.join().unwrap(); }
println!("{:?}", data.lock().unwrap());
```

### Send / Sync — 컴파일 타임 스레드 안전성

| trait | 의미 | 예외 예시 |
|-------|------|-----------|
| `Send` | 다른 스레드로 **소유권 이동** 가능 | `Rc<T>` (비원자 카운트) |
| `Sync` | 여러 스레드에서 `&T`로 **공유** 가능 | `RefCell<T>`, `Cell<T>` |

`thread::spawn`에 `Rc`를 넘기면 **컴파일 에러**가 납니다. → `Arc`로 바꾸라고 알려줍니다. C++에서 `shared_ptr` 안의 객체를 락 없이 여러 스레드가 수정하는 실수가 원천 봉쇄됩니다.

### 동기화 도구 대응표

| C++ | Rust |
|-----|------|
| `std::mutex` + 데이터 | `Mutex<T>` |
| `std::shared_mutex` | `RwLock<T>` |
| `std::atomic<int>` | `AtomicI32`, `AtomicUsize`, `AtomicBool` ... |
| `std::condition_variable` | `Condvar` |
| `std::call_once` / 함수 static | `OnceLock`, `LazyLock` |
| 스레드 안전 큐 (직접 구현) | `std::sync::mpsc`, `crossbeam-channel` |
| `std::async` / thread pool | `rayon`, `tokio::spawn_blocking` |
| `std::jthread` + 지역 변수 참조 | `thread::scope` |

### 채널 (메시지 패싱)

```rust
use std::sync::mpsc;
use std::thread;

let (tx, rx) = mpsc::channel::<String>();
for id in 0..3 {
    let tx = tx.clone();
    thread::spawn(move || {
        tx.send(format!("worker {id} done")).unwrap();
    });
}
drop(tx);                       // 원본 송신자 닫기 → 모든 송신자 drop 시 rx 루프 종료
for msg in rx {
    println!("{msg}");
}
```

### Scoped thread — 지역 변수를 Arc 없이 빌리기

```rust
let mut nums = vec![1, 2, 3, 4, 5, 6];
let (a, b) = nums.split_at_mut(3);
std::thread::scope(|s| {
    s.spawn(|| a.iter_mut().for_each(|x| *x *= 2));
    s.spawn(|| b.iter_mut().for_each(|x| *x *= 10));
});   // 스코프가 끝나면 모든 스레드 join 보장
println!("{nums:?}");   // [2, 4, 6, 40, 50, 60]
```

### Atomic & 전역 상태

```rust
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{LazyLock, Mutex};
use std::collections::HashMap;

static COUNTER: AtomicUsize = AtomicUsize::new(0);
static CACHE: LazyLock<Mutex<HashMap<String, String>>> =
    LazyLock::new(|| Mutex::new(HashMap::new()));

COUNTER.fetch_add(1, Ordering::Relaxed);   // memory_order_relaxed
CACHE.lock().unwrap().insert("k".into(), "v".into());
```

### rayon — 데이터 병렬 처리 (`#pragma omp parallel for` 대체)

```rust
use rayon::prelude::*;
let total: u64 = (0..1_000_000u64).into_par_iter().map(|x| x * x).sum();
let mut v = vec![5, 3, 1, 4];
v.par_sort();
```

`iter()` → `par_iter()`로 바꾸기만 하면 됩니다. data race가 있으면 컴파일이 안 됩니다.

---

## 3. ⭐ async/await (tokio)

> **C++ 고민:** "boost::asio 콜백 지옥 / C++20 코루틴은 라이브러리 지원이 애매"
> → Rust는 언어에 `async/await`가 있고, 런타임은 사실상 표준인 **tokio**를 씁니다.

```toml
[dependencies]
tokio = { version = "1", features = ["full"] }
```

```rust
use tokio::time::{sleep, Duration};

async fn fetch(id: u32) -> String {
    sleep(Duration::from_millis(100)).await;   // 논블로킹 대기
    format!("data {id}")
}

#[tokio::main]
async fn main() {
    // 순차
    let a = fetch(1).await;

    // 동시 실행 (std::when_all 개념)
    let (b, c) = tokio::join!(fetch(2), fetch(3));

    // 백그라운드 태스크 (경량 스레드)
    let handle = tokio::spawn(async { fetch(4).await });
    let d = handle.await.unwrap();

    // 타임아웃 / 먼저 끝나는 것 선택
    tokio::select! {
        r = fetch(5) => println!("got {r}"),
        _ = sleep(Duration::from_millis(50)) => println!("timeout"),
    }
    println!("{a} {b} {c} {d}");
}
```

### 간단한 TCP 에코 서버

```rust
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use tokio::net::TcpListener;

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    let listener = TcpListener::bind("0.0.0.0:8080").await?;
    loop {
        let (mut sock, addr) = listener.accept().await?;
        tokio::spawn(async move {
            let mut buf = [0u8; 1024];
            loop {
                let n = match sock.read(&mut buf).await {
                    Ok(0) | Err(_) => return,
                    Ok(n) => n,
                };
                if sock.write_all(&buf[..n]).await.is_err() { return; }
            }
        });
        println!("client {addr}");
    }
}
```

### async 실무 주의사항
- `async fn`은 호출만 해서는 실행되지 않습니다. `.await` 하거나 `spawn` 해야 합니다 (lazy future).
- async 안에서 **블로킹 호출 금지** (`std::thread::sleep`, 무거운 CPU 연산, 동기 파일 I/O) → `tokio::task::spawn_blocking` 사용.
- `.await`를 넘어서 `std::sync::Mutex` guard를 들고 있지 마세요 → 필요하면 `tokio::sync::Mutex`.
- `tokio::spawn`에 넘기는 future는 `Send + 'static` 이어야 합니다 (`Rc` 사용 불가).
- 채널: `tokio::sync::mpsc`, `oneshot`, `broadcast`, `watch`.

---

## 4. 매크로 — `#define` / variadic template 대체

> **C++ 고민:** "가변 인자 템플릿이나 전처리기 매크로로 하던 코드 생성은?"
> → `macro_rules!` (선언적 매크로)는 **AST 기반, 위생적**이라 `#define`의 함정이 없습니다.

```rust
// 간단한 HashMap 리터럴
macro_rules! hashmap {
    ($($k:expr => $v:expr),* $(,)?) => {{
        let mut m = ::std::collections::HashMap::new();
        $( m.insert($k, $v); )*
        m
    }};
}
let m = hashmap! { "a" => 1, "b" => 2 };

// 반복적인 trait 구현 자동화
trait Zero { fn zero() -> Self; }
macro_rules! impl_zero {
    ($($t:ty),*) => { $( impl Zero for $t { fn zero() -> Self { 0 as $t } } )* };
}
impl_zero!(i32, i64, u32, f64);
```

| 지정자 | 의미 |
|--------|------|
| `$x:expr` | 표현식 |
| `$x:ident` | 식별자 |
| `$x:ty` | 타입 |
| `$x:tt` | 토큰 트리 (아무거나) |
| `$(...),*` | 쉼표로 구분된 0회 이상 반복 |

**절차적 매크로 (proc-macro)**: `#[derive(Serialize)]`, `#[tokio::main]`처럼 코드를 입력받아 코드를 생성하는 Rust 함수입니다. 직접 작성할 일은 드물지만 `syn` + `quote` crate로 작성합니다.

### 조건부 컴파일 (`#ifdef` 대체)

```rust
#[cfg(target_os = "linux")]
fn platform() -> &'static str { "linux" }

#[cfg(windows)]
fn platform() -> &'static str { "windows" }

#[cfg(feature = "metrics")]
mod metrics;

if cfg!(debug_assertions) { println!("debug build"); }
```

```toml
# Cargo.toml
[features]
default = []
metrics = ["dep:prometheus"]

[dependencies]
prometheus = { version = "0.13", optional = true }
```

---

## 5. 고급 trait 기능

### 연관 타입 (associated type) — `typedef` 멤버 / trait 클래스 대체

```cpp
// C++: template<class C> using value_type = typename C::value_type;
```

```rust
trait Container {
    type Item;                         // 구현마다 하나로 결정되는 타입
    fn get(&self, i: usize) -> Option<&Self::Item>;
    fn first(&self) -> Option<&Self::Item> { self.get(0) }
}

struct MyVec(Vec<i32>);
impl Container for MyVec {
    type Item = i32;
    fn get(&self, i: usize) -> Option<&i32> { self.0.get(i) }
}
```

### const generics — 비타입 템플릿 파라미터

```cpp
template <typename T, size_t N> struct Matrix { T data[N][N]; };
```

```rust
#[derive(Debug, Clone, Copy)]
struct Matrix<T, const N: usize> {
    data: [[T; N]; N],
}

impl<T: Copy + Default + std::ops::Add<Output = T> + std::ops::Mul<Output = T>, const N: usize>
    Matrix<T, N>
{
    fn identity_like(one: T) -> Self {
        let mut data = [[T::default(); N]; N];
        for i in 0..N { data[i][i] = one; }
        Self { data }
    }
}

let m: Matrix<f64, 3> = Matrix::identity_like(1.0);
```

### 확장 trait (extension trait) 패턴

```rust
pub trait VecExt<T> {
    fn sorted(self) -> Self;
}
impl<T: Ord> VecExt<T> for Vec<T> {
    fn sorted(mut self) -> Self { self.sort(); self }
}
let v = vec![3, 1, 2].sorted();
```

### blanket impl — "조건을 만족하는 모든 타입"에 구현

```rust
trait Describe { fn describe(&self) -> String; }
impl<T: std::fmt::Debug> Describe for T {
    fn describe(&self) -> String { format!("<{:?}>", self) }
}
```

### CRTP 대체
C++의 CRTP(정적 다형성 + 믹스인)는 Rust에서 **trait 기본 메서드 + 제네릭**으로 자연스럽게 해결됩니다. 별도 패턴이 필요 없습니다.

---

## 6. ⭐ 타입 시스템을 활용한 설계 패턴

### Newtype — 강한 타입 별칭

```cpp
// C++: using UserId = uint64_t;  → OrderId와 섞여도 컴파일 됨
```

```rust
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
struct UserId(u64);
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
struct OrderId(u64);

fn find_order(user: UserId, order: OrderId) {}
// find_order(OrderId(1), UserId(2));  // ❌ 컴파일 에러

// 외부 타입에 외부 trait 구현할 때도 newtype (orphan rule 회피)
struct Csv(Vec<String>);
impl std::fmt::Display for Csv {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "{}", self.0.join(","))
    }
}
```

### Builder 패턴 (기본 인자 / 네임드 인자 대체)

Rust에는 함수 오버로딩, 기본 인자가 없습니다. 대신 builder 또는 `Default` + 구조체 업데이트 문법을 사용합니다.

```rust
#[derive(Debug)]
pub struct Request { url: String, timeout_ms: u64, retries: u32, headers: Vec<(String, String)> }

pub struct RequestBuilder { req: Request }

impl RequestBuilder {
    pub fn new(url: impl Into<String>) -> Self {
        Self { req: Request { url: url.into(), timeout_ms: 3000, retries: 0, headers: vec![] } }
    }
    pub fn timeout(mut self, ms: u64) -> Self { self.req.timeout_ms = ms; self }
    pub fn retries(mut self, n: u32) -> Self { self.req.retries = n; self }
    pub fn header(mut self, k: &str, v: &str) -> Self {
        self.req.headers.push((k.into(), v.into())); self
    }
    pub fn build(self) -> Request { self.req }
}

let req = RequestBuilder::new("https://example.com")
    .timeout(500)
    .header("Accept", "json")
    .build();
```

### Typestate — 잘못된 상태 전이를 컴파일 타임에 차단

```rust
use std::marker::PhantomData;

struct Closed; struct Open;

struct Connection<State> {
    addr: String,
    _s: PhantomData<State>,     // 크기 0, 타입 정보만 보유
}

impl Connection<Closed> {
    fn new(addr: &str) -> Self { Connection { addr: addr.into(), _s: PhantomData } }
    fn open(self) -> Connection<Open> { Connection { addr: self.addr, _s: PhantomData } }
}
impl Connection<Open> {
    fn send(&self, msg: &str) { println!("send {msg} to {}", self.addr); }
    fn close(self) -> Connection<Closed> { Connection { addr: self.addr, _s: PhantomData } }
}

let c = Connection::new("db:5432");
// c.send("x");              // ❌ Closed 상태에는 send가 없음
let c = c.open();
c.send("hello");
let _c = c.close();          // open 상태의 c는 소비됨 → 이후 사용 불가
```

### 오버로딩 대체 정리

| C++ | Rust |
|-----|------|
| 함수 오버로딩 `f(int)`, `f(string)` | 이름을 다르게 (`from_int`, `from_str`) 또는 trait (`impl Into<X>`) |
| 기본 인자 | `Option<T>` 인자, builder, `Default` |
| 생성자 여러 개 | `new`, `with_capacity`, `from_xxx` 연관 함수 |

---

## 7. unsafe

> unsafe는 "규칙을 끄는" 게 아니라 **컴파일러가 검증 못 하는 5가지 행동을 허용**하는 것입니다.
> 1) raw 포인터 역참조 2) unsafe 함수 호출 3) `static mut` 접근 4) unsafe trait 구현 5) union 필드 접근

```rust
let mut x = 10;
let p = &mut x as *mut i32;      // raw 포인터 생성은 safe
unsafe {
    *p += 1;                     // 역참조는 unsafe
}

// 안전한 API로 감싸는 것이 원칙
fn split_first_mut(v: &mut [i32]) -> Option<(&mut i32, &mut [i32])> {
    if v.is_empty() { return None; }
    let ptr = v.as_mut_ptr();
    let len = v.len();
    // SAFETY: 두 영역은 겹치지 않으며 len >= 1 이 보장됨
    unsafe {
        Some((&mut *ptr, std::slice::from_raw_parts_mut(ptr.add(1), len - 1)))
    }
}
```

**실무 규칙**
- unsafe 블록마다 `// SAFETY:` 주석으로 불변식을 문서화
- unsafe는 작은 모듈에 가두고 safe API로 노출
- `cargo miri test`로 UB 검사 (C++의 sanitizer 역할)

---

## 8. ⭐ FFI — C/C++ 코드와 연동

### Rust에서 C 함수 호출

```rust
use std::ffi::{CStr, CString};
use std::os::raw::{c_char, c_int};

extern "C" {
    fn strlen(s: *const c_char) -> usize;
    fn abs(x: c_int) -> c_int;
}

fn main() {
    let s = CString::new("hello").unwrap();      // NUL 종료 문자열 생성
    let n = unsafe { strlen(s.as_ptr()) };
    let a = unsafe { abs(-3) };
    println!("{n} {a}");
}
```

### C/C++에서 Rust 함수 호출

```rust
// lib.rs  (Cargo.toml: [lib] crate-type = ["cdylib"] 또는 ["staticlib"])
#[no_mangle]
pub extern "C" fn rs_add(a: i32, b: i32) -> i32 { a + b }

#[repr(C)]                      // C와 동일한 메모리 레이아웃
pub struct Point { pub x: f64, pub y: f64 }

#[no_mangle]
pub extern "C" fn rs_norm(p: *const Point) -> f64 {
    let p = unsafe { &*p };
    (p.x * p.x + p.y * p.y).sqrt()
}

// Rust 객체를 C에 opaque 핸들로 넘기기
pub struct Engine { count: u64 }

#[no_mangle]
pub extern "C" fn engine_new() -> *mut Engine {
    Box::into_raw(Box::new(Engine { count: 0 }))        // 소유권을 C로 넘김
}
#[no_mangle]
pub extern "C" fn engine_tick(e: *mut Engine) {
    if let Some(e) = unsafe { e.as_mut() } { e.count += 1; }
}
#[no_mangle]
pub extern "C" fn engine_free(e: *mut Engine) {
    if !e.is_null() { unsafe { drop(Box::from_raw(e)); } }  // 소유권 회수 후 해제
}
```

### 도구
| 도구 | 용도 |
|------|------|
| `bindgen` | C/C++ 헤더 → Rust 바인딩 자동 생성 |
| `cbindgen` | Rust → C 헤더 자동 생성 |
| `cxx` | C++ ↔ Rust 안전한 양방향 바인딩 (`std::string`, `unique_ptr`, `vector` 지원) |
| `cc` crate | `build.rs`에서 C/C++ 소스 컴파일 |

```rust
// build.rs — 빌드 전에 실행되는 스크립트 (CMake의 add_library 역할)
fn main() {
    cc::Build::new().cpp(true).file("src/legacy.cpp").compile("legacy");
    println!("cargo:rerun-if-changed=src/legacy.cpp");
}
```

**FFI 주의사항**
- panic이 FFI 경계를 넘어가면 안 됨 → `std::panic::catch_unwind`로 막기
- Rust의 `String`/`Vec`을 C 쪽에서 `free()` 하면 안 됨 (할당자가 다를 수 있음) → 해제 함수도 Rust에서 제공
- C++ 예외도 Rust로 넘어오면 안 됨

---

## 9. 성능 관련 실무 팁

| 주제 | 팁 |
|------|----|
| 할당 줄이기 | `Vec::with_capacity`, `String::with_capacity`, 버퍼 재사용 (`clear()`) |
| 불필요한 clone | 함수 인자를 `&str`, `&[T]`로; `Cow<'a, str>`로 "필요할 때만 복사" |
| 작은 벡터 | `smallvec`, `arrayvec` (SSO/스택 버퍼 개념) |
| 빠른 해시 | 기본 `HashMap`은 DoS 방어용 SipHash → 내부 용도면 `ahash`, `rustc-hash`(FxHashMap) |
| 경계 검사 제거 | iterator 사용 시 대부분 자동 제거. 인덱스 루프보다 iterator 선호 |
| 인라인 | `#[inline]`, `#[inline(always)]` (크레이트 경계를 넘는 작은 함수) |
| 릴리즈 프로필 | `lto = true`, `codegen-units = 1`, `panic = "abort"` |
| 프로파일링 | `perf`, `cargo flamegraph`, `criterion` 벤치마크 |
| 어셈블리 확인 | `cargo asm`, Compiler Explorer (godbolt) |

```toml
[profile.release]
lto = "fat"
codegen-units = 1
panic = "abort"
debug = true        # 프로파일링용 심볼
```

```rust
use std::borrow::Cow;
fn normalize(s: &str) -> Cow<str> {
    if s.contains(' ') { Cow::Owned(s.replace(' ', "_")) }  // 필요할 때만 할당
    else { Cow::Borrowed(s) }
}
```

---

## 10. 프로젝트 구조 & Cargo 실무

### Workspace — 여러 크레이트 관리 (CMake의 add_subdirectory)

```toml
# 루트 Cargo.toml
[workspace]
resolver = "2"
members = ["core", "server", "cli"]

[workspace.dependencies]
serde = { version = "1", features = ["derive"] }
tokio = { version = "1", features = ["full"] }
```

```toml
# server/Cargo.toml
[dependencies]
core = { path = "../core" }
serde.workspace = true
tokio.workspace = true
```

### 라이브러리 + 바이너리 구성

```
my_app/
├── Cargo.toml
├── build.rs            (선택)
├── src/
│   ├── lib.rs          # 핵심 로직 (테스트하기 쉬움)
│   ├── main.rs         # 얇은 진입점: use my_app::run;
│   └── bin/
│       └── tool.rs     # 추가 바이너리: cargo run --bin tool
├── tests/              # 통합 테스트
├── benches/            # criterion 벤치마크
└── examples/           # cargo run --example demo
```

### 자주 쓰는 명령

```bash
cargo add serde --features derive    # 의존성 추가
cargo tree                           # 의존성 트리
cargo update                         # Cargo.lock 갱신
cargo clippy -- -D warnings          # 경고를 에러로 (CI 용)
cargo fmt --check
cargo test --workspace
cargo build --target x86_64-unknown-linux-musl   # 크로스 컴파일 / 정적 바이너리
cargo audit                          # 보안 취약점 검사 (cargo-audit)
cargo expand                         # 매크로 전개 결과 보기 (cargo-expand)
```

### 로깅 — tracing

```rust
use tracing::{info, warn, instrument};

#[instrument]                 // 함수 진입/인자를 span으로 기록
fn process(id: u64) {
    info!(id, "processing");
    warn!("slow path");
}

fn main() {
    tracing_subscriber::fmt().with_env_filter("info").init();
    process(42);
}
```

---

## 11. C++ 개발자가 자주 빠지는 함정 정리

| 증상 | 원인 | 해결 |
|------|------|------|
| `clone()`을 남발하게 됨 | borrow checker를 피하려고 | 설계를 소유권 중심으로 다시 생각. 함수는 참조로 받고, struct는 소유 |
| `Rc<RefCell<T>>` 가 코드 곳곳에 | C++ 객체 그래프를 그대로 옮김 | arena + 인덱스, 메시지 패싱, 소유권 트리 구조로 |
| 상속 계층을 trait으로 그대로 재현 | OOP 습관 | 닫힌 집합이면 enum, 공통 동작이면 trait 기본 메서드, 데이터는 컴포지션 |
| `unwrap()` 투성이 | 에러 처리 귀찮음 | `?` + `anyhow`/`thiserror`. unwrap은 테스트/불변식에만 |
| 문자열 인덱싱 `s[i]` 불가 | UTF-8 | `as_bytes()`, `chars()`, `char_indices()` |
| struct에 참조 필드 → 라이프타임 지옥 | `string_view` 멤버 습관 | 소유 타입 사용, 꼭 필요할 때만 참조 |
| 전역 가변 변수 사용 어려움 | `static mut`은 unsafe | `OnceLock`, `LazyLock`, `Mutex`, `Atomic*` |
| 함수 오버로딩이 안 됨 | 언어 설계 | 다른 이름 / 제네릭 + trait / builder |
| 순환 참조로 메모리 누수 | `Rc` 순환 | 한쪽을 `Weak`로 |
| 컴파일이 느림 | 제네릭 monomorphization, 큰 크레이트 | workspace로 분할, `cargo check` 활용, 증분 빌드 |

---

## 고급 체크리스트

- [ ] split borrow, `mem::take`, arena 인덱스로 borrow checker 문제를 해결할 수 있다
- [ ] `Send`/`Sync`의 의미와 `Arc<Mutex<T>>` 패턴을 이해한다
- [ ] 채널, scoped thread, rayon 중 상황에 맞는 동시성 도구를 고를 수 있다
- [ ] tokio로 비동기 서버를 작성하고 블로킹 호출 주의사항을 안다
- [ ] `macro_rules!`로 반복 코드를 줄이고 `cfg`로 조건부 컴파일을 할 수 있다
- [ ] newtype / builder / typestate 패턴으로 API를 설계할 수 있다
- [ ] `extern "C"`, `#[repr(C)]`, `Box::into_raw/from_raw`로 C/C++과 연동할 수 있다
- [ ] unsafe 코드를 safe API로 감싸고 SAFETY 주석을 작성할 수 있다

← 이전: [02_Intermediate.md](02_Intermediate.md) | 처음: [README.md](README.md)
