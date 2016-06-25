#ifndef CELL_H
#define CELL_H

#include <string>
#include <time.h>
#include <utility>
#include <cstdlib>
#include "GlobalConstructs.h"
#include "GlobalVariables.h"
#include "simulator.h" // ADDED BY DEBJIT RAY FOR THE UNDIFF PROLIFERATION DEPENDING ON PRELEP, INCLUDES THE SIMULATOR IN CELL

using namespace std;

class Simulator;

class Cell
{
    friend class TestCell;
    friend class TestSim;

public:

    //PUBLIC METHODS

    /********************************************************************
    Default constructor

    @param type
        The type of cell to construct as defined by the CellType
        enumeration. (See GlobalDefinitions.h)
    @param location
        The coordinates of the cell on the unsigned coordinate system.
    ********************************************************************/
    Cell(CellType type, pair<unsigned int, unsigned int> location);

//ADDED BY DEBJIT RAY FOR THE UNDIFF PROLIFERATION DEPENDING ON PRELEP (SAME AS ABOVE EXCEPT HAS SIMULATOR INSTANCE)*****
   /********************************************************************
    constructor

    @param type
        The type of cell to construct as defined by the CellType
        enumeration. (See GlobalDefinitions.h)
    @param location
        The coordinates of the cell on the unsigned coordinate system.
    @param simulator instance
	The simulator instance within which the cell lives
    ********************************************************************/
    Cell(CellType type, pair<unsigned int, unsigned int> location, Simulator *sim);
//*****

   /********************************************************************
    Advances the cell through time by one hour

    @param proliferate
        To be set true if the cell proliferates on this step; false
        otherwise
    @param die
        To be set true if the cell dies on this step; false otherwise
    @param moved
        To be set true if the cell moves on this step; false otherwise
    @param grow
        To be set true if the cell differentiates on this step; false
        otherwise
    ********************************************************************/
    void step(bool *proliferate, bool *die, bool *moved, bool *grow);

    /********************************************************************
    Causes the cell to attempt to proliferate next turn after it tries
    to profliferate the first time. Instead of choosing a random number
    from the proliferation range for the current step, we set the
    differentiation value from 0 to 1.
    ********************************************************************/
    void deproliferate();

    /********************************************************************
    Causes the cell to attempt to move next turn after it tries to move
    the first time.
    ********************************************************************/
    void demove();

    /********************************************************************
    Retrives the cell type
    ********************************************************************/
    CellType get_type();

    /********************************************************************
    Operator overload for the assignment operator = . Copies the right
    hand side Cell to the current (left hand side) Cell.
    ********************************************************************/
    void operator=(Cell* rhs);


    //PUBLIC ATTRIBUTES

    // The coordinates (on unsigned x-y coordinate system) of the grid square
    // containing the cell. The unsigned x-y coordinate system is simply the
    // first quadrent of a standard coordinate system. The lowest-leftmost square
    // has the coordinates of (0,0).
    pair<unsigned int,unsigned int> position;

    // Timers for events. Hours until the denoted behaviors occurs
    int proliferation;
    int apoptosis;
    int differentiation;
    int move;

    //ADDED BY DEBJIT RAY FOR UNIQUE ID
    int id;
    string parent;

private:

    //PRIVATE METHODS

    /********************************************************************
    Evolves the cell into its next cell type
    ********************************************************************/
    void evolve( );

    /********************************************************************
    Assigns the cell a new random apoptosis based on the cell type
    ********************************************************************/
    void assign_apoptosis( );

    /********************************************************************
    Assigns the cell a new random proliferation based on the cell type
    ********************************************************************/
    void assign_proliferation( );

    /********************************************************************
    Assigns the cell a new random differentiation based on the cell type
    ********************************************************************/
    void assign_differentiation( );

    /********************************************************************
    Assigns the cell a new random speed based on cell type
    ********************************************************************/
    void assign_move( );

    //PRIVATE ATTRIBUTES

    // The current cell type of the germ cell
    CellType type;

    //ADDED BY DEBJIT RAY FOR THE UNDIFF PROLIFERATION DEPENDING ON PRELEP
    // THE SIMULATOR INSTANCE FOR WHICH THE CELL LIVES
    Simulator * sim;

    // ADDED BY DEBJIT RAY FOR UNIQUE ID
    static int cnt;

};

#endif // CELL_H
