#pragma once

#include<pbc++/PBC.h>
#include<iostream>
#include<string>
#include<vector>
#include<functional>
#include"Tester.h"


class SymPairTester
{
public:
    SymPairTester(int times, FILE* AsyPairingParamFile, int strLen, std::ofstream &outputFile);
    SymPairTester(SymPairTester &copy) = delete;
    ~SymPairTester();

private:
    Tester tester;
    Pairing e;
    std::vector<G1> G1Args;
    std::vector<Zr> ZrArgs;
    std::vector<string> strArgs;
    //Opt algs
    void PointMulG1(int i);
    void PointAddG1(int i);
    void MapG1(int i);
    void GetPair(int i);
};