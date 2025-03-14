#include"SymPairTester.h"

SymPairTester::SymPairTester(int times, FILE *AsyPairingParamFile, int strLen, Tester& _Tester, std::ofstream &outputFile)
    :e(AsyPairingParamFile)
{
    //随机选取运算参数，times+1个
    for (int i = 0; i <= times;i++){
        G1Args.emplace_back(e, false);
        ZrArgs.emplace_back(e, false);
        strArgs.emplace_back(getRandomString(strLen));
    }
    //要测试的算法加入测试队列中
    {
        algArr.emplace_back(bind(&SymPairTester::PointMulG1, this, placeholders::_1));
        algArr.emplace_back(bind(&SymPairTester::PointAddG1,this,placeholders::_1));
        algArr.emplace_back(bind(&SymPairTester::MapG1,this,placeholders::_1));
        algArr.emplace_back(bind(&SymPairTester::GetPair,this,placeholders::_1));
        algNameArr = {"SymPointMulG1", "SymPointAddG1", "SymMapG1", "SymGetPair"};
    }
    run();
}

SymPairTester::~SymPairTester()
{
}

void SymPairTester::PointMulG1(int i)
{
    auto res = G1Args[i] ^ ZrArgs[i];
}

void SymPairTester::PointAddG1(int i)
{
    auto res = G1Args[i] * G1Args[i + 1];
}

void SymPairTester::MapG1(int i)
{
    auto res = G1(e, &strArgs[i], sizeof(strArgs[i]));
}
void SymPairTester::GetPair(int i)
{
    auto res = e(G1Args[i], G1Args[i]);
}
