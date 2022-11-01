#include <string>
#include <vector>
#include <set>
#include <iostream>
using namespace std;

// 합은 set구조에 저장
// 7 9 1 1 4
// 7 9 1 4 -> 7 9 1 1(x) 4
// (7) (9) (1) (1) (4) !(7)
// (7+9) (9+1) (1+1) (1+4) (4+7) !(7+9)
// (7+9+1) (9+1+1) (1+1+4) (1+4+7) (4+7+9) !(7+9+1)
// (7+9+1+1) (9+1+1+4) (1+1+4+7) (1+4+7+9) (4+7+9+1) !(7+9+1+1)
// . . . .
// if sum_count == N.size()? go to next step
// N1 [n1, n2, n3, n4, n5]
// N2 [n1+n2, n2+n3, n3+n4, n4+n5, n5+n1] -> if n_idx > N.end? set n_idx 0, and ++ . . .
// N3 [n1+n2+n3, n2+n3+n4, n3+n4+n5, n4+n5+n1, n5+n1+n2]
// N4 [n1 +n2+n3+n4, n2 +n3+n4+n5, n3 +n4+n5+n1, n4 +n5+n1+n2, n5 +n1+n2+n3]

// N[N] = [(N[N-1] + nN) ]

// 7 9 1 1 4 7 9 1 1 4

// 마지막 값은 다 더해서 set에 insert
// answer = set.num()
int solution(vector<int> elements) {
    int answer = 0;
    
    vector<int> circular = elements;

    vector<int>::iterator it = circular.insert(circular.end(), elements.begin(), elements.end());
    set<int> sums_set;
    // 기저
    // N[0], N[MAX] -> N[0]의 경우 모든 원소들을 insert하고 N[MAX]합을 미리 insert한다.
    int t_sum = 0;
    for(int i = 0; i < elements.size(); ++i)
    {
        t_sum += elements[i];
        sums_set.insert(elements[i]);
    }
    sums_set.insert(t_sum);

    int count = 0;
    int sum_count = 1;
    int n_idx = 0;
    int temp = 0;    
    for(int i = 0; i < elements.size(); i++)
    {
        for(int j = 0; j < elements.size(); j++)
        {
            temp = 0;
            for(int k = j; k < j + sum_count; k++)
            {
                temp += circular[k];
            }
            sums_set.insert(temp);
        }
        sum_count++;
    }
    
    answer = sums_set.size();
    return answer;
}
