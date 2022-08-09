#include <string>
#include <vector>

using namespace std;

int is_can_convert(string begin, string target, string final_target)
{
    int count = 0;
    int final_count = 0;
    for(int i = 0; i < begin.size(); i++)
    {
        if(begin[i] == target[i])
        {
            count++;
        }
        if(begin[i] == final_target[i])
        {
            final_count++;
        }
    }
    if(count == 1)
    {
        return 1; 
    }
    else if(final_count == 1)
    {
        return 2;
    }
    return 0;
}

bool has_target_in_words(string target, vector<string> &words)
{
    for(int i = 0; i < words.size(); i++)
    {
        if(target == words[i])
        {
            return true;
        }
    }
    return false;
}

int solution(string begin, string target, vector<string> words) {
    int answer = 0;
    int count = 0;
    bool is_can_arrive = false;
    string begin_ref = begin;
    string target_ref = target;
    vector<bool> visited(words.size(), false);
    bool find_final = false;
    if(!has_target_in_words(target, words))
    {
        is_can_arrive = false;
    }
    else
    {
     while(true)
    {
        bool can_convert = false;

        if(find_final)  { break; }
        for(int i = 0; i < words.size(); i++)
        {
            // if(is_can_convert(begin_ref, words[i], target) == 2 && visited[i] == false)
            // {
            //     // begin_ref = words[i];
            //     count++;
            //     find_final = true;
            //     is_can_arrive = true;
            //     can_convert = true;
            //     break;
            // }
            if(begin_ref == target_ref) { find_final = true; }
            else if(is_can_convert(begin_ref, words[i], target) == 1 && visited[i] == false)
            {
                begin_ref = words[i];
                count++;
                is_can_arrive = true;
                can_convert = true;
                visited[i] = true;
                if(begin_ref == target_ref) { find_final = true; }
                break;
            }
        }
        if(!can_convert)
        { 
            is_can_arrive = false;
            break; 
        }
    }   
    }
    
    if(is_can_arrive == false) { answer = 0; }
    else { answer = count; }
    return answer;
}
