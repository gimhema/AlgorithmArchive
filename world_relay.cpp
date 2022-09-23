// 풀어놓고도 모르겠네..

#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<string> dict; // 끝말잇기 단어들이 저장됨


bool DictCheck(string str)
{
    for(int i = 0; i < dict.size(); i++)
    {
        if(dict[i] == str)
        {
            return true;
        }
    }
    return false;
}

int Judge(string prev, string current, int player)
{
    // 게임 재개는 false
    // 게임 끝은 true
    
    // bool dict_check = AddDict(current);
    
    // 끝말잇기 규칙을 준수하지 못했는가? - return player_idx;
    if(prev[prev.size() - 1] != current[0])
    {
        return player;
    }
    else if(DictCheck(current) == true)
    {
        // 이미 중복된 단어를 말했는가? - return player_idx;
        return player;        
    }
    // 모든 규칙이 준수되었는가? - -1;
    return -1;
}

vector<int> PlayGame(vector<int> &players, vector<string> &words)
{
    dict.push_back(words[0]);
    players[0] = players[0] + 1;
    vector<int> answer;
    
    int num_player = players.size();
    int player_idx = 1;
    int result = -1;
    
    for(int i = 1; i < words.size(); i++)
    {
        if(player_idx >= num_player) { player_idx = 0; }
        
        result = Judge(words[i - 1], words[i], player_idx);
        if(result == -1)
        {
            players[player_idx] = players[player_idx] + 1;
            player_idx++;
            dict.push_back(words[i]);
        }
        else
        {
            // result = player_idx;
            break;
        }
    }
    if(result == -1)
    {
        // 모든 플레이어가 규칙을 잘 준수했다면
        answer = {0, 0};
    }else
    {
        // answer = {result + 1, players[result - 1]};
        answer = {result + 1, players[result] + 1};
    }
    return answer;
}

vector<int> solution(int n, vector<string> words) {
    vector<int> answer;
    vector<int> players(n, 0); // 차례 정보가 들어감
    
    // [실행] 버튼을 누르면 출력 값을 볼 수 있습니다. 
    cout << "Hello Cpp" << endl;

    answer = PlayGame(players, words);
    return answer;
}
