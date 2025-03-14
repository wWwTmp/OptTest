#pragma once

#include <functional>
#include<vector>
#include<iostream>
#include<fstream>
#include<chrono>
#include<string>

class Tester{
public:
    Tester(int inputTimes, std::ofstream& outputFile);
    ~Tester(){};
    void run();

protected:
    std::vector <std::function<void(int)>> algArr;
    std::vector<std::string> algNameArr;

private:
    int runTimes;
    std::ofstream* resFile;
    double TestAlg(std::function<void(int)> f);
};