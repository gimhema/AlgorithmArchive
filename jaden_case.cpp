#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

vector<int> intergers;

vector<string> split(string input, char delimiter) {
    vector<string> answer;
    stringstream ss(input);
    string temp;
 
    while (getline(ss, temp, delimiter)) {
        answer.push_back(temp);
    }
    return answer;
}

string ConvertJadenCase(string target)
{
		
    if(isalpha(target[0]) && islower(target[0]))
    {
        // 앞글자가 알파벳이면 대문자 변환
        target[0] -= 32;
    }
    for(int i = 1; i < target.size(); i++)
    {
        if (isalpha(target[i]) && isupper(target[i])) 
        {
            // 대문자
            target[i] += 32; // 소문자 변환
		}
    }
    return target;
}

vector<string> MakeJadens(vector<string> splits)
{
    vector<string> result;
    for(int i = 0; i < splits.size(); i++)
    {
        result.push_back(ConvertJadenCase(splits[i]));
    }
    return result;
}

string MakeAnswer(vector<string> target)
{
    string answer = "";
    for(int i = 0; i < target.size(); i++)
    {
        answer = answer + target[i] + " ";
    }
    answer.pop_back();
    return answer;
}

string solution(string s) {
    string answer = "";
    vector<string> splits = split(s, ' ');
    answer = MakeAnswer(MakeJadens(splits));
    return answer;
}



