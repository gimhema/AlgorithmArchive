#include <string>
#include <vector>
using namespace std;

// banned_id 갯수만큼 경우의 수 배열(비트마스킹)들이 존재해야한다.
vector<vector<string>> bit_vec;
vector<string> bool_calc_result;

// bit_vec 첫번째 원소부터 마지막 원소까지, 전부다 bool 연산을 실시하고
// 결과중에 중복을 제거한다음에
// 제거한 뒤의 원소 개수를 리턴한다.

bool is_valied(string user_id, string banned_id)
{
    vecotr<int> point_idx;
    int count = 0;
    // 길이 체크
    if(user_id.length() != banned_id.length()){ return false; }
    
    // 길이가 맞다면
    for(int i = 0; i < banned_id.length(); i++)
    {
        if(banned_id[i] == "*")
        {
            count++;
        }else
        {
            // 문자 미스매치
            if(banned_id[i] != user_id[i]){ return false; }
            else { count++; }
        }
    }
    
    if(count == banned_id.length()){ return true; }
    return false;
}

int solution(vector<string> user_id, vector<string> banned_id) {
    int answer = 0;
    return answer;
}
