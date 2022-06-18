#include <string>
#include <vector>

using namespace std;
// 최대값, 업데이트할때마다 정렬함
vector<int> total_max_sums;
// 현재 합을 저장함
vector<int> current_sums;

int solution(vector<vector<int>> triangle) {
    int answer = 0;
    
    if(triangle.size() == 1)
    {
        return triangle[0][0];
    }
    else
    {
        current_sums.push_back(triangle[0][0]);
    }
    
    for(int i = 0; i < triangle.size() - 1; i++)
    {
        int next_idx = i + 1;
        vector<int> check_point = current_sums;
        current_sum.clear();
        for(int j = 0; j < triangle[next_idx].size(); j++)
        {
            for(int k = 0; k < check_point.size(); k++)
            {
                int _sum = check_point[k] + triangle[j];
                current_sums.push_back(_sum);
                total_max_sums.push_back(_sum);
                // total_max_sums 정렬
            }
        }
    }
    
    // 내림차순으로 정렬되었기때문에 가장 앞의 원소를 리턴함.
    answer = total_max_sums[0];
    
    return answer;
}

