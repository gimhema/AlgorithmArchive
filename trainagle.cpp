#include <string>
#include <vector>

using namespace std;
// 합을 저장, 업데이트할때마다 정렬함
vector<int> total_sums;
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
        for(int j = 0; j < triangle[next_idx].size(); j++)
        {
            current_sums.push_back()
        }
    }
    
    return answer;
}
