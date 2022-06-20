#include <string>
#include <vector>
#include <queue>

using namespace std;

int num_networks = 0;

void bfs(vector<vector<int>> &computers_ref, int current_pos, vector<bool> &networks)
{
    // 탐색 중 다음 노드로 이동 시 networks[next_idx] = true로 돌림
    // BFS(G, u)
    queue<int> q;
    // u.visited = 1
    networks[current_pos] = true;
    // q.push(u);
    q.push(current_pos);
    // while(q.size()) 
    while(q.size())
    {
        int v = q.front();
        q.pop();
        for(int i = 0; i < computers_ref[v].size(); i++)
        {
            if(computers_ref[v][i] == 1 && networks[i] == false)
            {
                networks[i] = true;
                q.push(i);
            }
        }
    }
    //     u = q.front() 
    //     q.pop()
    //     for each v ∈ G.Adj[u]
    //         if v.visited == false
    //             v.visited = u.visited + 1
    //             q.push(v)
    // 탐색이 종료된 후에는 네트워크의 개수를 1 올림.
    num_networks = num_networks + 1;
}


int solution(int n, vector<vector<int>> computers) {
    int answer = 0;
    vector<vector<int>> computers_cp = computers;
    vector<bool> networks(computers_cp.size(), false);

    for(int i = 0; i < networks.size(); i++)
    {
        if(networks[i] != true)
        {
            // 네트워크가 형성이 되지않았다는 가정하에
            bfs(computers_cp, i, networks);
        }
    }
    
    answer = num_networks;
    
    return answer;
}
