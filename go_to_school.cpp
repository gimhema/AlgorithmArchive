#include <string>
#include <vector>

using namespace std;

// 1000000007
// dfs + 완전탐색
// 최단경로의 개수를 구하라고했지만 사실 오른쪽, 아래만 움직이면 그게 최단경로다
// dfs문제..?
struct Position
{
    int x;
    int y;
};

Position current_position;
Position goal;
vector<vector<int>> _puddles;

bool is_arrive()
{
    if(current_position.x == goal.x && current_position.y == goal.y) { return true; }
    else { return false; }
}


void bfs()
{
    // 기저조건
    // 1. 학교에 도착한 경우
    if(is_arrive())
    { 
        
        return;
    }
    // 2. 범위 밖인 경우
    // 3. 물에 잠긴 경우
    
}

void move_right(int m)
{
    int temp = current_position.x + 1;
    current_position.x = temp;
}

void move_up(int n)
{
    int temp = current_position.y + 1;
    current_position.y = temp;
}

int solution(int m, int n, vector<vector<int>> puddles) {
    int answer = 0;
    _puddles = puddles;
    goal.x = m;
    goal.y = n;
    // answer = min_dist % 1000000007
    return answer;
}
