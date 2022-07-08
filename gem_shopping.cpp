#include <string>
#include <vector>

using namespace std;

// 먼저 보석의 종류를 Search하는 함수를 구현해야하고
// 종류를 파악했으면 최단거리를 알 수 있다.
// 각각의 케이스를 비교하면서 최단거리와 비교하고
// 최단거리를 만족하는 케이스가 존재하면 바로 리턴
// 없다면 모든 조건을 만족하는 케이스중 가장 짧은 케이스를 리턴
// 시작 진열대번호가 가장 작은 구간을 리턴하면 되기때문에 . . .

vector<string> gems_type;
long long min_gems_line = 0;
vector<string> gems_wallet;

bool FindGem(string gem_name)
{
    for(int i = 0; i < gems_type.size(); i++)
    {
        if(gems_type[i] == gem_name){ return true; }
    }
    return false;
}

void AddGem(string gem_name)
{
    gems_type.push_back(gem_name);
}

void InitializeGemsType(vector<string> &gems)
{
    for(int i = 0; i < gems.size(); i++)
    {
        if(!FindGem(gems[i]))
        {
            AddGem(gems[i]);
        }
    }
}

vector<int> solution(vector<string> gems) {
    vector<int> answer;
    InitializeGemsType(gems);
    min_gems_line = gems_type.size();
    
    return answer;
}


