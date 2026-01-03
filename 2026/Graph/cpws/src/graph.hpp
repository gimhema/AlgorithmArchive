#include <bits/stdc++.h>
using namespace std;

void bfs(const vector<vector<int>>& graph, int start) {

    const int MAX = 100000;

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
