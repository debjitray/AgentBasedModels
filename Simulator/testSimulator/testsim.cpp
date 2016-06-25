#include "testsim.h"

// PUBLIC METHODS

TestSim::TestSim()
{
    // Nothing to do here...
}

string TestSim::allTests()
{
    //For each testing function, run and concatenate the results to output string
    string output = "";
    output += testConstructor();
    output += testStep();
    output += testPopulate();
    output += testPopulate_Type();
    output += testMove();
    output += testFind_Forward();
    output += testSpace_Availiable();

    if (output == "")
        output = "All tests passed successfully\n";

    return output;
}

string TestSim::printSim(Simulator* testSim, bool behavior)
{
    stringstream output;

    output << "PRINTING SIMULATOR" << endl;
    output << "------------------" << endl;

    multimap<unsigned long long, Cell*>::iterator it;
    for (it = testSim->population.begin(); it != testSim->population.end(); it++)
    {
        output << toString(it->second->get_type()) << ": (" << it->second->position.first << ",";
        output << it->second->position.second << ")" << endl;
        if (behavior)
        {
            output << "Apoptosis: " << it->second->apoptosis << endl;
            output << "Proliferation: " << it->second->proliferation << endl;
            output << "Movement: " << it->second->proliferation << endl;
            output << "Differentiation: " << it->second->differentiation << endl;
            output << endl << endl;
        }
    }

    output << "------------------" << endl;

    return output.str();
}

string TestSim::testConstructor()
{
    //<-------------------------------------Test Constructor--------------------------------------->
    // We'll run a simple test to ensure we have elements which are not on top of each other and are
    // along the edge for both the blood_vessels array and sertoli array.
    //
    // 1) Construct a Simulator.
    // 2) Determine if any of the blood vessel points are on top of each other
    // 3) Determine if any of the sertoli cells are on top of each other
    //
    //    We will determine if points are near but inside the edge of the environment by ensuring
    //    they are farther from the center than the environment radius - 1.5 grid square, but not
    //    farther from the center than the environment radius (that is: radius - 1.5 < x <= radius).
    //
    // 4) Determine if each of the blood vessel points are near but inside the edge of the
    //    environment
    // 5) Determine if each of the sertoli cell nuclei are near but inside the edge of the
    //    environment
    //
    // Note: Expected blood vessel and sertoli locations must be defined or removed if
    //       BLOOD_VESSEL_NUMBER or SERTOLI_NUMBER is changed. This test will fail (or crash) until
    //       this is done.
    //<-------------------------------------------------------------------------------------------->
    stringstream output;

    // Construct a Simulator
    Simulator testSim;

    // Determine if any of the blood vessel points are on top of each other
    pair<unsigned int, unsigned int>expected_blood[5];
    expected_blood[0] = pair<unsigned int, unsigned int>(22, 11);
    expected_blood[1] = pair<unsigned int, unsigned int>(14, 21);
    expected_blood[2] = pair<unsigned int, unsigned int>(2, 17);
    expected_blood[3] = pair<unsigned int, unsigned int>(3, 5);
    expected_blood[4] = pair<unsigned int, unsigned int>(13, 1);

    for (int i = 0; i < global.BLOOD_VESSEL_NUMBER; i++)
    {
        if (testSim.blood_vessels[i] != expected_blood[i])
        {
            output << "testConstructor Failure: Blood vessels placed at location (";
            output << testSim.blood_vessels[i].first << "," << testSim.blood_vessels[i].second;
            output << ")" << endl;
        }
    }

    // Determine if any of the sertoli cells are on top of each other
    pair<unsigned int, unsigned int>expected_sertoli[9];
    expected_sertoli[0] = pair<unsigned int, unsigned int>(22, 11);
    expected_sertoli[1] = pair<unsigned int, unsigned int>(19, 18);
    expected_sertoli[2] = pair<unsigned int, unsigned int>(12, 21);
    expected_sertoli[3] = pair<unsigned int, unsigned int>(5, 20);
    expected_sertoli[4] = pair<unsigned int, unsigned int>(1, 13);
    expected_sertoli[5] = pair<unsigned int, unsigned int>(1, 8);
    expected_sertoli[6] = pair<unsigned int, unsigned int>(6, 2);
    expected_sertoli[7] = pair<unsigned int, unsigned int>(11, 1);
    expected_sertoli[8] = pair<unsigned int, unsigned int>(18, 4);

    for (int i = 0; i < global.SERTOLI_NUMBER; i++)
    {
        if (testSim.sertoli[i] != expected_sertoli[i])
        {
            output << "testConstructor Failure: Sertoli Nuclei being placed at location (";
            output << testSim.sertoli[i].first << "," << testSim.sertoli[i].second << ")" << endl;
        }
    }

    return output.str();
}

string TestSim::testStep()
{
    stringstream output;

    //<---------------------------Test Step Specific Algorithm Parts------------------------------->
    // Test 1: PopulationSize Maintenance
    output << this->subStep_PopulationSize();

    // Test 2: Lumen Area Handling
    output << this->subStep_LumenArea();

    // Test 3: Movement Actions
    output << this->subStep_Movement();

    // Test 4: Proliferation Actions
    output << this->subStep_Proliferation();

    // Test 5: Removal Actions
    output << this->subStep_Removal();

    // Test 6: Simultaneous Actions
    output << this->subStep_Simultaneous();
    //<-------------------------------------------------------------------------------------------->

    //<---------------------------------Test Step Set Cases---------------------------------------->
    // Test 1: Empty Population
    output << this->subStep_Empty();

    // Test 2: Emptied Population (initially populated)
    output << this->subStep_Emptied();

    // Test 3: Growing Population
    output << this->subStep_Growing();

    // Test 4: Stable Population
    output << this->subStep_Stable();
    //<-------------------------------------------------------------------------------------------->

    return output.str();
}

string TestSim::testPopulate()
{
    stringstream output;

    //<--------------------------------------Test Populate----------------------------------------->
    // Note: These test cases are constructed for default #define parameter values, if these values
    //       are changed, this test may be invalidated
    // The following number of spaces should be availiable per the denoted cell type after
    // accounting for sertoli cells
    //      Undifferentiated Spermatogonia  - 60 -> 53 accounting for sertoli
    //      Differentiated Spermatogonia    - 60 -> 53
    //      Preleptotene                    - 52 -> 46
    //      Leptotene                       - 64 -> 57
    //      Zygotene                        - 76 -> 67
    //      Pachytene                       - 132
    //      Diplotene                       - 148
    //      Second Spermatocyte             - 172
    //      Round Spermatids                - 180
    //      Elongated Spermatids            - 92
    // Total spaces availiable = 299
    //
    // 1) Check perfect population of each type
    // 2) Check 26 / 27 population of stem cells
    // 3) Check 27 / 26 population of stem cells
    // 4) Check 26 / 26 population of stem cells
    // 3) Check underpopulation
    // 4) Check overpopulation
    //<-------------------------------------------------------------------------------------------->
    unsigned int typeArray[10];
    Simulator testSim;

    // Check perfect population of each type
    unsigned int perfect[10];
    perfect[0] = 53;
    perfect[1] = 53;
    perfect[2] = 46;
    perfect[3] = 57;
    perfect[4] = 67;
    perfect[5] = 125;
    perfect[6] = 141;
    perfect[7] = 165;
    perfect[8] = 173;
    perfect[9] = 85;
    for (int i = 0; i < 10; i++)
    {
        for (int n = 0; n < 10; n++)
            typeArray[n] = 0;
        typeArray[i] = perfect[i];
        if (!testSim.populate(typeArray))
        {
            output << "testPopulate Failure: Perfect population of cell type ";
            output << toString(static_cast<CellType>(i)) << " could not be populated." << endl;
        }

        multimap<unsigned long long, Cell*>::iterator it;
        for (it = testSim.population.begin(); it != testSim.population.end(); it++)
            delete it->second;
        testSim.population.clear();
    }

    // Check 26 / 27 population of stem cells
    for (int n = 0; n < 10; n++)
        typeArray[n] = 0;
    typeArray[0] = 26;
    typeArray[1] = 27;
    if (!testSim.populate(typeArray))
    {
        output << "testPopulate Failure: 26/27 population of undiff/diff spermatogonia could not";
        output << " be populated" << endl;
    }
    multimap<unsigned long long, Cell*>::iterator it;
    for (it = testSim.population.begin(); it != testSim.population.end(); it++)
        delete it->second;
    testSim.population.clear();

    // Check 27 / 26 population of stem cells
    for (int n = 0; n < 10; n++)
        typeArray[n] = 0;
    typeArray[0] = 27;
    typeArray[1] = 26;
    if (!testSim.populate(typeArray))
    {
        output << "testPopulate Failure: 27/26 population of undiff/diff spermatogonia could not";
        output << " be populated" << endl;
    }
    for (it = testSim.population.begin(); it != testSim.population.end(); it++)
        delete it->second;
    testSim.population.clear();

    // Check 26 / 26 population of stem cells
    for (int n = 0; n < 10; n++)
        typeArray[n] = 0;
    typeArray[0] = 26;
    typeArray[1] = 26;
    if (!testSim.populate(typeArray))
    {
        output << "testPopulate Failure: 26/26 population of undiff/diff spermatogonia could not";
        output << " be populated" << endl;
    }
    for (it = testSim.population.begin(); it != testSim.population.end(); it++)
        delete it->second;
    testSim.population.clear();

    // Check underpopulation
    for (int n = 0; n < 10; n++)
        typeArray[n] = 10;
    if (!testSim.populate(typeArray))
    {
        output << "testPopulate Failure: underpopulation of all cell types could not";
        output << " be populated" << endl;
    }
    for (it = testSim.population.begin(); it != testSim.population.end(); it++)
        delete it->second;
    testSim.population.clear();

    // Check overrpopulation
    for (int n = 0; n < 10; n++)
        typeArray[n] = 30;
    if (testSim.populate(typeArray))
    {
        output << "testPopulate Failure: overpopulation of all cell types could";
        output << " be populated" << endl;
    }
    for (it = testSim.population.begin(); it != testSim.population.end(); it++)
        delete it->second;
    testSim.population.clear();

    return output.str();
}

string TestSim::testPopulate_Type()
{
    stringstream output;

    //<------------------------------------Test Populate Type-------------------------------------->
    // 1) Construct a vector of positions of varied availiablity
    // 2) Attempt to populate the entire vector with normal (not small cells). Determine if the
    //    correct positions are populated
    // 3) Attempt to populate the entire vector with small cells. Determine if the correct positions
    //    are populated.
    // 4) Attempt to populate less than maximum amount of cells with space availiable for normal
    //    cells in the vector. Determine if the full count is populated.
    //<-------------------------------------------------------------------------------------------->
    unsigned long long hash = 0;

    // Construct a vector of positions of varied availiablity
    Simulator testSim_norm;
    vector<pair<unsigned int, unsigned int> >spots;
    for (int i = 0; i < 10; i++)
        spots.push_back(pair<unsigned int, unsigned int>(i, global.ENVIRONMENT_RADIUS));
    for (int i = 0; i < global.SERTOLI_NUMBER; i++)
        testSim_norm.sertoli[i] = pair<unsigned int, unsigned int>(0,0);
    // Make spots (3, 11*), (4, 11*) unavailiable
    for (int i = 3; i < 5; i++)
        testSim_norm.sertoli[i] = pair<unsigned int, unsigned int>(i, global.ENVIRONMENT_RADIUS);

    // Attempt to populate fully with normal cells
    vector<pair<unsigned int, unsigned int> >tmp_spots = spots;
    if (testSim_norm.populate_type(10, UNDIFFERENTIATED_SPERMATOGONIA, &tmp_spots) ||
            testSim_norm.populationSize[0] > 8)
    {
        output << "testPopulate_Type Failure: On normal cell population, simulator is able to ";
        output << "populate more cells then there is space for" << endl;
    }
    for (int i = 0; i < 10; i++)
    {
        hash = testSim_norm.h(i, global.ENVIRONMENT_RADIUS);
        if (i != 3 && i != 4 && testSim_norm.population.count(hash) == 0)
        {
            output << "testPopulate_Type Failure: On normal cell population, position (" << i;
            output << "," << global.ENVIRONMENT_RADIUS;
            output << ") was not populated when space was availiable ";
            output << "and overpopulation was requested" << endl;
        }
        if (testSim_norm.population.count(hash) > 1)
        {
            output << "testPopulate_Type Failure: On normal cell population, position (" << i;
            output << "," << global.ENVIRONMENT_RADIUS;
            output << ") was populated with more than one cell\n";
        }
        if ((i == 3 || i == 4) && testSim_norm.population.count(hash) != 0)
        {
            output << "testPopulate_Type Failure: On normal cell population, position (" << i;
            output << "," << global.ENVIRONMENT_RADIUS << ") was populated when a sertoli nuclei was ";
            output << "present in that location" << endl;
        }
    }

    // Attempt to populate fully with small cells
    Simulator testSim_small;
    for (int i = 0; i < global.SERTOLI_NUMBER; i++)
        testSim_small.sertoli[i] = testSim_norm.sertoli[i];

    tmp_spots = spots;
    if (testSim_small.populate_type(20,ELONGATED_SPERMATIDS,&tmp_spots) ||
        testSim_small.populationSize[9] > 16)
    {
        output << "testPopulate_Type Failure: On small cell population, simulator is able to ";
        output << "populate more cells then there is space for" << endl;
    }
    for (int i = 0 ; i < 10; i++)
    {
        hash = testSim_small.h(i, global.ENVIRONMENT_RADIUS);
        if (i != 3 && i != 4 && testSim_small.population.count(hash) < 2)
        {
            output << "testPopulate_Type Failure: On small cell population, position (" << i;
            output << "," << global.ENVIRONMENT_RADIUS;
            output << ") was not fully populated when space was ";
            output << "availiable and overpopulation was requested" << endl;
        }
        if (testSim_small.population.count(hash) > 2)
        {
            output << "testPopulate_Type Failure: On small cell population, position (" << i;
            output << "," << global.ENVIRONMENT_RADIUS;
            output << ") was populated with more than two cells\n";
        }
        if ((i == 3 || i == 4) && testSim_small.population.count(hash) != 0)
        {
            output << "testPopulate_Type Failure: On small cell population, position (" << i;
            output << "," << global.ENVIRONMENT_RADIUS;
            output << ") was populated when a sertoli nuclei was ";
            output << "present in that location" << endl;
        }
    }

    // Attempt to populate less normal cells than there is space availiable
    Simulator testSim_under;
    for (int i = 0; i < global.SERTOLI_NUMBER; i++)
        testSim_under.sertoli[i] = testSim_norm.sertoli[i];

    tmp_spots = spots;
    if (!testSim_under.populate_type(7, UNDIFFERENTIATED_SPERMATOGONIA, &tmp_spots))
    {
        output << "testPopulate_Type Failure: On normal cell population, simulator is not able to ";
        output << "populate less cells then there is space for" << endl;
    }

    return output.str();
}

string TestSim::testMove()
{
    stringstream output;

    //<-----------------------------------------Test Move------------------------------------------>
    // Case 1: Valid Movement
    //      1) Create a location pair in a location inside the environment circle and not next to a
    //         boundry
    //      2) Determine if the move() function returns as expected
    //
    // Case 2: Invalid Movement
    //      1) For each Direction:
    //          1) Pick a location such that moving the denoted direction would place the cell
    //             outside the environment circle.
    //          2) Determine if the move function returns the passed coordinates (i.e. does nothing)
    //<-------------------------------------------------------------------------------------------->
    Simulator testSim;

    // Create a location pair away from a boundry
    pair<unsigned int, unsigned int> location(2, global.ENVIRONMENT_RADIUS);

    // Determine if the move() function returns as expected
    pair<unsigned int, unsigned int> testLocation[8];
    testLocation[0] = pair<unsigned int, unsigned int>(2, global.ENVIRONMENT_RADIUS + 1);
    testLocation[1] = pair<unsigned int, unsigned int>(3, global.ENVIRONMENT_RADIUS + 1);
    testLocation[2] = pair<unsigned int, unsigned int>(3, global.ENVIRONMENT_RADIUS);
    testLocation[3] = pair<unsigned int, unsigned int>(3, global.ENVIRONMENT_RADIUS - 1);
    testLocation[4] = pair<unsigned int, unsigned int>(2, global.ENVIRONMENT_RADIUS - 1);
    testLocation[5] = pair<unsigned int, unsigned int>(1, global.ENVIRONMENT_RADIUS - 1);
    testLocation[6] = pair<unsigned int, unsigned int>(1, global.ENVIRONMENT_RADIUS);
    testLocation[7] = pair<unsigned int, unsigned int>(1, global.ENVIRONMENT_RADIUS + 1);

    Direction dir = UP;

    for (int i = 0; i < 8; i++)
    {
        if (testLocation[i] != testSim.move(dir, location))
        {
            output << "testMove Failure: Movement in " << toString(dir) << " direction from (2,";
            output << global.ENVIRONMENT_RADIUS << ") resulting in movement to position (";
            output << testSim.move(dir, location).first << ",";
            output << testSim.move(dir, location).second << ")" << endl;
        }
        dir++;
    }

    // Pick a set of locations such that moving in the denoted direction will cause an invalid
    // movement
    for (int i = 0; i < 8; i++)
    {
        if (i < 1 || i > 3)
            testLocation[i] = pair<unsigned int, unsigned int>(0, global.ENVIRONMENT_RADIUS);
        else
            testLocation[i] = pair<unsigned int, unsigned int>(2 * global.ENVIRONMENT_RADIUS,
                                                               global.ENVIRONMENT_RADIUS);
    }

    // Determine if the move function returns the passed coordinates (i.e. does nothing)
    for (int i = 0; i < 8; i++)
    {
        if (testLocation[i] != testSim.move(dir, testLocation[i]))
        {
            output << "testMove Failure: Movement in " << toString(dir) << " direction from (";
            output << testLocation[i].first << "," << testLocation[i].second << ") resulting in ";
            output << "movement to position (" << testSim.move(dir, testLocation[i]).first << ",";
            output << testSim.move(dir, testLocation[i]).second << ")" << endl;
        }
        dir++;
    }

    return output.str();
}

string TestSim::testFind_Forward()
{
    //<------------------------------------Test Find Forward--------------------------------------->
    // 1) Create an array of positions comprised of all valid squares around the outer edge of
    //    the environment
    // 2) Create an array, parallel to the first, comprised of the expected directions for
    //    those positions with respect to undifferentiated spermatogonia
    // 3) Create an array, parallel to the first, comprised of the expected directions for those
    //    those positions with respect to all cells but undifferentiated spermatogonia
    // 4) Perform find_forward on all locations using type undifferentiated spermatogonia. Determine
    //    if the results match the manually constructed undiff[] array.
    // 5) Perform find_forward on all locations using type elongated spermatids. Determine if the
    //    results match the manually construced other[] array.
    //<-------------------------------------------------------------------------------------------->
    stringstream output;
    Simulator testSim;

    // Create an array of positions comprised of all valid squares around the outer edge of the
    // environment - see transform_construction.jpeg in included documentation folder
    pair<unsigned int, unsigned int> outer[84];

    // Top Quadrent III points
    for (int i = 0; i < 4; i++)
        outer[i] = pair<unsigned int, unsigned int>(1, global.ENVIRONMENT_RADIUS - (i + 1));
    for (int i = 4; i < 7; i++)
        outer[i] = pair<unsigned int, unsigned int>(2, global.ENVIRONMENT_RADIUS - i);
    for (int i = 7; i < 9; i++)
        outer[i] = pair<unsigned int, unsigned int>(3, global.ENVIRONMENT_RADIUS - (i - 1));

    // Reflect across y=x to get full Quadrent III point set
    // T:f(x,y) -> f(y,x)
    for (int i = 9; i < 18; i++)
    {
        outer[i].first = outer[i-9].second;
        outer[i].second = outer[i-9].first;
    }

    // Odd middle point
    outer[18] = pair<unsigned int, unsigned int>(4, global.ENVIRONMENT_RADIUS - 7);

    // Reflect across y = -x + 2 * ENVIRONMENT_RADIUS to add Quadrent I
    // T:f(x,y) -> f(-y + 22, -x + 22) | ENVIRONMENT_RADIUS = 11
    for (int i = 19; i < 38; i++)
    {
        outer[i].first = global.ENVIRONMENT_RADIUS * 2 - outer[i-19].second;
        outer[i].second = global.ENVIRONMENT_RADIUS * 2 - outer[i-19].first;
    }

    // Reflect across y = 0 to add Quadrents IV and II
    // T:f(x,y) -> f(2 * ENVIRONMENT_RADIUS - x, y)
    for (int i = 38; i < 76; i++)
    {
            outer[i].first = 2 * global.ENVIRONMENT_RADIUS - outer[i - 38].first;
            outer[i].second = outer[i - 38].second;
    }

    // Add in the axis points
    for (int i = 76; i < 78; i++)
        outer[i] = pair<unsigned int, unsigned int>(i - 76, global.ENVIRONMENT_RADIUS);
    for (int i = 78; i < 80; i++)
        outer[i] = pair<unsigned int, unsigned int>(global.ENVIRONMENT_RADIUS,
                                                    global.ENVIRONMENT_RADIUS * 2 - (i - 78));
    for (int i = 80; i < 82; i++)
        outer[i] = pair<unsigned int, unsigned int>(global.ENVIRONMENT_RADIUS * 2 - (i - 80),
                                                    global.ENVIRONMENT_RADIUS);
    for (int i = 82; i < 84; i++)
        outer[i] = pair<unsigned int, unsigned int>(global.ENVIRONMENT_RADIUS, i - 82);

    // Create an array, parallel to the first, comprised of the expected directions for
    // those positions with respect to undifferentiated spermatogonia
    Direction undiff[84];

    // Refer to transform_construction.jpeg in the included documentation folder and the
    // distance formula for reference on determining forward directions.
    for (int i = 0; i < 6; i++)
        undiff[i] = DOWN_RIGHT;
    undiff[6] = RIGHT;
    for (int i = 7; i < 9; i++)
        undiff[i] = UP;
    for (int i = 9; i < 12; i++)
        undiff[i] = RIGHT;
    for (int i = 12; i < 19; i++)
        undiff[i] = UP_LEFT;
    for (int i = 19; i < 21; i++)
        undiff[i] = RIGHT;
    undiff[21] = UP;
    undiff[22] = LEFT;
    for (int i = 23; i < 28; i++)
        undiff[i] = UP_LEFT;
    for (int i = 28; i < 35; i++)
        undiff[i] = DOWN_RIGHT;
    for (int i = 35; i < 38; i++)
        undiff[i] = UP_LEFT;
    for (int i = 38; i < 46; i++)
        undiff[i] = UP_RIGHT;
    undiff[46] = DOWN_LEFT;
    undiff[47] = RIGHT;
    undiff[48] = UP;
    for (int i = 49; i < 51; i++)
        undiff[i] = LEFT;
    for (int i = 51; i < 57; i++)
        undiff[i] = DOWN_LEFT;
    for (int i = 57; i < 60; i++)
        undiff[i] = RIGHT;
    for (int i = 60; i < 66; i++)
        undiff[i] = DOWN_LEFT;
    for (int i = 66; i < 70; i++)
        undiff[i] = UP_RIGHT;
    for (int i = 70; i < 73; i++)
        undiff[i] = UP;
    undiff[73] = LEFT;
    for (int i = 74; i < 76; i++)
        undiff[i] = DOWN_LEFT;
    for (int i = 76; i < 78; i++)
        undiff[i] = UP_RIGHT;
    undiff[78] = DOWN_RIGHT;
    undiff[79] = RIGHT;
    undiff[80] = UP;
    undiff[81] = RIGHT;
    undiff[82] = UP_RIGHT;
    undiff[83] = RIGHT;

    // Create an array, parallel to the first, comprised of the expected directions for those
    // those positions with respect to all cells but undifferentiated spermatogonia
    Direction other[84];

    // Refer to transform_construction.jpeg in the included documentation folder and the
    // distance formula for reference on determining forward directions.
    for (int i = 82; i < 84; i++)
        other[i] = UP;
    for (int i = 76; i < 78; i++)
        other[i] = RIGHT;
    for (int i = 78; i < 80; i++)
        other[i] = DOWN;
    for (int i = 80; i < 82; i++)
        other[i] = LEFT;
    for (int i = 0; i < 19; i++)
        other[i] = UP_RIGHT;
    for (int i = 19; i < 38; i++)
        other[i] = DOWN_LEFT;
    for (int i = 38; i < 57; i++)
        other[i] = UP_LEFT;
    for (int i = 57; i < 76; i++)
        other[i] = DOWN_RIGHT;

    // Perform find_forward on all locations using type undifferentiated spermatogonia. Determine
    // if the results match the manually constructed undiff[] array.
    // Don't fail on cells that have blood vessels directly on top of them. These can go anywhere.
    for (int i = 0; i < 84; i++)
        if(!testSim.find_forward(outer[i], UNDIFFERENTIATED_SPERMATOGONIA) == undiff[i] &&
                i != 24 && i != 39 && i != 72 && i != 10 && i != 57)
        {
            output << "testSpace_Availiable Failure: Undifferentiated spermatogonia of index ";
            output << i << " at position: (";
            output << outer[i].first << "," << outer[i].second << ") declaring forward as ";
            output << toString(testSim.find_forward(outer[i], UNDIFFERENTIATED_SPERMATOGONIA));
            output << endl;
        }

    return output.str();
}

string TestSim::testSpace_Availiable()
{
    stringstream output;

    //<-----------------------------------Test Space Availiable------------------------------------>
    // Test 1: Sertoli
    //      1) Create a sertoli nuclei
    //      2) Determine if space is availiable at the sertoli nuclei's location for all cell types
    //
    // Test 2: Normal Cell
    //
    //      For all normal cell types:
    //      1) Populate a normal (i.e. not small) cell
    //      2) Determine if the cell's location is availiable for all cell types
    //
    // Test 3: One Small Cell
    //
    //      For all small cell types:
    //      1) Populate one small cell
    //      2) Determine if the cell's location is availiable for all normal cell types
    //      3) Determine if the cell's location is availiable for all small cell types
    //
    // Test 4: Two Small Cells
    //
    //      For each of {2 elongated, 2 round, 1 elongated & 1 round}:
    //      1) Populate two small cells in the same location
    //      2) Determine if the cell's location is availiable for all cell types
    //
    // Test 5: Open Space
    //
    //      For each cell type:
    //      1) Determine if open space returns space available for the cell
    //<-------------------------------------------------------------------------------------------->

    Simulator testSim;
    pair <unsigned int, unsigned int> location (0, global.ENVIRONMENT_RADIUS);
    unsigned long long hash = testSim.h(0, global.ENVIRONMENT_RADIUS);
    multimap<unsigned long long, Cell*>::iterator it;
    Cell* tmpCell;

    //------------------
    // Test 1: Sertoli
    //------------------
    // Create a sertoli nuclei
    testSim.sertoli[0] = pair<unsigned int, unsigned int>(location);

    // Determine if space is availiable at the sertoli nuclei's location for all cell types
    for (int i = 0 ; i < 10; i++)
    {
        if (testSim.space_availiable(testSim.sertoli[0], static_cast<CellType>(i)))
        {
            output << "testSpace_Availiable Failure: Sertoli nuclei location determined to be ";
            output << "availiable for type " << toString(static_cast<CellType>(i)) << endl;
        }
    }

    for (int i = 0; i < global.SERTOLI_NUMBER; i++)
        testSim.sertoli[i] = pair<unsigned int, unsigned int>(0,0);

    //---------------------
    // Test 2: Normal Cell
    //---------------------
    for (int i = 0 ; i < 8; i++)
    {
        // Populate the normal (i.e. not small) cell
        tmpCell = new Cell(static_cast<CellType>(i), location);
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));

        // For all cell types, determine if the cell's location is availiable
        for (int n = 0 ; n < 10; n++)
        {
            if (testSim.space_availiable(location, static_cast<CellType>(n)))
            {
                output << "testSpace_Availiable Failure: Space populated by cell type ";
                output << toString(static_cast<CellType>(i)) << " determined";
                output << " to be availiable for cell type " << toString(static_cast<CellType>(n));
                output << endl;
            }
        }
        delete tmpCell;
        testSim.population.clear();
    }

    //------------------------
    // Test 3: One Small Cell
    //------------------------
    for (int i = 8; i < 10; i++)
    {
        // Populate one small cell
        tmpCell = new Cell(static_cast<CellType>(i), location);
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));

        // Determine if the cell's location is availiable for all normal cell types
        for (int n = 0; n < 8; n++)
        {
            if (testSim.space_availiable(location, static_cast<CellType>(n)))
            {
                output << "testSpace_Availiable Failure: Space populated by one cell of type ";
                output << toString(static_cast<CellType>(i)) << " determined to be availiable for ";
                output << "cell type " << toString(static_cast<CellType>(i)) << endl;
            }
        }

        for (int n = 8; n < 10; n++)
        {
            // Determine if the cell's location is availiable for all small cell types
            if (!testSim.space_availiable(location, static_cast<CellType>(n)))
            {
                output << "testSpace_Availiable Failure: Space populated by one cell of type ";
                output << toString(static_cast<CellType>(i)) << " determined to be unavailiable ";
                output << "for cell type " << toString(static_cast<CellType>(n)) << endl;
            }
        }
        delete tmpCell;
        testSim.population.clear();
    }

    //-------------------------
    // Test 4: Two Small Cells
    //-------------------------
    Cell* cellArr[6];

    // 2 elongated
    cellArr[0] = new Cell(ELONGATED_SPERMATIDS, location);
    cellArr[1] = new Cell(ELONGATED_SPERMATIDS, location);

    // 2 round
    cellArr[2] = new Cell(ROUND_SPERMATIDS, location);
    cellArr[3] = new Cell(ROUND_SPERMATIDS, location);

    // 1 elongated & 1 round
    cellArr[4] = new Cell(ELONGATED_SPERMATIDS, location);
    cellArr[5] = new Cell(ROUND_SPERMATIDS, location);


    for (int i = 0; i < 3; i++)
    {
        // Populate two small cells in the same location
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, cellArr[2*i]));
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, cellArr[2*i+1]));

        // Determine if the cell's location is availiable for all cell types
        for (int n = 0; n < 10; n++)
        {
            if (testSim.space_availiable(location, static_cast<CellType>(n)))
            {
                output << "testSpace_Availiable Failure: Space populated by ";
                output << toString(cellArr[2*i]->type) << " and " << toString(cellArr[2*i+1]->type);
                output << " determined to have space available for ";
                output << toString(static_cast<CellType>(n)) << endl;
            }
        }
        for (it = testSim.population.begin(); it != testSim.population.end(); it++)
            delete it->second;
        testSim.population.clear();
    }

    //--------------------
    // Test 5: Open Space
    //--------------------
    for (int i = 0; i < 10; i++)
    {
        //Determine if open space returns space available for the cell
        if (!testSim.space_availiable(location,static_cast<CellType>(i)))
        {
            output << "testSpace_Available Failure: Open space determined to be unavailable for ";
            output << "cell type: " << toString(static_cast<CellType>(i)) << endl;
        }
    }

    return output.str();
}


// PRIVATE METHODS

string TestSim::subStep_PopulationSize()
{
    stringstream output;

    //<---------------------------------PopulationSize Maintenence--------------------------------->
    // 1) Create a population with custom differentiation timers.
    // 2) Run the simulation until each cell in the population differentiates.
    //    --> Only allow the cells to differentiate one time.
    // 3) Determine if the population counters are as expected.
    // 4) Randomize the population by running cell steps
    // 5) Remove all members of the population by assigning their apoptosis timers to 1 and running
    //    a cell step
    // 6) Determine if the population counters are all zero
    //<-------------------------------------------------------------------------------------------->

    int smallestDiff = global.DIFFERENTIATION_MIN[0];
    for (int i = 1; i < 9; i++)
        if (global.DIFFERENTIATION_MIN[i] < smallestDiff)
            smallestDiff = global.DIFFERENTIATION_MIN[i];

    // Create a population with custom differentiation timers
    int diffArr[20];
    for (int i = 0; i < 20; i++)
        // Assign a differentiation between 0 and the smallest #defined differentiation exclusively
        diffArr[i] = rand() % (smallestDiff - 1) + 1;

    Cell* cellArr[20];
    for (int i = 0; i < 5; i++)
    {
        // !! Assumes Environment Radius - Tubule Radius >= 5 !!
        cellArr[i] = new Cell(static_cast<CellType>(rand() % 10),
                              pair<unsigned int, unsigned int>(i, 0));
        cellArr[i + 5] = new Cell(static_cast<CellType>(rand() % 10),
                                  pair<unsigned int, unsigned int>(0, i));
        cellArr[i + 10] = new Cell(static_cast<CellType>(rand() % 10),
                                   pair<unsigned int, unsigned int>(22 - i, 0));
        cellArr[i + 15] = new Cell(static_cast<CellType>(rand() % 10),
                                   pair<unsigned int, unsigned int>(0, 22 - i));
    }

    Simulator testSim;

    for (int i = 0; i < 10; i++)
        testSim.populationSize[i] = 0;

    for (int i = 0; i < 20; i++)
    {
        cellArr[i]->differentiation = diffArr[i];
        testSim.populationSize[cellArr[i]->type]++;
        unsigned long long hash = testSim.h(cellArr[i]->position.first, cellArr[i]->position.second);
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, cellArr[i]));
    }

    double initialSize[10];
    for (int i = 0; i < 10; i++)
        initialSize[i] = testSim.populationSize[i];

    // Run the simulation until each cell in the population differentiates
    for (int i = 0; i < smallestDiff; i++)
        testSim.step();

    // Determine if the population counters are as expected.
    bool test_failure = false;
    if (testSim.populationSize[0] != 0)
        test_failure = true;
    for (int i = 0; i < 8; i++)
        if (testSim.populationSize[i+1] != initialSize[i])
            test_failure = true;
    if (testSim.populationSize[9] != initialSize[8] + initialSize[9])
        test_failure = true;

    if (test_failure)
        output << "testStep Failure: PopulationSize Maintenence differentiation testing failed\n";

    // Randomize the population
    for (int i = 0; i < 500; i++)
    {
        testSim.step();
    }

    // Remove all members of the population

    // Assign each member an apoptosis of 1
    multimap<unsigned long long, Cell*>::iterator it;
    for (it = testSim.population.begin(); it != testSim.population.end(); it++)
        it->second->apoptosis = 1;

    // Run a cell step
    testSim.step();

    for (int i = 0; i < 10; i++)
        if (testSim.populationSize[i] != 0)
        {
            output << "testStep Failure: PopulationSize Maintence removal testing failed. ";
            output << toString(static_cast<CellType>(i)) << " population size listed as ";
            output << testSim.populationSize[i] << "when simulator is empty." << endl;
        }
    if (!testSim.population.empty())
    {
        output << "testStep Failure: Simulator population container is not being emptied correctly";
        output << endl;
    }

    for (int i = 0; i < 20; i++)
        delete cellArr[i];
    return output.str();
}

string TestSim::subStep_LumenArea()
{
    stringstream output;

    //<---------------------------------------Lumen Area Handling---------------------------------->
    // 1) Elongated spermatid entering the lumen area:
    //       1) Create an elongated spermatid outside the lumen area.
    //       2) Move the cell in towards the center and manually check its coordinates against the
    //          lumen area.
    //       3) When the cell enters the lumen area determine if the cell disappears.
    // 2) Other cell entering the lumen area:
    //       1) Create a cell of type other than elongated spermatid outside the lumen area
    //       2) Move the cell in towards the center and manually check its coordinates against the
    //          lumen area.
    //       3) When the cell enters the lumen area determine if the correct flag is fired.
    //<-------------------------------------------------------------------------------------------->

    // ---------------------------------------------
    // Elongated spermatid entering the lumen area
    // ---------------------------------------------

    // Create an elongated spermatid outside the lumen area
    Cell* tmpCell = new Cell(ELONGATED_SPERMATIDS, pair<unsigned int, unsigned int> (
                                 global.TUBULE_RADIUS + global.ENVIRONMENT_RADIUS + 1,
                                 global.TUBULE_RADIUS + global.ENVIRONMENT_RADIUS + 1));
    tmpCell->apoptosis = 1005;
    tmpCell->move = 1;
    Simulator testSim;
    unsigned long long hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
    testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));

    // Move the cell in towards the center and manually check its coordinates against the lumen
    // area.
    pair<unsigned int, unsigned int> center(global.ENVIRONMENT_RADIUS, global.ENVIRONMENT_RADIUS);
    pair<unsigned int, unsigned int> location (0, 0);
    int counter = 0;
    while (counter < 1000 && !testSim.population.empty())
    {
        location = testSim.population.begin()->second->position;
        testSim.step();

        if (testSim.distance(location, center) < global.TUBULE_RADIUS &&
                !testSim.population.empty())
        {
            output << "testStep Failure: Elongated Spermatid not removed from cell population upon";
            output << " entering the lumen area" << endl;
        }

        counter++;
    }
    if (counter >= 1000)
        output << "testStep Failure: Elongated Spermatid did not move into the lumen area" << endl;

    // ------------------------------------
    // Other cell entering the lumen area
    // ------------------------------------

    // Create a cell of type other than elongated spermatid outside the lumen area
    tmpCell = new Cell(PRELEPTOTENE, pair<unsigned int, unsigned int> (
                                 global.TUBULE_RADIUS + global.ENVIRONMENT_RADIUS + 1,
                                 global.TUBULE_RADIUS + global.ENVIRONMENT_RADIUS + 1));
    tmpCell->apoptosis = 505;
    tmpCell->move = 1;
    hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
    testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));

    // Move the cell in towards the center and manually check its coordinates against the lumen
    // area.
    counter = 0;
    while (counter < 500 && !testSim.population.empty() && testSim.flags.empty())
    {
        location = testSim.population.begin()->second->position;
        testSim.step();

        if (testSim.distance(location, center) < global.TUBULE_RADIUS && testSim.flags.empty())
        {
            output << "testStep Failure: Flag was not set upon a cell type other than elongated ";
            output << "spermatid entering the lumen area" << endl;
        }

        counter++;
    }
    if (counter >= 500)
    {
        output << "testStep Failure: Non elongated spermatid cell type did not move into the ";
        output << "lumen area" << endl;
    }

    return output.str();
}

string TestSim::subStep_Movement()
{
    stringstream output;

    //<--------------------------------------Movement Actions-------------------------------------->
    // For each of the small cell cases (see below), movement into a square with another cell will
    // be tested. Following this, movement out of the square containing two cells will be tested.
    // This implicitly tests basic movement functionality as well as the small cell special test
    // cases.
    //
    // 1) Small Cell Cases:
    //       1) Case 1: Two Round Spermatids
    //               1) Create two round spermatids. One with a movement timer of one and one with a
    //                  default movement timer. Place the cell with the default movement timer
    //                  adjacent and to the center of the other.
    //               2) Block all squares surrounding the two cells with Sertoli nuclei
    //               3) Run simulation steps until the cell moves or an upper bound is met (test
    //                  failure). Do not let either cell perform any cell behaviors besides
    //                  movement. Ensure the upper bound is less than the minimum default movement
    //                  time.
    //               4) Determine if both cells are present in the default movement timer cell's
    //                  location
    //               5) Reset both cell's movement timers to their initial settings.
    //               6) Remove the adjacent Sertoli nucleus to the center of the cells. Place a
    //                  Sertoli nucleus behind (away from the center) of the cells.
    //               6) Run simulation steps as before until the cell moves or an upper bound is met
    //                  (test failure).
    //               7) Determine if the moving cell is in the location of the removed Sertoli cell
    //                  and the non-moving cell is in it's original location.
    //
    //       2) Case 2: Two Elongated Spermatids
    //               Perform the same algorithm as Case 1...
    //
    //       3) Case 3: One Round Spermatid and One Elongated Spermatid
    //               Perform the same algorithm as Case 1...
    //<-------------------------------------------------------------------------------------------->

    // Create arrays to run each of the test cases
    Cell* first_cell[3];
    Cell* second_cell[3];

    // Create two cells (1)
    pair<unsigned int, unsigned int> first_location(0, global.ENVIRONMENT_RADIUS);
    pair<unsigned int, unsigned int> second_location(1, global.ENVIRONMENT_RADIUS);

    // Case 1 cells
    first_cell[0] = new Cell(ROUND_SPERMATIDS, first_location);
    second_cell[0] = new Cell(ROUND_SPERMATIDS, second_location);

    // Case 2 cells
    first_cell[1] = new Cell(ELONGATED_SPERMATIDS, first_location);
    second_cell[1] = new Cell(ELONGATED_SPERMATIDS, second_location);

    // Case 3 cells
    first_cell[2] = new Cell(ROUND_SPERMATIDS, first_location);
    second_cell[2] = new Cell(ELONGATED_SPERMATIDS, second_location);

    // Block all squares surrounding the two cells with Sertoli nuclei
    pair<unsigned int, unsigned int> sertoli[9];
    sertoli[0] = pair<unsigned int, unsigned int>(1, global.ENVIRONMENT_RADIUS + 1);
    sertoli[1] = pair<unsigned int, unsigned int>(2, global.ENVIRONMENT_RADIUS + 1);
    sertoli[2] = pair<unsigned int, unsigned int>(2, global.ENVIRONMENT_RADIUS);
    sertoli[3] = pair<unsigned int, unsigned int>(2, global.ENVIRONMENT_RADIUS - 1);
    sertoli[4] = pair<unsigned int, unsigned int>(1, global.ENVIRONMENT_RADIUS - 1);
    for (int i = 5; i < global.SERTOLI_NUMBER; i++)
        sertoli[i] = pair<unsigned int, unsigned int>(0, 0);

    Simulator testSim;

    // Run simulation steps...
    int bound[3];
    // Set the upper bound
    // Round Spermatids speed min
    bound[0] = global.SPEED_MIN[8];
    // Elongated Spermatids speed min
    bound[1] = global.SPEED_MIN[9];
    (bound[0] < bound[1])? bound[2] = bound[0] : bound[2] = bound[1];

    for (int i = 0; i < 3; i++)
    {
        for (int n = 0; n < global.SERTOLI_NUMBER; n++)
            testSim.sertoli[n] = sertoli[n];

        first_cell[i]->apoptosis = bound[i] + 10;
        first_cell[i]->differentiation = bound[i] + 10;
        first_cell[i]->proliferation = bound[i] + 10;
        first_cell[i]->move = 1;
        second_cell[i]->apoptosis = bound[i] + 10;
        second_cell[i]->differentiation = bound[i] + 10;
        second_cell[i]->proliferation = bound[i] + 10;

        unsigned long long first_hash;
        unsigned long long second_hash;

        second_hash = testSim.h(second_cell[i]->position.first, second_cell[i]->position.second);
        testSim.population.insert(pair<unsigned long long, Cell*>(second_hash, second_cell[i]));

        first_hash = testSim.h(first_cell[i]->position.first, first_cell[i]->position.second);
        testSim.population.insert(pair<unsigned long long, Cell*>(first_hash, first_cell[i]));

        int n;
        for (n = 0; n < bound[i]; n++)
        {
            testSim.step();
            if (testSim.population.count(first_hash) == 0)
                // The cell has moved
                break;
        }
        if (n == bound[i])
        {
            output << "testStep Failure: " << toString(first_cell[i]->type);
            output << " does not move onto " << toString(second_cell[i]->type) << endl;
        }
        else if (testSim.population.count(second_hash) != 2)
        {
            output << "testStep Failure: Testing Environment Failure - ";
            output << toString(first_cell[i]->type) << " has moved a location other than the cell ";
            output << "of type " << toString(second_cell[i]->type) << endl;
        }
        multimap<unsigned long long, Cell*>::iterator it;
        for (it = testSim.population.begin(); it != testSim.population.end(); it++)
            delete it->second;
        testSim.population.clear();
    }

    return output.str();
}

string TestSim::subStep_Proliferation()
{
    stringstream output;

    //<--------------------------------------Proliferation Actions--------------------------------->
    // 1) Create a large (not small) cell. Assign its proliferation timer to 1.
    // 2) Surround the cell with Sertoli nuclei except for one square
    // 3) Run a cell step
    // 4) Determine if a new cell has been created in the open adjacent square to the original cell.
    // 5) Determine if the original cell has moved
    //<-------------------------------------------------------------------------------------------->

    // Create a large cell with a proliferation of 1
    Cell* tmpCell = new Cell(UNDIFFERENTIATED_SPERMATOGONIA,
                             pair<unsigned int, unsigned int>(0, global.ENVIRONMENT_RADIUS));
    tmpCell->proliferation = 1;

    // Surround the cell with Sertoli nuclei except for the one square
    pair<unsigned int, unsigned int> sertoli[9];
    sertoli[0] = pair<unsigned int, unsigned int>(1, global.ENVIRONMENT_RADIUS + 1);
    sertoli[1] = pair<unsigned int, unsigned int>(2, global.ENVIRONMENT_RADIUS + 1);
    sertoli[2] = pair<unsigned int, unsigned int>(2, global.ENVIRONMENT_RADIUS);
    sertoli[3] = pair<unsigned int, unsigned int>(2, global.ENVIRONMENT_RADIUS - 1);
    sertoli[4] = pair<unsigned int, unsigned int>(1, global.ENVIRONMENT_RADIUS - 1);
    for (int i = 5; i < global.SERTOLI_NUMBER; i++)
        sertoli[i] = pair<unsigned int, unsigned int>(0, 0);

    Simulator testSim;
    unsigned long long hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
    testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
    for (int i = 0; i < global.SERTOLI_NUMBER; i++)
        testSim.sertoli[i] = sertoli[i];

    // Run a cell step
    testSim.step();

    // Determine if a new cell has been created in the open adjacent square to the original cell
    hash = testSim.h(1, global.ENVIRONMENT_RADIUS);
    if (testSim.population.find(hash) == testSim.population.end())
        output << "testStep Failure: Could not proliferate a cell" << endl;

    // Determine if the original cell has moved
    hash = testSim.h(0, global.ENVIRONMENT_RADIUS);
    if (testSim.population.find(hash) == testSim.population.end())
        output << "testStep Failure: Original cell moved on proliferation" << endl;

    if (testSim.population.find(hash)->second != tmpCell)
        output << "testStep Failure: Original cell memory address changed on proliferation" << endl;

    if (tmpCell->position.first != 0 || tmpCell->position.second != global.ENVIRONMENT_RADIUS)
        output << "testStep Failure: Original cell position changed during proliferation" << endl;

    return output.str();
}

string TestSim::subStep_Removal()
{
    stringstream output;

    //<-------------------------------------Removal Actions---------------------------------------->
    // 1) Case 1: Large Sudo-Randomized Set:
    //       1) Genereate a set of cells with death timers of 1
    //       2) Run a simulation step
    //       3) Determine if all cells have been removed.
    //
    // 2) Case 2: Small Cell Cases:
    //       1) For each of the small test cases listed under subStep_Movement():
    //               1) Create the two cells in the same location. Assign one cell a death timer of
    //                  1 and the other a default death timer.
    //               2) Run a cell step
    //               3) Determine if the expected cell was removed.
    //<-------------------------------------------------------------------------------------------->

    // ----------------------
    // Large Randomized Set
    // ----------------------

    // Generate a set of cells with death timers of 1
    Cell* tmpCell;
    unsigned long long hash;
    Simulator testSim;

    for (int i = 0; i < 25; i++)
    {
        tmpCell = new Cell(static_cast<CellType>(i % 10), pair<unsigned int, unsigned int>(
                               i, i + global.ENVIRONMENT_RADIUS));
        tmpCell->apoptosis = 1;
        hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
    }

    // Run a simulation step
    testSim.step();

    // Determine if all cells have been removed
    if (!testSim.population.empty())
        output << "testStep Failure: Cell apoptosis not occuring as expected";

    multimap<unsigned long long, Cell*>::iterator it;
    for (it = testSim.population.begin(); it != testSim.population.end(); it++)
        delete it->second;
    testSim.population.clear();

    // ------------------
    // Small Cell Cases
    // ------------------

    // Create arrays to run each of the test cases
    Cell* first_cell[3];
    Cell* second_cell[3];

    // Create the two cells in the same location.
    pair<unsigned int, unsigned int> location(0, global.ENVIRONMENT_RADIUS);

    // Case 1 cells
    first_cell[0] = new Cell(ROUND_SPERMATIDS, location);
    second_cell[0] = new Cell(ROUND_SPERMATIDS, location);

    // Case 2 cells
    first_cell[1] = new Cell(ELONGATED_SPERMATIDS, location);
    second_cell[1] = new Cell(ELONGATED_SPERMATIDS, location);

    // Case 3 cells
    first_cell[2] = new Cell(ROUND_SPERMATIDS, location);
    second_cell[2] = new Cell(ELONGATED_SPERMATIDS, location);

    hash = testSim.h(0, global.ENVIRONMENT_RADIUS);

    for (int i = 0; i < 3; i++)
    {
        // Assign one cell an apoptosis timer of 1
        first_cell[i]->apoptosis = 1;

        testSim.population.insert(pair<unsigned long long, Cell*>(hash, second_cell[i]));
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, first_cell[i]));

        // Run a cell step
        testSim.step();

        // Determine if the expected cell was removed.
        if (testSim.population.size() != 1 ||
                testSim.population.find(hash)->second != second_cell[i])
        {
            output << "testStep Failure: The correct cell of two small cells in the same square ";
            output << "was not removed on death" << endl;
        }
        multimap<unsigned long long, Cell*>::iterator it;
        for (it = testSim.population.begin(); it != testSim.population.end(); it++)
            delete it->second;
        testSim.population.clear();
    }

    return output.str();
}

string TestSim::subStep_Simultaneous()
{
    //<---------------------------Simultaneous Cell Behavior Actions------------------------------->
    // Case 1: Apoptosis & Proliferation
    // Case 2: Apoptosis & Movement
    // Case 3: Apoptosis & Differentiation
    // Case 4: Proliferation & Movement
    // Case 5: Proliferation & Differentiation
    // Case 6: Movement & Differentiation
    //
    // For each of the cases:
    // 1) Create a cell with the specified cell behaviors assigned to 1
    // 2) Populate the Simulator with this cell
    // 3) Run one simulator step
    // 4) Determine if the specified cell behaviors occured
    //<-------------------------------------------------------------------------------------------->
    stringstream output;

    Simulator testSim;
    Cell* tmpCell;
    CellType type = DIFFERENTIATED_SPERMATOGONIA;
    pair<unsigned int, unsigned int> location(0, global.ENVIRONMENT_RADIUS);
    unsigned long long hash = testSim.h(location.first, location.second);
    multimap<unsigned long long, Cell*>::iterator it;

    //------------------------------------
    // Case 1: Apoptosis & Proliferation
    //------------------------------------

    // Create the cell
    tmpCell = new Cell(type, location);
    tmpCell->apoptosis = 1;
    tmpCell->proliferation = 1;

    // Populate the Simulator
    testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));

    // Run one step
    testSim.step();

    // Determine outcome - in this case, the cell should be gone but its daughter should remain
    if (testSim.population.size() != 1 || testSim.population.begin()->second == tmpCell)
        output << "testStep Failure: Simultaneous apoptosis and proliferation failed" << endl;

    for (it = testSim.population.begin(); it != testSim.population.end(); it++)
        delete it->second;
    testSim.population.clear();

    //------------------------------------
    // Case 2: Apoptosis & Movement
    //------------------------------------
    tmpCell = new Cell(type, location);
    tmpCell->apoptosis = 1;
    tmpCell->move = 1;

    testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
    testSim.step();

    if (testSim.population.size() != 0)
        output << "testStep Failure: Simultaneous apoptosis and movement failed" << endl;

    for (it = testSim.population.begin(); it != testSim.population.end(); it++)
        delete it->second;
    testSim.population.clear();

    //------------------------------------
    // Case 3: Apoptosis & Differentiation
    //------------------------------------
    tmpCell = new Cell(type, location);
    tmpCell->apoptosis = 1;
    tmpCell->differentiation = 1;

    testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
    testSim.step();

    if (testSim.population.size() != 0)
        output << "testStep Failure: Simultaneous apoptosis and differentiation failed" << endl;

    for (it = testSim.population.begin(); it != testSim.population.end(); it++)
        delete it->second;
    testSim.population.clear();

    //------------------------------------
    // Case 4: Proliferation & Movement
    //------------------------------------
    tmpCell = new Cell(type, location);
    tmpCell->proliferation = 1;
    tmpCell->move = 1;

    testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
    testSim.step();

    it = testSim.population.begin();
    it++;
    if (testSim.population.size() != 2)
        output << "testStep Failure: Simultaneous proliferation and movement failed" << endl;

    for (it = testSim.population.begin(); it != testSim.population.end(); it++)
        delete it->second;
    testSim.population.clear();

    //-----------------------------------------
    // Case 5: Proliferation & Differentiation
    //-----------------------------------------
    tmpCell = new Cell(type, location);
    tmpCell->proliferation = 1;
    tmpCell->differentiation = 1;

    testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
    testSim.step();

    it = testSim.population.begin();
    it++;
    if (testSim.population.size() != 2)
        output << "testStep Failure: Simultaneous proliferation and movement failed" << endl;

    for (it = testSim.population.begin(); it != testSim.population.end(); it++)
        delete it->second;
    testSim.population.clear();

    //------------------------------------
    // Case 6: Movement & Differentiation
    //------------------------------------
    tmpCell = new Cell(type, location);
    tmpCell->move = 1;
    tmpCell->differentiation = 1;

    testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
    testSim.step();

    if (testSim.population.size() != 1 ||
            testSim.population.begin()->second->get_type() != PRELEPTOTENE)
        output << "testStep Failure: Simultaneous movement and differentiation failed" << endl;

    return output.str();
}

string TestSim::subStep_Empty()
{
    stringstream output;

    //<--------------------------------------Empty Population-------------------------------------->
    // 1) Generate an empty population
    // 2) Run a designated number of simulation step iterations
    //<-------------------------------------------------------------------------------------------->

    // Generate an empty population
    Simulator testSim;

    for (int i = 0; i < 100; i++)
        testSim.step();

    if (!testSim.population.empty())
    {
        output << "testStep Failure: Simulation generated cells from an initially empty population";
        output << endl;
    }

    return output.str();
}

string TestSim::subStep_Emptied()
{
    stringstream output;

    //<-----------------------------------------Emptied Population--------------------------------->
    // 1) Generate a non-empty population
    // 2) Reassign each member of the population to have a death timer less than the minimum
    //    proliferation timer and differentiation timer values for their respective cell types.
    // 3) Run simulation step iterations until 1 step after the simulation becomes empty or an upper
    //    bound is reached (test failure).
    //<-------------------------------------------------------------------------------------------->

    // Generate a non-empty population
    Simulator testSim;
    Cell* tmpCell;
    unsigned long long hash;
    multimap<unsigned long long, Cell*>::iterator it;

    for (int i = 0; i < 5; i++)
    {
        tmpCell = new Cell(UNDIFFERENTIATED_SPERMATOGONIA, pair<unsigned int, unsigned int> (
                               i, global.ENVIRONMENT_RADIUS));
        // Reassign each member of the population's death timers
        // Undifferentiated Spermatogonia proliferation min
        tmpCell->apoptosis = global.PROLIFERATION_MIN[0];
        if (global.DIFFERENTIATION_MIN[0] < global.PROLIFERATION_MIN[0])
            tmpCell->apoptosis = global.DIFFERENTIATION_MIN[0];
        hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
        testSim.populationSize[UNDIFFERENTIATED_SPERMATOGONIA]++;
    }

    for (int i = 0; i < 5; i++)
    {
        // Do the same for Differentiated Spermatagonia
        tmpCell = new Cell(DIFFERENTIATED_SPERMATOGONIA, pair<unsigned int, unsigned int> (
                               global.ENVIRONMENT_RADIUS, i));
        // Reassign each member of the population's death timers
        // Differentiated Spermatogonia proliferation min
        tmpCell->apoptosis = global.PROLIFERATION_MIN[1];
        if (global.DIFFERENTIATION_MIN[1] < global.PROLIFERATION_MIN[1])
            tmpCell->apoptosis = global.DIFFERENTIATION_MIN[1];
        hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
        testSim.populationSize[DIFFERENTIATED_SPERMATOGONIA]++;
    }

    for (int i = 0; i < 5; i++)
    {
        // Throw some random non-proliferating cells in - don't worry about minimizing the apoptosis
        CellType type = static_cast<CellType>(rand() % 8 + 2);
        tmpCell = new Cell(type, pair<unsigned int, unsigned int>(global.ENVIRONMENT_RADIUS,
                                                                  2 * global.ENVIRONMENT_RADIUS -
                                                                  i));
        hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
        testSim.populationSize[type]++;
    }


    int counter = 0;
    // Run simulation step iterations until empty or upper bound is met
    bool loop = true;
    for (counter = 0; counter < 1000 && loop; counter++)
    {
        if (testSim.population.empty())
            loop = false;
        testSim.step();

        for (it = testSim.population.begin(); it != testSim.population.end(); it++)
            if (it->second->proliferation == 1)
                it->second->proliferation = 100;
    }
    if (counter >= 1000)
    {
        cout << this->printSim(&testSim, true) << endl;
        output << "testStep Failure: Population with no proliferation failed to die" << endl;
    }

    return output.str();
}

string TestSim::subStep_Growing()
{
    stringstream output;

    //<---------------------------------------Growing Population----------------------------------->
    // 1) Generate a population with a set of spaced out stem cells. Set their proliferation timers
    //    to 1
    // 2) Run a cell step.
    // 3) Determine if the population is twice the initial amount
    //<-------------------------------------------------------------------------------------------->

    // Generate a population with a set of spaced out stem cells
    Simulator testSim;
    unsigned long long hash;
    Cell* tmpCell;

    for (int i = 1; i < 3; i++)
    {
        tmpCell = new Cell(UNDIFFERENTIATED_SPERMATOGONIA, pair<unsigned int, unsigned int>(
                                 2 * i, global.ENVIRONMENT_RADIUS));
        hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
        tmpCell->proliferation = 1;
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));

        tmpCell = new Cell(UNDIFFERENTIATED_SPERMATOGONIA, pair<unsigned int, unsigned int>(
                               2 * global.ENVIRONMENT_RADIUS - 2 * i, global.ENVIRONMENT_RADIUS));
        hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
        tmpCell->proliferation = 1;
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));

        tmpCell = new Cell(DIFFERENTIATED_SPERMATOGONIA, pair<unsigned int, unsigned int>(
                               global.ENVIRONMENT_RADIUS, 2 * i));
        hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
        tmpCell->proliferation = 1;
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));

        tmpCell = new Cell(DIFFERENTIATED_SPERMATOGONIA, pair<unsigned int, unsigned int>(
                               global.ENVIRONMENT_RADIUS, 2 * global.ENVIRONMENT_RADIUS - 2 * i));
        hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
        tmpCell->proliferation = 1;
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
    }

    // Run a cell step

    string before = printSim(&testSim);

    testSim.step();

    // Determine if the population is twice the initial amount
    if (testSim.population.size() != 16)
    {
        output << "testStep Failure: Population of cells with proliferations timers of 1 did not ";
        output << "double" << endl;

        //cout << "Before:\n" << before;
        ///cout << "After:\n" << printSim(&testSim);
    }

    return output.str();
}

string TestSim::subStep_Stable()
{
    stringstream output;

    //<----------------------------------------Stable Population----------------------------------->
    // 1) Generate a population with a determined number of cells
    // 2) Run the described simulation steps for a determined number of steps.
    // Note: While this subroutine does not test for anything specifically, it ensures the
    //       simulation as constructed is stable (i.e. won't crash).
    //<-------------------------------------------------------------------------------------------->

    // Generate a population with a determined number of cells
    Simulator testSim;
    Cell* tmpCell;
    unsigned long long hash;
    multimap<unsigned long, Cell*>::iterator it;

    for (int i = 0; i < 5; i++)
    {
        tmpCell = new Cell(UNDIFFERENTIATED_SPERMATOGONIA, pair<unsigned int, unsigned int> (
                               i, global.ENVIRONMENT_RADIUS));
        // Reassign each member of the population's death timers
        // Undifferentiated Spermatogonia proliferation min
        tmpCell->apoptosis = global.PROLIFERATION_MIN[0];
        if (global.DIFFERENTIATION_MIN[0] < global.PROLIFERATION_MIN[0])
            tmpCell->apoptosis = global.DIFFERENTIATION_MIN[0];
        hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
        testSim.populationSize[UNDIFFERENTIATED_SPERMATOGONIA]++;
    }

    for (int i = 0; i < 5; i++)
    {
        // Do the same for Differentiated Spermatagonia
        tmpCell = new Cell(DIFFERENTIATED_SPERMATOGONIA, pair<unsigned int, unsigned int> (
                               global.ENVIRONMENT_RADIUS, i));
        // Reassign each member of the population's death timers
        // Differentiated Spermatogonia proliferation min
        tmpCell->apoptosis = global.PROLIFERATION_MIN[1];
        if (global.DIFFERENTIATION_MIN[1] < global.PROLIFERATION_MIN[1])
            tmpCell->apoptosis = global.DIFFERENTIATION_MIN[1];
        hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
        testSim.populationSize[DIFFERENTIATED_SPERMATOGONIA]++;
    }

    for (int i = 0; i < 5; i++)
    {
        // Throw some random non-proliferating cells in - don't worry about minimizing the apoptosis
        CellType type = static_cast<CellType>(rand() % 8 + 2);
        tmpCell = new Cell(type, pair<unsigned int, unsigned int>(global.ENVIRONMENT_RADIUS,
                                                                  2 * global.ENVIRONMENT_RADIUS -
                                                                  i));
        hash = testSim.h(tmpCell->position.first, tmpCell->position.second);
        testSim.population.insert(pair<unsigned long long, Cell*>(hash, tmpCell));
        testSim.populationSize[type]++;
    }

    for (int i = 0; i < 1000; i++)
        testSim.step();

    return output.str();
}
