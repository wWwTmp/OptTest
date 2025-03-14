#include"Tester.h"

Tester::Tester(int inputTimes, std::ofstream &outputFile)
    :runTimes(inputTimes), resFile(&outputFile)
{}

void Tester::run()
{
    for (int i = 0; i < algArr.size(); i++)
    {
        auto res=TestAlg(algArr[i]);
        *resFile << algNameArr[i] << ',' << res << std::endl;
    }
}

double Tester::TestAlg(std::function<void(int)> f)
{

    std::chrono::duration<double, std::milli> duration;
    for (int i = 0; i < runTimes; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        f(i);
        auto end = std::chrono::high_resolution_clock::now();
        duration += (end - start);
    }
    return duration.count() / runTimes;
}
