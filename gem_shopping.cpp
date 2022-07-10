#include <string>
#include <vector>

using namespace std;

"DIA" (cursor_r = 0)
1
"DIA"  "RUBY" (cursor_r = 1)
2
"DIA"  "RUBY"   "RUBY" (cursor_r = 1)
2
"DIA"  "RUBY"   "RUBY"  "DIA" (cursor_r = 1)
2
"DIA"  "RUBY"   "RUBY"  "DIA"   "DIA" (cursor_r = 1)
2
"DIA"  "RUBY"   "RUBY"  "DIA"   "DIA"  "EMERALD" (cursor_r = 5)
3
"DIA"  "RUBY"   "RUBY"  "DIA"   "DIA"  "EMERALD"  "SAPPHIRE" (cursor_r = 6)
4
"DIA"  "RUBY"   "RUBY"  "DIA"   "DIA"  "EMERALD"  "SAPPHIRE"  "DIA" (cursor_r = 6) >> 여기까지 왔다가.
4
gem_count_temp = gem_count_max;
다시 6번 커서에서 좌측으로 가면서
만약 가지고있지 않은 보석이라면(기존 보석버퍼를 새로지우거나 거꾸로갈때 전용 버퍼를 별도로 만든다)
gem_count_temp를 감소시킨다.
"DIA"  "RUBY"   "RUBY"  "DIA"   "DIA"  "EMERALD"  "SAPPHIRE"
gem_count_temp = 3 (4 - 1)
"DIA"  "RUBY"   "RUBY"  "DIA"   "DIA"  "EMERALD"
gem_count_temp = 2 (3 - 1)
"DIA"  "RUBY"   "RUBY"  "DIA"   "DIA"
gem_count_temp = 1(2 - 1)
"DIA"  "RUBY"   "RUBY"  "DIA" 
gem_count_temp = 1(가지고 있는 보석임, 현상유지)
"DIA"  "RUBY"   "RUBY"
gem_count_temp = 0(1 - 1)
gem_count_temp가 0이되는 지점을 가져온다 이 지점을 cursor_l로 놓는다.
answer = {cursor_l, cursor_r}



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


