#include <bits/stdc++.h>
using namespace std;

const int MAX = 100000;

void bfs(const vector<vector<int>>& graph, int start) {

    queue<int> result;
    std::array<bool, MAX> visited = {false};

    visited[start] = true;
    result.push(start);

    while(!result.empty()) {

        int current = result.front();
        result.pop();

        for(int child : graph[current]) {
            if(!visited[child]) {
                visited[child] = true;
                result.push(child);
            }
        }
    }
}

std::array<bool, MAX> dfs_visited = {false};
void dfs(const vector<vector<int>> graph, int start) {
    

    dfs_visited[start] = true;

    for (int child : graph[start]) {
        if(!dfs_visited[child]) {
            dfs(graph, child);
        }
    }

}
