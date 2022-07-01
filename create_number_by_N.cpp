#include <string>
#include <vector>

using namespace std;

int preprocess(vector<vector<int>> &calc_vec, int number, int find_number)
{
    // 연속적인 숫자 8개를 만들어 미리 저장해둔다
    // N, NN, NNN . . . .
    calc_vec = {{number}, {(10*number + number)},
                {100*number + 10*number + number},
                {1000*number + 100*number + 10*number + number},
                {10000*number + 1000*number + 100*number + 10*number + number},
                {100000*number + 10000*number + 1000*number + 100*number + 10*number + number},
                {1000000*number + 100000*number + 10000*number + 1000*number + 100*number + 10*number + number},
                {10000000*number + 1000000*number + 100000*number + 10000*number + 1000*number + 100*number + 10*number + number}
               }
    // 2개의 수식으로 표현할 수 있는 수도 미리 저장해둔다
    // N+N, N-N, N*N, N/N
    calc_vec[1].push_back(number + number);
    calc_vec[1].push_back(number - number);
    calc_vec[1].push_back(number * number);
    calc_vec[1].push_back(number / number);
    
    // 만약 찾고자하는 수 find_number이 calc_vec에 있다면 바로 해당 인덱스를 리턴한다.
    int result = 9999;
    for(int i = 0; i < calc_vec.size(); i++)
    {
        for(int j = 0; j < calc_vec[i].size(); j++)
            if(calc_vec[i][j] == find_number)
            {
                result = i;
            }
    }
    
    return result;
}


int solution(int N, int number) {
    int answer = 0;
    vector<vector<int>> calc_vec;
    // calc_vec은 아래와같이 저장된다.
    int preprocess_result = preprocess(calc_vec, N, number);
    if(preprocess_result != 9999){answer = preprocess_result;}
    else
    {
        // calc_vec[0] = {N} 1번만 사용된 케이스
        // calc_vec[1] = {NN, N+N, N-N, N*N, N/N} 2번만 사용된 케이스
        // calc_vec[2] = {NNN, . . . .} 3번만 사용된 케이스
        // 저장하는 도중 number를 찾을 수 있다면 바로 반복자의 인덱스를 answer에 저장하고 끝낸다.
        // 모든 루프를 돌았다면 answer에 -1을 저장하고 끝낸다
    }    
    return answer;
}
