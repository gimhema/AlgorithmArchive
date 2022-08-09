#include<vector>
#include<queue>
using namespace std;

int current_x = 0;
int current_y = 0;
vector<int> dx = {0, 1, 0, -1};
vector<int> dy = {-1, 0, 1, 0};

// v(10,vector <int>(10,0));

bool is_can_go(int c_x, int c_y, int dx, int dy, vector<vector<int> > &maps)
{
    // vector<int> dx = {0, 1, 0, -1};
    // vector<int> dy = {-1, 0, 1, 0};
    int next_x = c_x + dx;
    int next_y = c_y + dy;
    
    if(c_x == maps[0].size() && c_y == maps.size()) { return true; }
    
    // 장외 밖 판정
    if(next_x >= maps[0].size() || next_x < 0) { return false; }
    if(next_y >= maps.size() || next_y < 0) { return false; }
    if(maps[next_x][next_y] == 0) { return false; }
    return true;
}

void bfs(int start_x, int start_y, vector<vector<int>> &maps, vector<vector<int>> &cost_maps, vector<vector<bool>> &visited_maps)
{
    // 맵을 채워나가면서 cost를 계산
    // BFS(G, u)
    //     u.visited = 1
    queue<int> q_x;
    queue<int> q_y;
    visited_maps[start_x][start_y] = true;
    //     q.push(u);
    q_x.push(start_x);
    q_y.push(start_y);
    while(q_x.size() && q_y.size())
    {
        //     while(q.size())         
        //         u = q.front()
        int temp_x = q_x.front();
        int temp_y = q_y.front();
        //         q.pop()
        q_x.pop();
        q_y.pop();
        //         for each v ∈ G.Adj[u]
        if(is_can_go(temp_x, temp_y, dx[0], dy[0], maps))
        {
            if(visited_maps[temp_x + dx[0]][temp_y + dy[0]] == false)
            {
            //             if v.visited == false
                visited_maps[temp_x + dx[0]][temp_y + dy[0]] = true;                
            //                 v.visited = u.visited + 1
            //                 q.push(v) 
            q_x.push(temp_x + dx[0]);
            q_y.push(temp_y + dy[0]);
            cost_maps[temp_x + dx[0]][temp_y + dy[0]] = cost_maps[temp_x][temp_y] + 1;
            }
        }
                             
        if(is_can_go(temp_x, temp_y, dx[1], dy[1], maps))
        {
            if(visited_maps[temp_x + dx[1]][temp_y + dy[1]] == false)
            {
            //             if v.visited == false
                visited_maps[temp_x + dx[1]][temp_y + dy[1]] = true;                
            //                 v.visited = u.visited + 1
            //                 q.push(v) 
            q_x.push(temp_x + dx[1]);
            q_y.push(temp_y + dy[1]);
            cost_maps[temp_x + dx[1]][temp_y + dy[1]] = cost_maps[temp_x][temp_y] + 1;
            }
        }
                             
        if(is_can_go(temp_x, temp_y, dx[2], dy[2], maps))
        {
            if(visited_maps[temp_x + dx[2]][temp_y + dy[2]] == false)
            {
            //             if v.visited == false
                visited_maps[temp_x + dx[2]][temp_y + dy[2]] = true;                
            //                 v.visited = u.visited + 1
            //                 q.push(v) 
            q_x.push(temp_x + dx[2]);
            q_y.push(temp_y + dy[2]);
            cost_maps[temp_x + dx[2]][temp_y + dy[2]] = cost_maps[temp_x][temp_y] + 1;
            }
        }
                             
        if(is_can_go(temp_x, temp_y, dx[3], dy[3], maps))
        {
            if(visited_maps[temp_x + dx[3]][temp_y + dy[3]] == false)
            {
            //             if v.visited == false
                visited_maps[temp_x + dx[3]][temp_y + dy[3]] = true;                
            //                 v.visited = u.visited + 1
            //                 q.push(v) 
            q_x.push(temp_x + dx[3]);
            q_y.push(temp_y + dy[3]);
            cost_maps[temp_x + dx[3]][temp_y + dy[3]] = cost_maps[temp_x][temp_y] + 1;
            }
        }

    }
}

int is_can_arrive(int end_x, int end_y, vector<vector<int> > &cost_map)
{
    if(cost_map[end_x][end_y] == 0) { return -1; }
    return cost_map[end_x][end_y] + 1;
}

int solution(vector<vector<int> > maps)
{
    int answer = 0;
    vector<vector<int>> cost_map(maps.size(), vector<int>(maps[0].size(), 0));
    vector<vector<bool>> visited_map(maps.size(), vector<bool>(maps[0].size(), false));
    
    int endpoint_x = maps[0].size() - 1;
    int endpoint_y = maps.size() - 1;
    
    bfs(0, 0, maps, cost_map, visited_map);
    
    answer = is_can_arrive(endpoint_x, endpoint_y, cost_map);
    return answer;
}
