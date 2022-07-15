#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// 요청 시간은 고정되어 있다.
// 내가 지금 이 작업을 수행하면 언제 끝낼 수 있을까 . . .
// 가장 짧은 작업을 선택해야겠다 . . .

// 내가 지금 = 현재 시간(작업을 완전히 끝냈을 때 업데이트됨)
// 이 작업을 = Job
// 언제 끝낼 수 있을까 = 
// 만약 대기시간이 현재 시간보다 작거나 같은 경우 = (현재 시간 - 대기 시간) + 수행 시간
// 대기시간이 현재 시간보다 큰 경우 = (현재 시간 + 대기 시간) + 수행 시간

int current_time = 0;
int process_coint = 0;
int selected_job_idx = -5; // 가장 짧은 작업시간을 가진 작업의 인덱스를 여기에 기록한다.
int min_job_time = 0; // 가장 짧은 작업시간을 여기에 기록한다.

vector<int> total_job_time; // (현재시간 -+ 대기시간) + 수행시간 결과를 저장
vector<int> job_time_calc_temp; // 계산 과정을 저장
vector<vector<int>> jobs_ref;

// 수행이 끝난 작업은, 작업시간을 -1로 돌린다.

int calc_jobs_avg()
{
    int sum = 0;
    for(int i = 0; i < total_job_time.size(); i++)
    {
        sum = sum + total_job_time[i];
    }
    return sum / total_job_time.size();
}

int solution(vector<vector<int>> jobs) {
    jobs_ref = jobs;
    int wait_v = 0;
    int answer = 0;
    while(total_job_time.size() != jobs.size())
    {
        selected_job_idx = -5;
        min_job_time = 9999;
        for(int i = 0; i < jobs.size(); i++)
        {
            if(jobs_ref[i][0] != -1)
            {
                if(jobs[i][0] <= current_time)
                {
                    int t = (current_time - jobs[i][0]) + jobs[i][1];
                    if(t <= min_job_time)
                    {
                        selected_job_idx = i;
                        min_job_time = t;
                    }
                }
                else
                {
                    int t = (current_time + jobs[i][0]) + jobs[i][1];
                    if(t <= min_job_time)
                    {
                        selected_job_idx = i;
                        min_job_time = t;
                        
                    }
                }
                // min_job_time = 100000000;
            }
        }
        jobs_ref[selected_job_idx][0] = -1;
        current_time = current_time + jobs[selected_job_idx][1];
        total_job_time.push_back(min_job_time); // 가장 앞 원소, 즉 최소값을 저장함
        process_coint++;
    }

    answer = calc_jobs_avg();
    return answer;
}



