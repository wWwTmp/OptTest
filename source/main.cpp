#include <iostream>
#include "OperatorTester.h"

int main()
{
    FILE *sysParamFile = fopen("/home/vansgay/code/LibSrc/pbc-0.5.14/param/f.param", "r");
    if (sysParamFile == NULL)
    {
        cerr << "Can't open the parameter file " << "\n";
        return 0;
    }
    OperatorTester optTester(100, sysParamFile);
    optTester.run();
}