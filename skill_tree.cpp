#include <string>
#include <vector>
#include <set>
#include <iostream>
using namespace std;


// CBD -> M[] = {C, B , D}
// C CB CBD
// C -> M[0], CB -> M[0] + M[1], CBD -> (M[0] + M[1]) + M[2]
// if CBDF
// C -> M[0], CB -> M[0] + M[1], CBD -> (M[0] + M[1]) + M[2], CBDF -> ((M[0] + M[1]) + M[2]) + M[3]
// F(0) = M[0]
// for(int i = 0; i < M.size(); i++) { temp = temp + M[i]; vec.push_back(temp); }
// BACDE CBADF AECB BDA
// BCD CBD CB BD

// 반복을 통해 가능한 스킬트리맵을 완성
// 매개변수 스킬트리값에서 공통스킬들 모두 제거
// 제거된 값이 맵에 존재하는지로 answer++

int solution(string skill, vector<string> skill_trees) {
    int answer = 0;
    
    string temp = "";
    set<string> skill_set;
    set<char> skills;
    set<string>::iterator iter;
    set<char>::iterator iter_c;
    vector<string> clear_strs;
    for(int i = 0; i < skill.size(); ++i)
    {
        skills.insert(skill[i]);
        temp = temp + skill[i];
        skill_set.insert(temp);
        // cout << temp << endl;
    }
    
    // cout << "----" << endl;
    
    for(int i = 0; i < skill_trees.size(); ++i)
    {
        string temp2 = "";
        for(int j = 0; j < skill_trees[i].size(); ++j)
        {
            iter_c = skills.find(skill_trees[i][j]);
            if(iter_c != skills.end())
            {
                temp2 += skill_trees[i][j];
            }
        }
        // cout << temp2 << endl;
        // 여기서 부분집합 검증
        iter = skill_set.find(temp2);
        if(iter != skill_set.end())
        {
            // 존재한다면
            cout <<* iter << " EXIST" << endl;
            answer++;
        }else if(temp2 == ""){answer ++;}
    }
    
    return answer;
}
