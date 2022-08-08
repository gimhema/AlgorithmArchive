#include <string>
#include <vector>
#include <algorithm>

using namespace std;



int solution(vector<int> people, int limit) {
    int answer = 0;
    int need_boat = 0;
    sort(people.begin(), people.end());
    vector<bool> is_rescue(people.size(), false); 
    vector<bool> rescue_vec;
    int rescue_num = 0;
    
    while(true)
    {
        if(rescue_num == people.size()) { break; }
        int boat_weight = limit;
        for(int i = 0; i < people.size(); i++)
        {
            if(is_rescue[i] == false && people[i] <= boat_weight)
            {
                boat_weight = boat_weight - people[i];
                is_rescue[i] = true;
                // rescue_vec.push_back(true);
                rescue_num++;
            }
        }
        need_boat = need_boat + 1;
    }
    
    answer = need_boat;
    return answer;
}
