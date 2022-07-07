#include <string>
#include <vector>

// 효율성이 떨어짐...

using namespace std;

int school_way_count = 0;
vector<vector<int>> world;
vector<vector<bool>> visited;
// 재귀사용

bool is_safe(int next_m, int next_n)
{
    if(world[next_m][next_n] == 1)
    { 
        return false;
    }
    return true;
}

bool is_can_arrive(int next_m, int next_n)
{
    if(world[next_m][next_n] == 2)
    {
        school_way_count = school_way_count + 1;
        return true;
    }
    return false;
}

void go_to_school(int m, int n, int current_m, int current_n)
{
    if(is_can_arrive(current_m, current_n)) { return; }
    if(!is_safe(current_m, current_n)) {return;}
    
    if(current_m + 1 < m) // 오른쪽 장외탈출 기저조건
    {
        int next_m = current_m + 1;
        int next_n = current_n;
        if(is_can_arrive(next_m, next_n)){return;}
        if(is_safe(next_m, next_n))
        {
            go_to_school(m, n, next_m, next_n);
        }
    }
    if(current_n + 1 < n) // 아래쪽 장외탈출 기저조건
    {
        int next_m = current_m;
        int next_n = current_n + 1;
        if(is_can_arrive(next_m, next_n)){return;}
        if(is_safe(next_m, next_n)) 
        {
            go_to_school(m, n, next_m, next_n);
        }
    }
    // school_way_count = school_way_count + 1;
    return;
}

void create_world(int m, int n, vector<vector<int>> puddles)
{
    world[m - 1][n - 1] = 2;
    
    // 웅덩이를 1로 설정
    for(int k = 0; k < puddles.size(); k++)
    {
        int puddle_m = puddles[k][0] - 1;
        int puddle_n = puddles[k][1] - 1;
        world[puddle_m][puddle_n] = 1;
        
    }
    
}

int solution(int m, int n, vector<vector<int>> puddles) {

    // puddles : 웅덩이가 표현된 배열
    // 지나갈수있는 길은 0으로 웅덩이는 1로 학교는 2로 표시
    // 월드 초기화
    vector<vector<int>> _world(m, vector<int>(n, 0));
    world = _world;
    vector<vector<bool>> _visited(m, vector<bool>(n, 0));
    visited = _visited;
    create_world(m, n, puddles);
    go_to_school(m, n, 0, 0);
    int answer = school_way_count % 1000000007;
    return answer;
}


