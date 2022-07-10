#include <string>
#include <vector>
#include <unordered_map>


using namespace std;

unordered_map<string, bool> right_sweep_gem_wallet;
unordered_map<string, bool> left_sweep_gem_wallet;
int gem_count_max = 0;
int gem_count_temp = 0;
int gem_count = 0;
int current = 0;
int cursor_l = 0;
int cursor_r = 0;
// vector sweep = {cursor_l, cursor_r}

void UpdateGemCountMax()
{
    // 오른쪽으로 이동할 때 실행되는 함수이다.
    // gem_count_max 값을 갱신한다.
    // HasGemRight 함수 이후에 실행되어야 한다.
    if(gem_count > gem_count_max)
    {
        gem_count_max = gem_count;
        // 오른쪽 커서를 현재 커서로 고정한다.
        cursor_r = current;
    }
}

bool FixLeftCursor()
{
    // 왼쪽으로 이동할 때 실행되는 함수이다.
    if(gem_count_temp == 0)
    {
        // 왼족 커서를 현재 커서로 고정한다.
        cursor_l = current;
        return true;
    }
    return false;
}



bool HasGemRight(string gem)
{
    unordered_map<string, bool>::iterator findIter;
    findIter = right_sweep_gem_wallet.find(gem);
	if (findIter != right_sweep_gem_wallet.end())
	{
		// 현재 위치만 이동, 보석 지갑, gem_count 현상유지
        current++;
        return true;  
	}
	else
	{
		// 갖고있지 않다면
        right_sweep_gem_wallet.insert({gem,true});
        // 현재 커서 이동
        current++;
        // gem_count 증가
        gem_count++; 
        return false;
	}
    return true;
}

bool HasGemLeft(string gem)
{
    unordered_map<string, bool>::iterator findIter;
    findIter = left_sweep_gem_wallet.find(gem);
	if (findIter != right_sweep_gem_wallet.end())
	{
		// 현재 위치만 이동, 보석 지갑, gem_count_temp 현상유지
        current--;
        return true; 
	}
	else
	{
		// 갖고있지 않다면
        left_sweep_gem_wallet.insert({gem,true});
        // 현재 커서 이동(좌측이기때문에 감소)
        current--;
        // gem_count_temp 감소
        gem_count_temp--;
        return false;
	}
    return true;
}

void ChangePhase()
{
    // 오른쪽 커서 고정 후 왼쪽으로 이동하기위해 사용되는 함수
    // 현재 위치를 다시 오른쪽 커서로 고정한다.
    current = cursor_r;
    // gem_count_temp값을 max값으로 설정한다.
    gem_count_temp = gem_count_max;
}

vector<int> solution(vector<string> gems) {
    vector<int> answer;
    
    // 오른쪽으로 이동하면서 오른쪽 커서 고정 후
    for(int i = 0; i < gems.size(); i++)
    {
        if(!HasGemRight(gems[i]))
        {
            UpdateGemCountMax();
        }
    }
    // 페이즈 변경하고
    ChangePhase();
    
    // 왼쪽으로 다시 이동하면서 왼쪽 커서를 설정한다.
    while(true)
    {
        if(!HasGemLeft(gems[current - 1]))
        {
            if(FixLeftCursor())
            {
                answer.push_back(cursor_l + 1);
                answer.push_back(cursor_r);
                break;
            }            
        }
        
    }
    // answer는 양쪽 커서를 +1씩한 값을 리턴한다.
    return answer;
}



