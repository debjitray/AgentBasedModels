#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "GlobalConstructs.h"

using namespace std;


struct Globals
{
public:

//<------------------------------------Non-Associated Definitions---------------------------------->
    double PI;
    int CUTOFF;  // ADDED BY DEBJIT RAY FOR PRELEP CELL NUMBER
    // The radius of the environment (including tubule) in grid squares
    // Calculated: 150um*2 / 13um per square = 23 squares (for the diameter)
    // As we have a center grid and radius is defined as the number of squares away
    // from that center square, we get (23 - 1) / 2 = radius
    unsigned int ENVIRONMENT_RADIUS;

    // The radius of the tubule (lumen) area in grid squares.
    // Calculated as environment radius was. Tubule radius defined as = 50um
    int TUBULE_RADIUS;

    // The number of micrometers per grid square for conversion purposes
    int GRID_SQUARE_SIZE;

    // The number of sertoli cells in the simulation
    int SERTOLI_NUMBER;

    // The number of blood vessels in the simulation
    int BLOOD_VESSEL_NUMBER;

//<-------------------------------------Proliferation Method Definitions--------------------------->
    int PROLIFERATION_SPLIT_U;     // Percentage chance that only one child cell will differentiate
    int PROLIFERATION_RESET_U;     // Percentage chance that both cells will not differentiate
    int PROLIFERATION_DIFFER_U;    // Percentage chance that both cells will differentiate

    int PROLIFERATION_SPLIT_D;     // Percentage chance that only one child cell will differentiate
    int PROLIFERATION_RESET_D;     // Percentage chance that both cells will not differentiate
    int PROLIFERATION_DIFFER_D;    // Percentage chance that both cells will differentiate

    int PROLIFERATION_SPLIT_DIP;     // ADDED BY DEBJIT RAY FOR DIPLOTENE PROLIFERATION
    int PROLIFERATION_RESET_DIP;     // ADDED  BY DEBJIT RAY FOR DIPLOTENE PROLIFERATION
    int PROLIFERATION_DIFFER_DIP;    // ADDED  BY DEBJIT RAY FOR DIPLOTENE PROLIFERATION

    int PROLIFERATION_SPLIT_SECO;     // ADDED  BY DEBJIT RAY FOR SECONDARY PROLIFERATION
    int PROLIFERATION_RESET_SECO;     // ADDED  BY DEBJIT RAY FOR SECONDARY PROLIFERATION
    int PROLIFERATION_DIFFER_SECO;    // ADDED  BY DEBJIT RAY FOR SECONDARY PROLIFERATION

    int INITIALDIFF_DIFFERENTIATION_MAX; // ADDED BY DEBJIT RAY TO TAKE CARE OF A4 INITIAL DIFF CELL DIFFERENTIATION
    int INITIALDIFF_DIFFERENTIATION_MIN; // ADDED BY DEBJIT RAY TO TAKE CARE OF A4 INITIAL DIFF CELL DIFFERENTIATION

    int INITIALUNDIFF_PROLIFERATION_MAX; // ADDED BY DEBJIT RAY TO TAKE CARE OF A4 INITIAL DIFF CELL DIFFERENTIATION
    int INITIALUNDIFF_PROLIFERATION_MIN; // ADDED BY DEBJIT RAY TO TAKE CARE OF A4 INITIAL DIFF CELL DIFFERENTIATION

//<---------------------------------------Germ Cell Definitions------------------------------------>
    int DIFFERENTIATION_MAX[10];
    int DIFFERENTIATION_MIN[10];

    int PROLIFERATION_MAX[10];
    int PROLIFERATION_MIN[10];
    int APOPTOSIS_MAX[10];
    int APOPTOSIS_MIN[10];
    int SPEED_MAX[10];
    int SPEED_MIN[10];
    double SIZE[10];

    // Upper bound for the radius of the circle to initially populate inward from
    double INITIAL_MAX[10];
    // Lower bound... Both in grid squares
    double INITIAL_MIN[10];

    // Initial number of cells in simulation
    unsigned int INITIAL_NUM[10];

    // Movement Direction Biases
    // Note: Assumed that these values sum to 100

    double MOVE_FORWARD[10];
    double MOVE_FORWARD_RIGHT[10];
    double MOVE_FORWARD_LEFT[10];
    double MOVE_RIGHT[10];
    double MOVE_LEFT[10];
    double MOVE_BACKWARD_RIGHT[10];
    double MOVE_BACKWARD_LEFT[10];
    double MOVE_BACKWARD[10];
};

// extern keyword initializes variable for program runtime
extern Globals global;

//<------------------------------------------Initialization---------------------------------------->
inline void global_insert(string type, stringstream *ss, int identifier)
{
    // Retrieve the index for the cell type as per the CellType enumeration

    string type_arr[10] = {"UNDIFFERENTIATED_SPERMATOGONIA", "DIFFERENTIATED_SPERMATOGONIA",
                           "PRELEPTOTENE", "LEPTOTENE", "ZYGOTENE", "PACHYTENE", "DIPLOTENE",
                           "SECOND_SPERMATOCTYE", "ROUND_SPERMATIDS", "ELONGATED_SPERMATIDS"};
    int index = -1;
    for (int i = 0; i < 10; i++)
        if (type_arr[i] == type)
            index = i;

    // Return on invalid arguments
    if (index == -1 || identifier < 0 || identifier > 19)
        return;

    if (identifier == 0)
        *ss >> global.DIFFERENTIATION_MAX[index];
    if (identifier == 1)
        *ss >> global.DIFFERENTIATION_MIN[index];
    if (identifier == 2)
        *ss >> global.PROLIFERATION_MAX[index];
    if (identifier == 3)
        *ss >> global.PROLIFERATION_MIN[index];
    if (identifier == 4)
        *ss >> global.APOPTOSIS_MAX[index];
    if (identifier == 5)
        *ss >> global.APOPTOSIS_MIN[index];
    if (identifier == 6)
        *ss >> global.SPEED_MAX[index];
    if (identifier == 7)
        *ss >> global.SPEED_MIN[index];
    if (identifier == 8)
        *ss >> global.SIZE[index];
    if (identifier == 9)
        *ss >> global.INITIAL_MAX[index];
    if (identifier == 10)
        *ss >> global.INITIAL_MIN[index];
    if (identifier == 11)
        *ss >> global.INITIAL_NUM[index];
    if (identifier == 12)
        *ss >> global.MOVE_FORWARD[index];
    if (identifier == 13)
        *ss >> global.MOVE_FORWARD_RIGHT[index];
    if (identifier == 14)
        *ss >> global.MOVE_FORWARD_LEFT[index];
    if (identifier == 15)
        *ss >> global.MOVE_RIGHT[index];
    if (identifier == 16)
        *ss >> global.MOVE_LEFT[index];
    if (identifier == 17)
        *ss >> global.MOVE_BACKWARD_RIGHT[index];
    if (identifier == 18)
        *ss >> global.MOVE_BACKWARD_LEFT[index];
    if (identifier == 19)
        *ss >> global.MOVE_BACKWARD[index];
}

inline bool initGlobals( )
{
    string line;
    string name;
    string value;
    ifstream infile ("parameters.txt");
    if (infile.is_open())
    {
        // Read global variables from the input file
        while (infile.good() )
        {
            getline (infile, line);

            // Don't read commented lines
            if (line.substr(0,2) != "//")
            {
                // Save the name (first part of line) and value (second part of line) seperately
                int space_i = line.find(" ");
                name = line.substr(0, space_i);
                value = line.substr(space_i + 1);
                stringstream ss;
                ss.str(value);

                if (name == "PI")
                    ss >> global.PI;
                if (name == "CUTOFF")         // ADDED BY DEBJIT RAY FOR PRELEP CELL NO. PARAMETER
                    ss >> global.CUTOFF;      // ADDED BY DEBJIT RAY FOR PRELEP CELL NO. PARAMETER
                if (name == "ENVIRONMENT_RADIUS")
                    ss >> global.ENVIRONMENT_RADIUS;
                if (name == "TUBULE_RADIUS")
                    ss >> global.TUBULE_RADIUS;
                if (name == "GRID_SQUARE_SIZE")
                    ss >> global.GRID_SQUARE_SIZE;
                if (name == "SERTOLI_NUMBER")
                    ss >> global.SERTOLI_NUMBER;
                if (name == "BLOOD_VESSEL_NUMBER")
                    ss >> global.BLOOD_VESSEL_NUMBER;
                if (name == "PROLIFERATION_SPLIT_U")
                    ss >> global.PROLIFERATION_SPLIT_U;
                if (name == "PROLIFERATION_RESET_U")
                    ss >> global.PROLIFERATION_RESET_U;
                if (name == "PROLIFERATION_DIFFER_U")
                    ss >> global.PROLIFERATION_DIFFER_U;
                if (name == "PROLIFERATION_SPLIT_D")
                    ss >> global.PROLIFERATION_SPLIT_D;
                if (name == "PROLIFERATION_RESET_D")
                    ss >> global.PROLIFERATION_RESET_D;
                if (name == "PROLIFERATION_DIFFER_D")
                    ss >> global.PROLIFERATION_DIFFER_D;

                if (name == "PROLIFERATION_SPLIT_DIP")         // ADDED BY DEBJIT RAY FOR DIPLOTENE PROLIFERATION
                    ss >> global.PROLIFERATION_SPLIT_DIP;      // ADDED BY DEBJIT RAY FOR DIPLOTENE PROLIFERATION
                if (name == "PROLIFERATION_RESET_DIP")         // ADDED BY DEBJIT RAY FOR DIPLOTENE PROLIFERATION
                    ss >> global.PROLIFERATION_RESET_DIP;      // ADDED BY DEBJIT RAY FOR DIPLOTENE PROLIFERATION
                if (name == "PROLIFERATION_DIFFER_DIP")        // ADDED BY DEBJIT RAY FOR DIPLOTENE PROLIFERATION
                    ss >> global.PROLIFERATION_DIFFER_DIP;     // ADDED BY DEBJIT RAY FOR DIPLOTENE PROLIFERATION


                if (name == "PROLIFERATION_SPLIT_SECO")         // ADDED BY DEBJIT RAY FOR SECONDARY PROLIFERATION
                    ss >> global.PROLIFERATION_SPLIT_SECO;      // ADDED BY DEBJIT RAY FOR SECONDARY PROLIFERATION
                if (name == "PROLIFERATION_RESET_SECO")         // ADDED BY DEBJIT RAY FOR SECONDARY PROLIFERATION
                    ss >> global.PROLIFERATION_RESET_SECO;      // ADDED BY DEBJIT RAY FOR SECONDARY PROLIFERATION
                if (name == "PROLIFERATION_DIFFER_SECO")        // ADDED BY DEBJIT RAY FOR SECONDARY PROLIFERATION
                    ss >> global.PROLIFERATION_DIFFER_SECO;     // ADDED BY DEBJIT RAY FOR SECONDARY PROLIFERATION

                if (name == "INITIALDIFF_DIFFERENTIATION_MAX")        // ADDED BY DEBJIT RAY FOR A4 INITIAL DIFF CELL DIFFERENTIATION
                    ss >> global.INITIALDIFF_DIFFERENTIATION_MAX;     // ADDED BY DEBJIT RAY FOR A4 INITIAL DIFF CELL DIFFERENTIATION
                if (name == "INITIALDIFF_DIFFERENTIATION_MIN")        // ADDED BY DEBJIT RAY FOR A4 INITIAL DIFF CELL DIFFERENTIATION
                    ss >> global.INITIALDIFF_DIFFERENTIATION_MIN;     // ADDED BY DEBJIT RAY FOR A4 INITIAL DIFF CELL DIFFERENTIATION

                if (name == "INITIALUNDIFF_PROLIFERATION_MAX")        // ADDED BY DEBJIT RAY FOR INITIAL UNDIFF CELL PROLIFERATION
                    ss >> global.INITIALUNDIFF_PROLIFERATION_MAX;     // ADDED BY DEBJIT RAY FOR INITIAL UNDIFF CELL PROLIFERATION
                if (name == "INITIALUNDIFF_PROLIFERATION_MIN")        // ADDED BY DEBJIT RAY FOR INITIAL UNDIFF CELL PROLIFERATION
                    ss >> global.INITIALUNDIFF_PROLIFERATION_MIN;     // ADDED BY DEBJIT RAY FOR INITIAL UNDIFF CELL PROLIFERATION
                // Assign cell type specific global variables
                else
                {
                    // Pass cell type and an identifier for the array to insert the value into
                    // to the global_insert() function
                    size_t i = name.find("DIFFERENTIATION_MAX");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 0);

                    i = name.find("DIFFERENTIATION_MIN");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 1);

                    i = name.find("PROLIFERATION_MAX");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 2);

                    i = name.find("PROLIFERATION_MIN");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 3);

                    i = name.find("APOPTOSIS_MAX");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 4);

                    i = name.find("APOPTOSIS_MIN");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 5);

                    i = name.find("SPEED_MAX");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 6);

                    i = name.find("SPEED_MIN");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 7);

                    i = name.find("SIZE");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 8);

                    i = name.find("INITIAL_MAX");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 9);

                    i = name.find("INITIAL_MIN");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 10);

                    i = name.find("INITIAL_NUM");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 11);

                    i = name.find("MOVE_FORWARD");
                    if (i != string::npos)
                    {
                        if (name.find("MOVE_FORWARD_RIGHT") != string::npos)
                            global_insert(name.substr(0,i - 1), &ss, 13);
                        else if (name.find("MOVE_FORWARD_LEFT") != string::npos)
                            global_insert(name.substr(0,i - 1), &ss, 14);
                        else
                            global_insert(name.substr(0,i - 1), &ss, 12);
                    }

                    i = name.find("MOVE_RIGHT");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 15);

                    i = name.find("MOVE_LEFT");
                    if (i != string::npos)
                        global_insert(name.substr(0,i - 1), &ss, 16);

                    i = name.find("MOVE_BACKWARD");
                    if (i != string::npos)
                    {
                        if (name.find("MOVE_BACKWARD_RIGHT") != string::npos)
                            global_insert(name.substr(0,i - 1), &ss, 17);
                        else if (name.find("MOVE_BACKWARD_LEFT") != string::npos)
                            global_insert(name.substr(0,i - 1), &ss, 18);
                        else
                            global_insert(name.substr(0,i - 1), &ss, 19);
                    }
                }
            }
        }
        infile.close();

        // Assign non-changable values
        for (int i = 2; i < 6; i++)
        {
            global.PROLIFERATION_MAX[i] = -1;
            global.PROLIFERATION_MIN[i] = -1;
        }
        // ADDED BY DEBJIT RAY FOR DIPLOTENE AND SECONDARY PROLIFERATION *****
        for (int i = 8; i < 10; i++)
        {
            global.PROLIFERATION_MAX[i] = -1;
            global.PROLIFERATION_MIN[i] = -1;
        } //*****

        global.DIFFERENTIATION_MAX[9] = -1;
        global.DIFFERENTIATION_MIN[9] = -1;
    }
    else
        // Couldn't read input file
        return false;
    // Success!

    return true;
}

inline string printGlobals( )
{
    stringstream ss;

    // Non-type-dependant parameters
    ss << "Pi: " << global.PI << endl;
    ss << "Cut off: " << global.CUTOFF << endl; // ADDED BY DEBJIT RAY FOR PRELEP CELL NO. PARAMETER
    ss << "Environment Radius: " << global.ENVIRONMENT_RADIUS << endl;
    ss << "Tubule Radius: " << global.TUBULE_RADIUS << endl;
    ss << "Grid Square Size: " << global.GRID_SQUARE_SIZE << endl;
    ss << "Sertoli Number: " << global.SERTOLI_NUMBER << endl;
    ss << "Blood Vessel Number: " << global.BLOOD_VESSEL_NUMBER << endl;
    ss << "Proliferation Split: " << global.PROLIFERATION_SPLIT_U << endl;
    ss << "Proliferation Reset: " << global.PROLIFERATION_RESET_U << endl;
    ss << "Proliferation Differ: " << global.PROLIFERATION_DIFFER_U << endl;
    ss << "Proliferation Split: " << global.PROLIFERATION_SPLIT_D << endl;
    ss << "Proliferation Reset: " << global.PROLIFERATION_RESET_D << endl;
    ss << "Proliferation Differ: " << global.PROLIFERATION_DIFFER_D << endl;

    ss << "Proliferation Split: " << global.PROLIFERATION_SPLIT_DIP << endl;    // ADDED BY DEBJIT RAY FOR DIPLOTENE PROLIFERATION
    ss << "Proliferation Reset: " << global.PROLIFERATION_RESET_DIP << endl;    // ADDED BY DEBJIT RAY FOR DIPLOTENE PROLIFERATION
    ss << "Proliferation Differ: " << global.PROLIFERATION_DIFFER_DIP << endl;  // ADDED BY DEBJIT RAY FOR DIPLOTENE PROLIFERATION

    ss << "Proliferation Split: " << global.PROLIFERATION_SPLIT_SECO << endl;   // ADDED BY DEBJIT RAY FOR SECONDARY PROLIFERATION
    ss << "Proliferation Reset: " << global.PROLIFERATION_RESET_SECO << endl;   // ADDED BY DEBJIT RAY FOR SECONDARY PROLIFERATION
    ss << "Proliferation Differ: " << global.PROLIFERATION_DIFFER_SECO << endl; // ADDED BY DEBJIT RAY FOR SECONDARY PROLIFERATION

    ss << " Diff Differentiation Max: " << global.INITIALDIFF_DIFFERENTIATION_MAX << endl;  // ADDED BY DEBJIT RAY FOR A4 INITIAL DIFF CELL DIFFERENTIATION
    ss << "Diff Differentiation Min: " << global.INITIALDIFF_DIFFERENTIATION_MIN << endl;   // ADDED BY DEBJIT RAY FOR A4 INITIAL DIFF CELL DIFFERENTIATION

    ss << " UnDiff Proliferation Max: " << global.INITIALUNDIFF_PROLIFERATION_MAX << endl;  // ADDED BY DEBJIT RAY FOR INITIAL UNDIFF CELL PROLIFERATION
    ss << "UnDiff Proliferation Min: " << global.INITIALUNDIFF_PROLIFERATION_MIN << endl;   // ADDED BY DEBJIT RAY FOR INITIAL UNDIFF CELL PROLIFERATION

    ss << endl;

    // Type-dependant parameters
    ss << "Differentiation Max:" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.DIFFERENTIATION_MAX[i] << endl;

    ss << endl << "Differentiation Min:" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.DIFFERENTIATION_MIN[i] << endl;

    ss << endl << "Proliferation Max" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.PROLIFERATION_MAX[i] << endl;

    ss << endl << "Proliferation Min" << endl;
    for (int i = 0; i < 10; i++)
       ss << global.PROLIFERATION_MIN[i] << endl;

    ss << endl << "Apoptosis Max" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.APOPTOSIS_MAX[i] << endl;

    ss << endl << "Apoptosis Min" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.APOPTOSIS_MIN[i] << endl;

    ss << endl << "Speed Max" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.SPEED_MAX[i] << endl;

    ss << endl << "Speed Min" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.SPEED_MIN[i] << endl;

    ss << endl << "Size" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.SIZE[i] << endl;

    ss << endl << "Initial Max" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.INITIAL_MAX[i] << endl;

    ss << endl << "Initial Min" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.INITIAL_MIN[i] << endl;

    ss << endl << "Initial Num" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.INITIAL_NUM[i] << endl;

    ss << endl << "Move Forward" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.MOVE_FORWARD[i] << endl;

    ss << endl << "Move Forward Right" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.MOVE_FORWARD_RIGHT[i] << endl;

    ss << endl << "Move Forward Left" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.MOVE_FORWARD_LEFT[i] << endl;

    ss << endl << "Move Right" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.MOVE_RIGHT[i] << endl;

    ss << endl << "Move Left" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.MOVE_LEFT[i] << endl;

    ss << endl << "Move Backward Right" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.MOVE_BACKWARD_RIGHT[i] << endl;

    ss << endl << "Move Backward Left" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.MOVE_BACKWARD_LEFT[i] << endl;

    ss << endl << "Move Backward" << endl;
    for (int i = 0; i < 10; i++)
        ss << global.MOVE_BACKWARD[i] << endl;

    return ss.str();
}

#endif // GLOBALVARIABLES_H
