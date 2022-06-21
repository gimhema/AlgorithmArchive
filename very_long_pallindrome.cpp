#include <iostream>
#include <string>
using namespace std;

int str_cursor = 0; // 0일땐 왼쪽부터 1일땐 오른쪽부터

int isPalindrome(string s)
{
    // s가 팰린드롬인지 여부를 판단함.
    
    // 길이를 리턴함
    return 0;
}

bool isSameStartEnd(string s)
{
    // 주어진 문자열의 시작과 끝이 같은지를 리턴한다.
    return false
}

int solution(string s)
{
    // 먼저 주어진 s자체가 팰린드롬인지 검사함.
    // s자체가 팰린드롬이면 그 자체로 이미 정답(기저조건)
    // 바로 s의 길이를 answer로 리턴
    
    // 팰린드롬을 가정할 수 있는 조건 1. 길이가 홀수
    // 조건 2. 첫 번째 문자와 끝 문자가 같아야함
    
    // 조건2번을 활용
    // 첫번째 문자와 끝 문자가 같은지 확인한다.(CheckStep)
        // 다르다면
        // 문자열 왼쪽 커서를 1칸 옮긴 뒤 다시 CheckStep을 한다.
        // 다르다면
        // 문자열 오른쪽 커서를 1칸 옮긴 뒤 다시 CheckStep을 한다.
    // 같다면 isCheckPallindrome을 진행한다.
        // true라면, 즉 Pallindrome이 맞고 길이가 나온다면 즉시 리턴후 종료
        // false라면, 즉 길이가 0이라면 다시 CheckStep으로 돌아간다
    // return;
    
    int answer=0;

    // [실행] 버튼을 누르면 출력 값을 볼 수 있습니다.
    cout << "Hello Cpp" << endl;

    return answer;
}
