/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <set>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

int main()
{
    
    // Vector Usage
    cout << "Vector" << endl;
    vector<int> vec1 = {1, 2, 3};
    vector<int> vec2 = {5, 6, 7};
    vec1.push_back(4);    
    // 벡터 병합
    vector<int>::iterator it = vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    // vec1 = 1 2 3 4 5 6 7
    for(const auto data : vec1)
    {
        cout << data << " ";
    }
    cout << endl;
    vec1.push_back(1);
    for(const auto data : vec1)
    {
        cout << data << " ";
    }
    cout << endl;
    
    // 오름차순
    sort(vec1.begin(), vec1.end());
    for(const auto data : vec1)
    {
        cout << data << " ";
    }
    cout << endl;
    
    // 내림차순
    sort(vec1.rbegin(), vec1.rend());
    for(const auto data : vec1)
    {
        cout << data << " ";
    }
    
    
    // Set Usage
    cout << endl;
    cout << "Set" << endl;
    set<int>::iterator s_it;
    set<int> i_set;
    
    i_set.insert(5);
    i_set.insert(3);
    i_set.insert(2);
    i_set.insert(1);
    i_set.insert(1);
    i_set.insert(7);
    i_set.insert(1);
    i_set.insert(12);
    
    // 1 2 3 5 7 12 . . .
    for(const auto val : i_set)
    {
        cout << val << " ";
    }
    cout << endl;
    
    s_it = i_set.find(30);
    if(s_it != i_set.end()){
        cout << "Find Sucessful" << endl; 
    }else{
        cout << "Find Fail " << endl; 
    }
    
    s_it = i_set.find(12);
    if(s_it != i_set.end()){
        cout << "Find Sucessful" << endl; 
    }else{
        cout << "Find Fail " << endl; 
    }
    
    // Queue Usage
    cout << endl;
    cout << "Queue" << endl;
    queue<int> i_q;
    i_q.push(1);
    i_q.push(2);
    i_q.push(3);
    i_q.push(4);
    i_q.push(1);
    i_q.push(2);
    i_q.push(1);
    i_q.push(100);
    while(!i_q.empty())
    {
        cout << i_q.front() << " ";
        i_q.pop();
    }

    // Priority Queue Usage
    cout << endl;
    cout << "Priority Queue" << endl;
    // 기본적으로는 내림차순으로 진행됨
    priority_queue<int> i_pq;
    i_pq.push(4);
    i_pq.push(11);
    i_pq.push(1);
    i_pq.push(2);
    i_pq.push(2);
    i_pq.push(3);
    i_pq.push(4);
    i_pq.push(4987);
    
    while(!i_pq.empty())
    {
        cout << i_pq.top() << " ";
        i_pq.pop();
    }
    cout << endl;
    // 오름차순으로 하려면
    priority_queue<int, vector<int>, greater<int>> i_pq2;
    i_pq2.push(4);
    i_pq2.push(11);
    i_pq2.push(1);
    i_pq2.push(2);
    i_pq2.push(2);
    i_pq2.push(3);
    i_pq2.push(4);
    i_pq2.push(4987);
    while(!i_pq2.empty())
    {
        cout << i_pq2.top() << " ";
        i_pq2.pop();
    }
    cout << endl;
    
    // Unordered Map Usage
    cout << endl;
    cout << "Unordered Map" << endl;
    unordered_map<string, int> i_umap;
    unordered_map<string, int>::iterator map_it;
    i_umap.insert({"Mino", 28});
    i_umap.insert({"PAKA", 29});
    i_umap.insert({"Ralo", 29});
    i_umap.insert({"DOPA", 29});
    i_umap.insert({"Mino", 28});
    
    for(const auto map_d : i_umap)
    {
        cout << "Key : " << map_d.first << " Val : " << map_d.second;
        cout << endl;
    }
    cout << endl;
    
    i_umap.insert({"Mino", 29});
    
    map_it = i_umap.find("Mino");
    if(map_it != i_umap.end())
    {
        cout << "Mino Find Sucessful" << endl;
        i_umap["Mino"] = 29;
        i_umap["young mino"] = 1;
    }
    
    for(const auto map_d : i_umap)
    {
        cout << "Key : " << map_d.first << " Val : " << map_d.second;
        cout << endl;
    }
    cout << endl;
    
    cout << endl;
    cout << "Program End . . . ." << endl;

    return 0;
}


