#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

// 승자를 기준으로 방향 그래프를 그린다.
// 거쳐가는 노드 개수의 최소값 = 정확하게 순위를 매길 수 있는 선수의 수

int node_count_min = 9999;
int node_count = 0;

// void initialzie_graph(unordered_map<int, vector<int>> &graph, int _size)
// {
//     // _size만큼 그래프를 초기화한다.
// }

void create_graph(unordered_map<int, vector<int>> &graph, vector<vector<int>> &results)
{
    // results를 바탕으로 그래프를 구성한다.
    // um.insert({"melon",3});
    for(int i = 0; i < results.size(); i++)
    {
        graph.insert({results[i][0], graph[results[i][0]].push_back(results[i][1])})
    }
}

void rank_search(unordered_map<int, vector<int>> &graph, int current_pos)
{
    // if graph에 current_pos를 key로하는 map이 존재하는지
        // 존재한다면 node_count ++하고
        // for graph[key].size() 만큼
            // rank_search(graph, graph[current_pos]) 재귀호출
    // else 존재하지않는다면, 즉 끝점이라면
        // node_count와 node_count_min을 비교
        // 만약 node_count가 node_count_min보다 작다면 업데이트
        // 종료
    return;
}


int solution(int n, vector<vector<int>> results) {
    int answer = 0;
    return answer;
}
