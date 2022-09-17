#include <string>
#include <vector>
#include <sstream>

using namespace std;

int zero_cut_count = 0;

int RemoveZero(string target)
{
    // 0을 제거한 문장의 길이를 리턴
    int count = 0;
    for(int i = 0; i < target.size(); i++)
    {
        if(target[i] == '1')
        {
            count++;
        }
    }
    zero_cut_count = zero_cut_count + (target.size() - count);
    return count;
}

string ConvertBinary(int n)
{
    // target을 2진수로 바꾸고 바뀐 2진수를 문자열로 변환
    string r;
    while (n != 0){
        r += ( n % 2 == 0 ? "0" : "1" );
        n /= 2;
    }
    return r;
}

int Translation(string s)
{
    int count = 0;
    string target = s;
    while(true)
    {
        if(target == "1")
        { 
            break;
        }
        int length = RemoveZero(target);
        target = ConvertBinary(length);
        if(target == "1")
        { 
            break;
        }
        else
        {
            count++;
        }
    }
    
    return count;
}

vector<int> solution(string s) {
    vector<int> answer;
    int translation_count = Translation(s);
    answer = {translation_count + 1, zero_cut_count};
    return answer;
}
