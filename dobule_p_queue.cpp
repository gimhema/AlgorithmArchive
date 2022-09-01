#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

// 이거 큐로 구현하는거 맞나????????
// 최소값은 뒤에
// 최대값은 앞에
vector<int> answer_vec;

void delete_max()
{
     answer_vec.pop_back();
}

void delete_min()
{
    answer_vec.erase(answer_vec.begin());
}

void insert_queue(int val)
{
    answer_vec.push_back(val);
    sort(answer_vec.begin(), answer_vec.end()); 
}

void parsing_operator(string target)
{
    // string oper;
    // string val;

    stringstream ss(target);

    string oper, val;
    ss >> oper >> val;

    int val_intger;
    std::stringstream ssInt(val);
    ssInt >> val_intger;
    if(oper == "I")
    {
        insert_queue(val_intger);
    }
    else
    {
        if(answer_vec.empty() == 0)
        {
            if(val_intger == 1)
            {
                delete_max();
            }
            else
            {
                delete_min();
            }            
        }
    }
}



vector<int> solution(vector<string> operations) {
    vector<int> answer;

    for(int i = 0; i < operations.size(); i++)
    {
        parsing_operator(operations[i]);
    }

    // answer = {0, 0};
    if(answer_vec.empty() == 1)
    {
        answer = {0, 0};    
    }
    else
    {
    answer = {answer_vec.back(), answer_vec.front()};        
    }

    return answer;
}
