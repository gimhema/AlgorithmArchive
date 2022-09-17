#include<string>
#include <iostream>

using namespace std;

// 열려있는 괄호의 갯수가 0일경우 true로
bool answer_ref = false;
int open_count = 0;

void ObserveState()
{
    if(open_count == 0)
    {
        answer_ref = true;
    }else
    {
        answer_ref = false;
    }
}

void CheckStr(string s)
{
    for(int i = 0; i < s.size(); i++)
    {
        if(s[i] == '(')
        {
            open_count++;
        }else
        {
            open_count--;
        }
        ObserveState();
    }
}


bool solution(string s)
{
    bool answer = true;
    
    // [실행] 버튼을 누르면 출력 값을 볼 수 있습니다.
    cout << "Hello Cpp" << endl;
    CheckStr(s);
    if(s[0] == ')')
    {
        answer = false;
    }
    else
    {
        answer = answer_ref;
    }
    return answer;
}
