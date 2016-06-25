#include <iostream>

#include "testcell.h"
#include "testsim.h"

using namespace std;

Globals global;

int main()
{
    srand((unsigned int)(time(NULL)));

    // Initialize global variables
    if (!initGlobals())
    {
        cout << "Please place the parameters.txt file in the same directory \nas the ";
        cout << "executable for this program\n\n";

        return 0;
    }

    for (int i = 0 ; i < 1; i++)
    {
        TestSim testSim;
        cout << "Simulator Tests:\n";
        cout << testSim.allTests();

        TestCell testCell;
        cout << "Cell Tests:\n";
        cout << testCell.allTests();
    }

    return 0;
}
