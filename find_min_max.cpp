#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

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


string convert_strs2int(vector<string> s)
{
    for(int i = 0; i < s.size(); i++)
    {
     	int j = stoi(s[i]);
        intergers.push_back(j);
        sort(intergers.begin(), intergers.end());
    }
    // string str1 = to_string(num1);
    string min_str = to_string(intergers[0]);
    string max_str = to_string(intergers[intergers.size() - 1]);
    string answer = min_str + " " + max_str;
    
    return answer;
}

string solution(string s) {
    string answer = "";
    vector<string> splits = split(s, ' ');

    answer = convert_strs2int(splits);
    
    return answer;
}
