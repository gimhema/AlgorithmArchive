#include <string>
#include <vector>
using namespace std;

// banned_id 갯수만큼 경우의 수 배열(비트마스킹)들이 존재해야한다.
vector<vector<string>> bit_vec = { {}, {}, {}, {}, {}, {}, {}, {} };
vector<string> bool_calc_result;

// bit_vec 첫번째 원소부터 마지막 원소까지, 전부다 bool 연산을 실시하고
// 결과중에 중복을 제거한다음에
// 제거한 뒤의 원소 개수를 리턴한다.

string is_valied(string user_id, string banned_id)
{
    string bit = "";
    int count = 0;
    // 길이 체크
    if(user_id.length() != banned_id.length()){ return "F"; }
    
    // 길이가 맞다면
    for(int i = 0; i < banned_id.length(); i++)
    {
        if(banned_id[i] == "*")
        {
            bit.push_back("1");
            count++;
        }else
        {
            // 문자 미스매치
            if(banned_id[i] != user_id[i]){ return "F"; }
            else 
            {
                bit.push_back("0");
                count++;
            }
        }
    }
    
    if(count == banned_id.length()){ return bit; }
    return "F";
}

void checking(vector<string> &user_id, vector<string> &banned_id)
{
    int bit_vec_idx = 0;
    for(int i = 0; i < banned_id.size(); i++)
    {
        for(int j = 0; j < user_id.size(); j++)
        {
            string result = is_valied(user_id[j], banned_id[i]);
            if(result != "F") 
            { 
                bit_vec[bit_vec_idx].push_back(result);
            }
        }
        bit_vec_idx++;
    }
}

void bit_check(int idx)
{
   /*
0 0 0
0 1 0
0 1 1
1 0 0
1 1 0
1 0 1
1 1 1 
2 0 0
2 1 0
2 0 1
2 1 1
2 1 2
2 2 2
2 2 2
이런 규칙을 가지는 배열을 만들고, 위의 배열에 맞는 . . . bool 연산을 해주면서 . . 중복처리를 해서 결과를 리턴한다.
*/

    return;
}


int solution(vector<string> user_id, vector<string> banned_id) {
    checking(user_id, banned_id);
    bit_check(0);
    int answer = bool_calc_result.size();
    return answer;
}

