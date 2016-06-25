#include "testcell.h"

TestCell::TestCell()
{
    //Nothing to do here...
}

string TestCell::allTests()
{
    //For each testing function, run and concatenate the results to output string
    string output = "";
    output += testConstructor();
    output += testStep();
    output += testEvolve();
    output += testAssign_Apoptosis();
    output += testAssign_Proliferation();
    output += testAssign_Differentiation();
    output += testAssign_Move();

    if (output == "")
        output = "All tests passed successfully\n";

    return output;
}

string TestCell::testConstructor()
{
    Cell* testCell;
    CellType type = UNDIFFERENTIATED_SPERMATOGONIA;
    unsigned int x;
    unsigned int y;
    stringstream output;

    //Construct a number of cells and check for appropriate construction
    for (int i = 0; i < 10; i++)
    {
        //Generate some varied locations
        x = i * 42;
        y = i * 256;
        testCell = new Cell(type,pair<unsigned int, unsigned int>(x,y));
        if (testCell->type != type)
            output << "testConstructor Failure: \tCellType '" << toString(type)
                      << "'constructed as '" << toString(testCell->type)
                      << "'\n";
        if (x != testCell->position.first || y != testCell->position.second)
            output << "testConstructor Failure: \tposition '(" << x << ","
                      << y << ")' constructed as '("
                      << testCell->position.first << ","
                      << testCell->position.second << ")'\n";
        type++;
        delete testCell;
    }
    return output.str();
}

string TestCell::testStep()
{
    //<----------------------------------------Test Step------------------------------------------->
    // We will generate a set of Cells for a test of the function Step(). As step contains such a
    // large scope of functions, this test is more of a blanket test to check if the Cell is working
    // as expected. More specific testing will be performed on the more specific functions. This
    // test is implemented to help track problems into the Cell step process.

    // For the set of generated Cells:
    //
    // 1) Generate a varied location for the Cell
    // 2) Assign Cell varied behaviors
    // 3) Run Cell steps() until each behavior has been triggered. For each step:
    //      1) Reset behavior booleans to false
    //      2) Run the step
    //      3) Determine if the signals triggered at their expected times.
    //      4) Ensure the behavior signals are never triggered again after being triggered. Set the
    //         behavior signal timers to larger than the number of times we will call step()
    //<-------------------------------------------------------------------------------------------->
    stringstream output;
    Cell* testCell;
    CellType type = UNDIFFERENTIATED_SPERMATOGONIA;
    bool arr[4];
    int x, y;
    string action;

    for (int i = 0; i < 10; i++)
    {
        // Generate a varied location for the Cell
        x = i * 32;
        y = i * 8;
        testCell = new Cell(type,pair<unsigned int, unsigned int>(x,y));

        // Assign Cell varied behaviors
        testCell->proliferation = 1;
        testCell->apoptosis = 2;
        testCell->move = 3;
        testCell->differentiation = 4;

        // Run Cell steps() until each behavior has been triggered.
        for (int j = 1; j < 5; j++)
        {
            // Reset behavior booleans to false
            for (int n = 0; n < 4; n++)
                arr[n] = false;

            // Run the step
            testCell->step(&arr[0],&arr[1],&arr[2],&arr[3]);

            // Determine if the signals triggered at their expected times.
            for (int n = 0; n < 4; n++)
            {
                switch (n)
                {
                case 0: action = "proliferation";
                    break;
                case 1: action = "apoptosis";
                    break;
                case 2: action = "move";
                    break;
                case 3: action = "differentiation";
                    break;
                }
                // Ensure the behavior signals are never triggered again after being triggered.
                if (n + 1 == j)
                {
                    switch (n)
                    {
                    case 0: testCell->proliferation = 5;
                        break;
                    case 1: testCell->apoptosis = 5;
                        break;
                    case 2: testCell->move = 5;
                        break;
                    case 3: testCell->differentiation = 5;
                        break;
                    }
                }

                if (n + 1 == j && arr[n] != true)
                {
                    if (type != ELONGATED_SPERMATIDS || action != "differentiation")
                    {
                        output << "testStep Failure: " << action << " failed to trigger on type ";
                        output << toString(type) << endl;
                    }
                }
                else if (n + 1 != j && arr[n] == true)
                {
                    output << "testStep Failure: " << action << " triggered incorrectly";
                    output << endl;
                }
            }
        }
        delete testCell;
        type++;
    }

    return output.str();
}

string TestCell::testEvolve()
{
    //<-----------------------------------Test Evolve---------------------------------------------->
    // 1) Create a set of Cells for each CellType
    // 2) For each Cell in the set:
    //      1) Evolve the Cell
    //      2) Determine if the new CellType is as expected
    //<-------------------------------------------------------------------------------------------->

    stringstream output;

    // Create a set of Cells for each CellType
    int type_int = static_cast<int>(ELONGATED_SPERMATIDS);
    Cell* testCell;
    for (int i = 0; i <= type_int; i++)
    {
        testCell = new Cell(static_cast<CellType>(i), pair<unsigned int, unsigned int>(0, 0));

        // Evolve the Cell
        testCell->evolve();

        // Determine if the new CellType is as expected
        if (i < type_int && testCell->type != static_cast<CellType>(i+1))
        {
            output << "testEvolve Failure: " << toString(static_cast<CellType>(i));
            output << " evolved to " << toString(testCell->type) << endl;
        }
        if (i == type_int && testCell->type != ELONGATED_SPERMATIDS)
        {
            output << "testEvolve Failure: Elongated Spermatids evolved to ";
            output << toString(testCell->type) << endl;
        }
        delete testCell;
    }
    return output.str();
}

string TestCell::testAssign_Apoptosis()
{
    //<------------------------------Test Assign Apoptosis----------------------------------------->
    // 1) For a determined number of tests:
    //      1) Generate a Cell of each CellType
    //      2) Call assign_apoptosis()
    //      3) Determine if the assigned apoptosis value is within the #defined min and max
    // 2) Determine if the apoptosis value was assigned the same each time for that cell type
    //<-------------------------------------------------------------------------------------------->

    stringstream output;
    Cell* testCell;
    int lastNum = 0;
    bool all_same = true;

    // Generate a Cell of each CellType
    for (int i = 0; i <= static_cast<int>(ELONGATED_SPERMATIDS); i++)
    {
        all_same = true;
        for (int n = 0; n < 10; n++)
        {
            testCell = new Cell(static_cast<CellType>(i), pair<unsigned int, unsigned int>(0, 0));

            // Call assign_apoptosis()
            testCell->assign_apoptosis();

            // Determine if the assigned apoptosis value is within the #defined min and max
            if (testCell->apoptosis > global.APOPTOSIS_MAX[i] || testCell->apoptosis <
                    global.APOPTOSIS_MIN[i])
            {
                output << "testAssign_Apoptosis Failure: " << toString(testCell->type);
                output << " assigned apoptosis of " << testCell->apoptosis << endl;
            }

            if (testCell->apoptosis != lastNum || global.APOPTOSIS_MIN[i] ==
                    global.APOPTOSIS_MAX[i])
                all_same = false;

            lastNum = testCell->apoptosis;
        }
        // Determine if the an apoptosis value was assigned the same each time for that cell type
        if (all_same)
        {
            output << "testAssign_Apoptosis Failure: Same apoptosis value assigned the same value ";
            output << "every time for type " << toString(static_cast<CellType>(i)) << endl;
        }
    }
    return output.str();
}

string TestCell::testAssign_Proliferation()
{
    //<-------------------------------Test Assign Proliferation------------------------------------>
    // 1) For a determined number of tests:
    //      1) Generate a Cell of each CellType
    //      2) Call assign_proliferation()
    //      3) Determine if the assigned proliferation value is within the #defined min and max
    // 2) Determine if the proliferation value was assigned the same each time for that cell type
    //<-------------------------------------------------------------------------------------------->

    stringstream output;
    Cell* testCell;
    int lastNum = 0;
    bool all_same;

    // Generate a Cell of each CellType
    for (int i = 0; i <= static_cast<int>(ELONGATED_SPERMATIDS); i++)
    {
        all_same = true;
        for (int n = 0; n < 10; n++)
        {
            testCell = new Cell(static_cast<CellType>(i), pair<unsigned int, unsigned int>(0, 0));

            // Call assign_proliferation()
            testCell->assign_proliferation();

            // Determine if the assigned proliferation value is within the #defined min and max
            if (testCell->proliferation > global.PROLIFERATION_MAX[i] ||
                    testCell->proliferation < global.PROLIFERATION_MIN[i])
            {
                output << "testAssign_Proliferation Failure: " << toString(testCell->type);
                output << " assigned proliferation of " << testCell->proliferation << endl;
            }

            if (testCell->proliferation != lastNum || global.PROLIFERATION_MIN[i] ==
                    global.PROLIFERATION_MAX[i])
                all_same = false;

            lastNum = testCell->proliferation;
        }
        // Determine if the proliferation value was assigned the same each time for that cell type
        if (all_same)
        {
            output << "testAssign_Proliferation Failure: Same proliferation value assigned the ";
            output << "same value every time for type " << toString(static_cast<CellType>(i));
            output << endl;
        }
    }
    return output.str();
}

string TestCell::testAssign_Differentiation()
{
    //<------------------------------Test Assign Differentiation----------------------------------->
    // 1) For a determined number of tests:
    //      1) Generate a Cell of each CellType
    //      2) Call assign_differentiation()
    //      3) Determine if the assigned differentiation value is within the #defined min and max
    // 2) Determine if the differentiation value was assigned the same each time for that cell type
    //<-------------------------------------------------------------------------------------------->

    stringstream output;
    Cell* testCell;
    int lastNum = 0;
    bool all_same = true;

    // Generate a Cell of each CellType
    for (int i = 0; i <= static_cast<int>(ELONGATED_SPERMATIDS); i++)
    {
        all_same = true;
        for (int n = 0; n < 10; n++)
        {
            testCell = new Cell(static_cast<CellType>(i), pair<unsigned int, unsigned int>(0, 0));

            // Call assign_differentiation()
            testCell->assign_differentiation();

            // Determine if the assigned differentiation value is within the #defined min and max
            if (testCell->differentiation > global.DIFFERENTIATION_MAX[i] ||
                    testCell->differentiation < global.DIFFERENTIATION_MIN[i])
            {
                output << "testAssign_Differentiation Failure: " << toString(testCell->type);
                output << " assigned differentiation of " << testCell->differentiation << endl;
            }
            if (testCell->differentiation != lastNum ||
                    global.DIFFERENTIATION_MIN[i] != global.DIFFERENTIATION_MAX[i])
                all_same = false;

            lastNum = testCell->differentiation;
        }
        // Determine if the differentiation value was assigned the same each time for that cell type
        if (all_same)
        {
            output << "testAssign_Differentiation Failure: Same differentiation value assigned the";
            output << " same value every time for type " << toString(static_cast<CellType>(i));
            output << endl;
        }
    }
    return output.str();
}

string TestCell::testAssign_Move()
{
    //<------------------------------Test Assign Move----------------------------------------->
    // 1) For a determined number of tests:
    //      1) Generate a Cell of each CellType
    //      2) Call assign_move()
    //      3) Determine if the assigned move value is within the #defined min and max
    // 2) Determine if the move value was assigned the same each time for that cell type
    //<-------------------------------------------------------------------------------------------->

    stringstream output;
    Cell* testCell;
    int lastNum = 0;
    bool all_same = true;

    // Generate a Cell of each CellType
    for (int i = 0; i <= static_cast<int>(ELONGATED_SPERMATIDS); i++)
    {
        all_same = true;
        for (int n = 0; n < 10; n++)
        {
            testCell = new Cell(static_cast<CellType>(i), pair<unsigned int, unsigned int>(0, 0));

            // Call assign_move()
            testCell->assign_move();

            // Determine if the assigned move value is within the #defined min and max
            if (testCell->move > global.SPEED_MAX[i] || testCell->move < global.SPEED_MIN[i])
            {
                output << "testAssign_move Failure: " << toString(testCell->type);
                output << " assigned move of " << testCell->move << endl;
            }

            if (testCell->move != lastNum || global.SPEED_MIN[i] == global.SPEED_MAX[i])
                all_same = false;

            lastNum = testCell->move;
        }
        // Determine if the move value was assigned the same each time for that cell type
        if (all_same)
        {
            output << "testAssign_Move Failure: Same move value assigned the same value each ";
            output << "time for type " << toString(static_cast<CellType>(i)) << endl;
        }
    }
    return output.str();
}
