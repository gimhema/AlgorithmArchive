#include <string>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

int ticket_num;
vector<string> answer_ref;
vector<vector<string>> ticket_ref;

string CheckAlphabet(string a, string b)
{
    if(a.compare(b) < 0)
    {
        // A가 더 먼저일경우
        return a;
    }
    /// B가 더 먼저인 경우
    return b;
}

void UseTicket(string dest, int ticket_num)
{
    // answer_ref.push_back(dest);
    // ticket_ref.erase(ticket_ref.begin() + ticket_num);
    ticket_num = ticket_num - 1;
}


// 로직
// ICN을 출발지점으로 잡고 티켓 벡터를 탐색한다.
// 일단 티켓 하나를 집으면
// 그 티켓의 목적지와 같은 출발지점이 있는지 확인해야한다.
// 있다면 알파벳 순서를 비교한다.
// 더 알파벳 순서가 앞서는 티켓을 현재 티켓으로 고른다.
// 고른 티켓은 티켓 벡터에서 제거하고 출발지점을 해당 벡터의 0번째 원소로 변경한다.
// answer 벡터에 고른 티켓의 0, 1번째 원소를 push_bakc한다.
// 위의 과정을 반복한다.



void Travel(string dest)
{
    // dest는 tickets[1]이다.
    int use_ticket_num = -1;
    vector<string> current_ticket = { "" };
    string new_dest = dest;    
    while(true)
    {
        if(ticket_num <= 0) {break;}

        for(int i = 0; i < ticket_ref.size(); i++)
        {
            if(new_dest == ticket_ref[i][0])
            {
                if(current_ticket.size() > 1)
                {
                    vector<string> compare_ticket = ticket_ref[i];
                    // 현재 티켓이 존재하는 경우
                    if (CheckAlphabet(current_ticket[1], compare_ticket[1]) == current_ticket[1])
                    {
                        // 현재 티켓이 앞서는 경우
                        // current_ticket = compare_ticket;
                        // use_ticket_num = i;
                    }
                    else
                    {
                        // 탐색한 티켓이 앞서는 경우
                        current_ticket = compare_ticket;
                        use_ticket_num = i;
                    }
                }
                else if(current_ticket.size() == 1)
                {
                    // 존재하지 않는 경우
                    use_ticket_num = i;
                    current_ticket = ticket_ref[i];
                }
                // else
                // {
                //     use_ticket_num = i;
                //     current_ticket = ticket_ref[i];
                // }
            }
        }
        ticket_ref.erase(ticket_ref.begin() + use_ticket_num);
        ticket_num = ticket_num - 1;
        
        new_dest = current_ticket[1];
        answer_ref.push_back(new_dest);
        
        current_ticket = { "" };
        
        if(ticket_num <= 0) {break;}
    }
    return;
}



vector<string> solution(vector<vector<string>> tickets) {
    vector<string> answer;
    ticket_num = tickets.size();
    
    ticket_ref = tickets;
    answer_ref.push_back("ICN"); // First Destination
    
    Travel("ICN");
    
    answer = answer_ref;
    return answer;
}

