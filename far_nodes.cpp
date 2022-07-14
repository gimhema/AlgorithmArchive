#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>

using namespace std;

struct node
{
  int index;
  vector<int> neighbors;
};

unordered_map<int, vector<int>> graph;
vector<bool> visited;
vector<int> levels; // 검색할 때 i + 1로 찾아야함
int current_level = 0;
int _count = 0;
// 1번 노드로부터 BFS 사용

void calc_answer()
{
    int max = levels[0];
    for(int i = 0; i < levels.size(); i++)
    {
        if(levels[i] == max)
        {
            _count++;
        }
        else
        {
            return;
        }
    }
}

void init_graph(int n, vector<vector<int>> &edge)
{
    for(int i = 0; i < n; i++)
    {
        visited.push_back(false);
        levels.push_back(0);
        vector<int> v;
        graph.insert({i + 1, v});
    }
    for(int j = 0; j < edge.size(); j++)
    {
        graph[edge[j][0]].push_back(edge[j][1]);
    }
}

void bfs(int start)
{
    visited[start] = true;
    queue<int> q;
    q.push(start);
    
    while(q.size())
    {
        int current = q.front();
        q.pop();
        for(int i = 0; i < graph[current].size(); i++)
        {
            if(visited[graph[current][i]] == false )
            {
                levels[graph[current][i] + 1] = levels[current + 1] + 1;
                sort(levels.rbegin(), levels.rend());	
                q.push(graph[current][i]);
                visited[graph[current][i]] = true;
            }
        }
    }
    calc_answer();
}



int solution(int n, vector<vector<int>> edge) {
    init_graph(n, edge);
    bfs(1);
    int answer = _count;
    return answer;
}
