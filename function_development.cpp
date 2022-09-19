#include <string>
#include <vector>

using namespace std;

// 남은 일수는 speeds를 progresses에 계속 더해서 100을 넘어가면 그 카운트를 계산해서 넣는다.
// 그러면 남은 일수 배열이 완성된다
// [ 7, 3, 9 ] -> [2, 1]
// [5, 10, 1, 1, 20, 1] -> [1, 3, 2] 


vector<int> remains;
vector<int> answer_ref;

int calc_remain(int progress, int speed)
{
    int p = progress;
    int s = speed;
    int count = 0;
    while(true)
    {
        if(p >= 100) {break;}
        else
        {
            p = p + s;
            count++;
        }
    }
    
    return count;
}

void init_remains(vector<int> progresses, vector<int> speeds)
{
    for(int i = 0; i < progresses.size(); i++)
    {
        remains.push_back(calc_remain(progresses[i], speeds[i]));
    }
}

void calc_answer()
{
    // 결과 값을 계산할 때 pivot을 정한다. pivot은 최초에 remains의 맨 앞의 원소이다.
    // 그리고 completes라는 카운트 값을 하나 정한다. 0부터 시작한다.
    // 남은 일수 배열을 탐색하면서 pivot보다 작거나 같으면 건너뛰고 completes를 1 더한다
    // 만약 탐색하는 원소가 pivot보다 크다면 현재 completes를 정답 배열에 push하고 completes를 0으로 초기화한뒤
    // pivot을 해당 원소로 바꾸고 위의 과정을 다시 진행한다.
    
    // answer_ref
    int pivot = remains[0];
    int completes = 1;
    for(int i = 1; i < remains.size(); i++)
    {
        if(pivot >= remains[i])
        {
            completes++;
        }
        else
        {
            answer_ref.push_back(completes);
            completes = 1;
            pivot = remains[i];
        }
    }
    answer_ref.push_back(completes);
}

vector<int> solution(vector<int> progresses, vector<int> speeds) {
    vector<int> answer;
    init_remains(progresses, speeds);
    calc_answer();
    answer = answer_ref;
    return answer;
}


