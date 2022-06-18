#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

struct MemberInfo
{
    string referral,
    string seller,
    int amount,
    int wallet
};

MemberInfo CreateMemberInfo(string referral, string seller, int amount)
{
    MemberInfo m;
    m.referral = referral;
    m.seller = seller;
    m.amount = amount;
    return m;
}

unordered_map<string, MemberInfo> LetsStartPyramidBusiness(vector<string> enroll, vector<string> referral, vector<string> seller, vector<int> amount)
{
    unordered_map<string, MemberInfo> company;
        
    // Setting Enroll
    for(int i = 0; i < enroll.size(); i++)
    {
        if(referral[i] != "-")
        {
            company.insert({enroll[i], CreateMemberInfo(referral[i],"AA",-1)})    
        }else
        {
            company.insert({enroll[i], CreateMemberInfo("-","AA",-1)})
        }
        
    }
    // Setting Seller & Amount
    for(int i = 0; i < seller.size(); i++)
    {
        company[seller[i]].seller = seller[i];
        company[seller[i]].amount = amount[i];
        company[seller[i]].wallet = amount[i] * 100;
    }
}

// 전역변수
// 다단계 회사 창업을 시작한다.
int CEOWallet = 0; // 회사 대표의 지갑.
unordered_map<string, MemberInfo> my_pyramid_company;

int CalcFee(int incom){return incom / 10;}
void FeeCalculation(string enroll ,string referral, int fee)
{
 
}

vector<int> OperationIncomCalculation()
{
    int CEO_wallet = 0;
    vector<int> result;
    // 수익이라는건
    // 자식노드로부터 얻는 수수료 + 본인 판매수익
    // 판매수익과 수수료수익을 달리하여 계산
    // 판매수익은 10퍼빼고 바로 다이렉트로 반영
    // 수수료 수익은 재귀함수로 구현
    
    for(auto elem : my_pyramid_company){
        cout<<"key : "<<elem.first<<" value : "<<elem.second<<endl;
        int _amount = elem.second.amount;
        int fee = CalcFee(fee);
        elem.second.wallet = _amount - fee;
        FeeCalculation(elem.second.enroll,
                      elem.second.referral,
                      fee);
    }
    
    return result;
}


vector<int> solution(vector<string> enroll, vector<string> referral, vector<string> seller, vector<int> amount) {

    my_pyramid_company = LetsStartPyramidBusiness(
    enroll, referral, seller, amount);
    
    // 생성된 회사 정보를 바탕으로 영업이익을 계산한다.
    // 계산한 결과를 리턴하고 종료
    vector<int> answer = OperationIncomCalculation();
    return answer;
}
