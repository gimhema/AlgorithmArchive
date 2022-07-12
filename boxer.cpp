#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

/*
[[4, 3], [4, 2], [3, 2], [1, 2], [2, 5]]
2 - 4, 3, 1
5 - 2
*/

/*
끝점을 찾는다.
아무 점 하나를 잡고 끝점까지 이동시킨다.

탐험을 완료하고 Destination을 찾을때마다, Destination을 맵에서 지우고
그래프가 완전한지 체크한다.

그래프가 완전하다는 것은, 모든 선수, 즉 노드가 관계를 하나씩은 가지고 있다는 의미앋.

*/

unordered_map<int, vector<int>> player_map;
int answer_count = 0;
int destination = 9999;

void init_player_map(vector<int> &results)
{
    for(int i = 0; i < results.size(); i++)
    {
        if(player_map.find(results[i][0])!=player_map.end()){
            // 찾았다면
            player_map[results[i][0]].push_back(results[i][1]);
        }
        else
        {
            // 찾지못했다면
            player_map[results[i][0]] = { results[i][1] };
        }
    }
}

bool is_map_complete()
{
    
}

void exploration(int player_idx)
{
    // 끝점은 반드시 키가 존재하지 않을 것이다.
        if(player_map.find(player_idx)!=player_map.end()){
            // 찾았다면
            vector<int> exp_vec = player_map[results[i][0]];
            for(int i = 0; i < exp_vec.size(); i++)
            {
                exploration(exp_vec[i]);
            }
        }
        else
        {
            destination = player_idx;
            answer_count++;
        }
}


int solution(int n, vector<vector<int>> results) {
    int answer = 0;
    return answer;
}


