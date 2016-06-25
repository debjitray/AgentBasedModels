#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <map>
#include <string>
#include <list>
#include <queue>
#include <math.h>
#include <sstream>
#include "cell.h"

using namespace std;

class Cell;

class Simulator
{
    friend class TestSim;

public:

    //PUBLIC METHODS

    /********************************************************************
    Default constructor
    ********************************************************************/
    Simulator( );

    /********************************************************************
    Default deconstructor
    ********************************************************************/
    ~Simulator( );

    /********************************************************************
    Advances the simulation by one hour
    ********************************************************************/
    void step( );

    /********************************************************************
    Populates the Simulator with initial numbers of germ cells

    @param typeArray
	An array indexed with the CellType enumeration indicating the
	number of each cell type to populate the simulation with
    ********************************************************************/
    bool populate(unsigned int *typeArray);

    /********************************************************************
    Hash function. Uniquely maps a pair of numbers (the 2D coordinate
    in our case) to a single number by use of the Cantor pairing function
    (see wikipedia.org/wiki/Pairing_function for more details).

    @param x
        The x coordinate value to map
    @param y
        The y coordinate value to map
    @return
        the unique hash of the pair
    ********************************************************************/
    unsigned long long h(unsigned int x, unsigned int y);

    /********************************************************************
    Returns the population size of given cell type

    @param type
        cell type
    ********************************************************************/
    double counter(int type);
    

    //PUBLIC ATTRIBUTES

    //Average distance from the center of the environment for each cell type
    double averageDistanceCenter[10];

    //Number of cells alive for each type. Both lines for output purposes
    unsigned int populationSize[10];

    //Distance from center of each cell for each cell type
    vector<double> distanceCenter[10];

    //Uses a hash tree implemented as a multimap. The 2D grid coordinates are
    //converted to an unique integer, which is the non-unique key of the map. A
    //Cell* is stored as the value of the map. The reason for using a multimap is
    //the need for mul non-unique keys. Non-unique keys are need as multiple cells
    //may occupy the same grid.
    multimap<unsigned long long,Cell*> population;

    //Stores the flags set at any given time by the simulator. Handled flags should be removed.
    list<Flag> flags;

    //Container for the locations of five blood vessel points in the environment
    vector< pair<unsigned int, unsigned int> > blood_vessels;

    //Container for the locations of the sertoli cells in the environment
    vector< pair<unsigned int, unsigned int> > sertoli;

    // ADDED BY DEBJIT RAY FOR UNIFORM PLACEMENT OF UNDIFFERENTIATED CELLS
    vector< pair<unsigned int, unsigned int> > undifferentiated;

private:

    //PRIVATE METHODS

    /********************************************************************
    Moves a germ cell towards one of the eight defined directions based
    on the defined percentage chances of moving a certain direction.
    Space for the cell must be availiable at the projected location
    otherwise the cell does not move. Cell directions are defined based
    on measuring the closest direction towards the center of the
    environment and defining it as "forward". The exceptions to this rule
    are undifferentiated spermatogonia, who define "forward" as the
    direction to the nearest blood vessel.

    @param cell
        The cell to move
    @return
        True if the move is successful, otherwise false
    ********************************************************************/
    bool move_cell(Cell* cell);

    /********************************************************************
    Attempts to create a new cell of the same type as the mother in one
    of the 8 adjacent spaces to the mother's location

    @param mother_location
        The pointer to the mother cell
    @return
        A pointer to the newly created cell or NULL if no cell was created
    ********************************************************************/
    Cell* proliferate(Cell *mother);

    /********************************************************************
    Determines a movement based on one grid square move in the passed
    direction

    @param move
        The direction to move
    @param location
        The location to move from
    @return
        The new location after the movement
    ********************************************************************/
    pair<unsigned int, unsigned int> move(Direction direction,
                                          pair<unsigned int,
                                          unsigned int> location, CellType type);

    /********************************************************************
    Determines the square surrounding the cell to define as "forward" for
    purposes of moving

    @param cell
        The cell to find the forward direction of
    @param type
        The type of cell to find the forward direction of
    @return
        The absolute direction of the position's individual "forward"
        direction
    ********************************************************************/
    Direction find_forward(pair<unsigned int, unsigned int> position,
                           CellType type);

    /********************************************************************
    Determines if there is availiable space for a cell at the given
    position. A square can hold 2 of either round spermatids or elongated
    spermatids (or one of each), or 1 of any other type of cell.

    @param location
        The grid square containing the space in question
    @param type
        The type of cell to determine if the space is availiable for
    @return
        True if the space is availiable, false otherwise
    ********************************************************************/
    bool space_availiable(pair<unsigned int,unsigned int> location,
                          CellType type);

    /********************************************************************
    Attempts to populate the simulation with the given number of cells of
    the given type. These new cells are added along with those currently
    in the Simulation.

    @param number
        The number of cells of the given type to populate
    @param type
        The type of cell to populate
    @param spots
        A pointer to an array of the locations of the spots availiable
        in the area this cell type is allowed to populate in
    @return
        True if all the requested cells were able to be populated, else
        false.
    ********************************************************************/
    bool populate_type(unsigned int number, CellType type,
                       vector<pair<unsigned int,unsigned int> >* spots);

    /********************************************************************
    Calculates the distance between two coordinates (pairs) by use of the
    distance formula. Distances are calculated between the centers of the
    grid squares.

    @param point1
        The first point to calculate the distance from
    @param point2
        The second point to calculate the distance from
    @return
        The distance between the two points
    ********************************************************************/
    double distance(pair<unsigned int, unsigned int> point1,
                    pair<unsigned int, unsigned int> point2);

    /********************************************************************
    Updates the data vectors with new data for the current time step
    ********************************************************************/
    void update_data( );
};

#endif // SIMULATOR_H
