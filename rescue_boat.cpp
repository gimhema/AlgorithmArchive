#include <string>
#include <vector>

using namespace std;



int solution(vector<int> people, int limit) {
    int answer = 0;
    int need_boat = 0;
    vector<bool> is_rescue(people.size(), false); 
    vector<bool> rescue_vec;    
    
    while(true)
    {
        if(rescue_vec.size() == people.size()) { break; }
        // bool redundancy_check = false;
        int boat_weight = limit;
        for(int i = 0; i < people.size(); i++)
        {
            if(is_rescue[i] == false && people[i] <= boat_weight)
            {
                boat_weight = boat_weight - people[i];
                is_rescue[i] = true;
                rescue_vec.push_back(true);
            }
            if(boat_weight == 0)
            {
                need_boat = need_boat + 1;
                // redundancy_check = true;
                break;
            }
        }
        if(redundancy_check == false) { need_boat = need_boat + 1; }
        // redundancy_check == false
    }
    answer = need_boat;
    
    return answer;
}
