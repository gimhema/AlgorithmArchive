#include <string>
#include <vector>

using namespace std;

// 먼저 보석의 종류를 Search하는 함수를 구현해야하고
// 종류를 파악했으면 최단거리를 알 수 있다.
// 각각의 케이스를 비교하면서 최단거리와 비교하고
// 최단거리를 만족하는 케이스가 존재하면 바로 리턴
// 없다면 모든 조건을 만족하는 케이스중 가장 짧은 케이스를 리턴
// 시작 진열대번호가 가장 작은 구간을 리턴하면 되기때문에 . . .
// min_gems_line 개수만큼, 배열들의 연속된 부분들을 wallet에 쓸어담으면서
// 조건을 만족하는지 확인한 후, 만약 조건을 만족한다면 wallet의 길이를 answer로 리턴하고
// 아니라면 배열 인덱스 포인터를 ++하여 위의 과정을 반복한다.
// 만약 배열들을 쓸어담으면서 끝부분에 도달한다면 min_gems_line을 ++하고
// 다시 배열의 첫번째 원소로 돌아가 위의 과정을 반복한다.

vector<string> gems_type;
vector<string> gems_wallet;
long long min_gems_line = 0;
vector<int> answer_cursor;

void AddGem(string gem_name)
{
    gems_type.push_back(gem_name);
}

bool FindGem(string gem_name)
{
    for(int i = 0; i < gems_type.size(); i++)
    {
        if(gems_type[i] == gem_name){ return true; }
    }
    AddGem(gem_name);
    return false;
}

void InitializeGemsType(vector<string> &gems)
{
    for(int i = 0; i < gems.size(); i++)
    {
        FindGem(gems[i]);
    }
}

bool CheckGemCompleteCondition()
{
    // gems_wallet이 조건에 맞는지 확인한다.
    int count = 0;
    for(int i = 0; i < gems_type.size(); i++)
    {
        for(int j = 0; j < gems_wallet.size(); j++)
        {
            if(gems_type[i] == gems_wallet[j])
            { 
                count++;
                break;
            }
        }
    }
    if(count == gems_type.size())
    {
        return true;
    }
    return false;
}

void SweepGemLine(vector<string> &gems, int start, int end, int section)
{
    // 구간의 수가 보석의 개수와 일치하는 경우, 
    // 즉 최악의 경우(조건을 만족하는 케이스가 보석라인 전체를 쓸어담아야하는 경우)
    gems_wallet.clear();

    int _start = start;
    int _end = end;
    int _section = section;
    
    if(section == 1) { _section = 2; }
    
    if(start == end)
    {
        _end = _start + 1;
    }
    
    if(section == gems.size())
    {
        answer_cursor.push_back(0);
        answer_cursor.push_back(gems.size() - 1);
        return;
    }
    
    // end커서가 끝부분을 초과하는 경우, 즉 쓸어담아야할 보석의 개수와 인덱스가 맞지않는 경우    
    if((end - start) + 1 != section) 
    { 
        int next_section = section + 1;
        // 배열 커서를 0으로 돌리고 쓸어담을 구간을 ++한 뒤 재귀적으로 실행
        SweepGemLine(gems, 0, next_section, next_section);
        return; 
    }

        
    for(int i = _start; i == _end; i++)
    {
        gems_wallet.push_back(gems[i]);
    }
    if(CheckGemCompleteCondition())
    { 
        // 정답 조건을 만족하는 경우
        answer_cursor.push_back(_start);
        answer_cursor.push_back(_end);
        return;
    }
    else
    {
        // 만족하지 않는 경우
        int next_start = _start + 1;
        int next_end = _end + 1;
        SweepGemLine(gems, next_start, next_end, section);
        return;
    }
    return;
}

vector<int> solution(vector<string> gems) {
    vector<int> answer;
    InitializeGemsType(gems);
    min_gems_line = gems_type.size();
    
    SweepGemLine(gems, 0, min_gems_line - 1, min_gems_line);
    
    answer = answer_cursor;
    return answer;
}


