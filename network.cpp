#include <string>
#include <vector>

using namespace std;

int num_networks = 0;

void bfs(vector<vector<int>> &computers_ref, i, vector<int> &networks)
{
    // 탐색 중 다음 노드로 이동 시 networks[next_idx] = true로 돌림
    
    // 탐색이 종료된 후에는 네트워크의 개수를 1 올림.
    num_networks = num_networks + 1;
}


int solution(int n, vector<vector<int>> computers) {
    int answer = 0;
    vector<vector<int>> computers_cp = computers
    vector<bool> networks(computers_cp.size(), false);

    for(int i = 0; i < networks.size(); i++)
    {
        if(networks[i] != true)
        {
            // 네트워크가 형성이 되지않았다는 가정하에
            networks[i] = true;
            bfs(computers_cp, i, networks);
        }
    }
    
    return answer;
}
