#include "simulator.h"
#include<sstream> // ADDED BY DEBJIT RAY FOR UNIQUE ID

Simulator::Simulator( )
{
    // Initialize population counters
    for (int i = 0; i < 10; i++)
    {
        this->populationSize[i] = 0;
        this->averageDistanceCenter[i] = 0;
    }

    pair<unsigned int, unsigned int> center(global.ENVIRONMENT_RADIUS,
                                            global.ENVIRONMENT_RADIUS);

    //<-------------------------Blood Vessel Initialization---------------------------------------->
    // 1) Divide the circle into the desired number of angles:
    //      Ex. 360 / 5 = 72 --> i*72 gives us five equally spaced angles
    // 2) Use cos() and sin() to get x and y vector components respectively
    //    Note: cmath cos() and sin() take radians as arguments.
    // 3) Add the grid_size (radius) to convert to unsigned coordinates
    // 4) Have our integer conversions invalidated any of our placements?
    //    Yes --> 1) Move the cell towards the center. We can use the find forward function to
    //               determine which direction to move. Elongated spermatids always move toward
    //               the center so we can use their type to pass into the find forward function.
    //<-------------------------------------------------------------------------------------------->
    for (int i = 0; i < global.BLOOD_VESSEL_NUMBER; i++)
    {
        // CHANGED BY DEBJIT RAY, CHANGED THE COS TO SIN and VICE VERSA TO AVOID OVERALP OF BLOOD VESSEL AND SERTOLI
        this->blood_vessels.push_back(pair<unsigned int, unsigned int> (
            (int)(sin(i * 2 * global.PI / global.BLOOD_VESSEL_NUMBER) * (global.ENVIRONMENT_RADIUS)
                + global.ENVIRONMENT_RADIUS),
            (int)(cos(i * 2 * global.PI / global.BLOOD_VESSEL_NUMBER) * (global.ENVIRONMENT_RADIUS)
                  + global.ENVIRONMENT_RADIUS)));


        // Have our integer conversions invalidated any of our placements?
        // This occurs when we have integer type casting which takes the floor of the value.
        // This shifts every value towards the origin as opposed to necessarily towards the center
        if (distance(this->blood_vessels[i], center) > global.ENVIRONMENT_RADIUS)

            // Move the cell towards the center
            this->blood_vessels[i] = this->move(
                    this->find_forward(this->blood_vessels[i], ELONGATED_SPERMATIDS),
                    this->blood_vessels[i], ELONGATED_SPERMATIDS);
    }

    //<-------------------------Sertoli Cell Initialization---------------------------------------->
    //This is done using the same algorithm as the blood vessel initialization.

    //<-------------------------------------------------------------------------------------------->
    for (int i = 0; i < global.SERTOLI_NUMBER; i++)
    {
        this->sertoli.push_back(pair<unsigned int, unsigned int> (
            (int)(cos(i * 2 * global.PI / global.SERTOLI_NUMBER) * global.ENVIRONMENT_RADIUS +
                  global.ENVIRONMENT_RADIUS),
            (int)(sin(i * 2 * global.PI / global.SERTOLI_NUMBER) * global.ENVIRONMENT_RADIUS +
                  global.ENVIRONMENT_RADIUS)));

        if (distance(this->sertoli[i], center) > global.ENVIRONMENT_RADIUS)
            this->sertoli[i] = this->move(
                    this->find_forward(this->sertoli[i], ELONGATED_SPERMATIDS),
                    this->sertoli[i], ELONGATED_SPERMATIDS);
    }
}

Simulator::~Simulator()
{
    multimap<unsigned long long, Cell*>::iterator it;
    for (it = this->population.begin(); it != this->population.end(); it++)
        delete it->second;
}

void Simulator::step()
{


    // Cell behavior booleans
    bool proliferate, die, move, diff;

    unsigned long long hash = 0;
    Cell* tmpCell;

    // Cell action queues for preventing iterator invalidation
    queue<Cell*> to_proliferate, to_remove, to_move;

    // Used for inserting a new cell into the multimap on a proliferate or move
    pair<unsigned long long, Cell*> insert;

    // Center of the environment
    pair<unsigned int, unsigned int> center;

    multimap<unsigned long long,Cell*>::iterator it;

    // Initialize the center using the environment's radius
    center.first = global.ENVIRONMENT_RADIUS;
    center.second = global.ENVIRONMENT_RADIUS;

    //<--------------------------Population Step Iteration----------------------------------------->
    // Iterate through each cell in our population and call their step() function
    // 1) Reset the booleans which will represent individual cell behaviors
    // 2) Call the step function
    // 3) Did our cell fire the proliferate signal?
    //    Yes --> 1) Push the cell onto the proliferation queue
    // 4) Did our cell fire the move signal and not the die signal?
    //    Yes --> 1) Push the cell onto the move queue
    //        --> Note: we do not act upon cells that move and die as these will die before movement
    // 5) Did our cell fire the death signal?
    //    Yes --> 1) Push the cell onto the remove queue
    // 6) Did our cell fire the differentiate signal?
    //    Yes --> **Ability to perform actions on differentiation here**
    //<-------------------------------------------------------------------------------------------->

    for (it = this->population.begin(); it != this->population.end(); it++)
    {
        // Reset the booleans which will represent individual cell behaviors
        proliferate = false;
        die = false;
        move = false;
        diff = false;

        // Call the step function
        it->second->step(&proliferate, &die, &move, &diff);

        // Did our cell fire the proliferate signal?
        if (proliferate)
            to_proliferate.push(it->second);

        // Did our cell fire the move signal and not the die signal?
        if (move && !die)
            to_move.push(it->second);

        // Did our cell fire the death signal?
        if (die)
            to_remove.push(it->second);

        // Did our cell fire the differentiate signal?
        if (diff)
        {
            // Nothing to do here right now...
        }
    }

    //<-------------------------------Proliferation Actions---------------------------------------->
    // 1) Attempt to proliferate and get a pointer to the new Cell                        => O(1)
    // 2) Did we create a new Cell?                                                       => O(1)
    //    Yes -->    Construct a pair object to insert into the multimap
    //            1) Get hash for the Cell location and store in the pair                 => O(1)
    //            2) Copy the new Cell pointer into the pair                              => O(1)
    //            3) Insert the constructed pair into the multimap                        => O(logN)
    //
    //    No  --> 1) Have the mother cell try again next turn (ONCE DEBJIT)                      => O(1)
    //
    // 3) Pop the front off the proliferate queue and move to the next one.               => O(1)
    //<-------------------------------------------------------------------------------------------->

    while (!to_proliferate.empty())
    {
        // Attempt to proliferate and get a pointer to the new Cell
        tmpCell = this->proliferate(to_proliferate.front());

        // Did we create a new Cell?
        if (tmpCell != NULL)
        {
            // Yes...
            insert.first = h(tmpCell->position.first, tmpCell->position.second);
            insert.second = tmpCell;
            this->population.insert(insert);
        }
        // DELETED BY DEBJIT RAY IN TO STOP THE DEPROLIFERATE(), CELLS NEVER TRY TO PROLIFERATE ON NEXT HOUR
        //else
            // No...
            //to_proliferate.front()->deproliferate();

        // Pop the front off the proliferate queue and move to the next one
        to_proliferate.pop();
    }

    //<----------------------------------Movement Actions------------------------------------------>
    // Our hash tree is indexed by position. Thus, to move we must reinsert our cell
    //
    // 1) Retrieve the hash for the position of the cell to be removed                    => O(1)
    // 2) Determine which cell in the set is the one to remove                            => O(logN)
    // 3) Is it possible to move the cell?                                                => O(1)
    //    No  --> 1) Have the cell try again next turn                                    => O(1)
    //
    //    Yes -->    Reinsert the cell into the population tree with its updated hash.
    //               We'll use a pair object to insert into the multimap
    //
    //            1) Retrieve an updated hash and store in the pair                       => O(1)
    //            2) Copy the Cell pointer into the pair                                  => O(1)
    //            3) Is the cell within the lumen area (50 um of 150 um tubule)?          => O(1)
    //               Yes --> 1) Is the cell an elongated spermatid?                       => O(1)
    //                          Yes --> 1) Push the cell onto the remove queue            => O(1)
    //                                     (elongated spermatids disappear.)
    //                          No  --> 1) Set a flag indicating an illegal cell is in    => O(1)
    //                                     the lumen area

    //            4) Insert the constructed pair object into the multimap                 => O(logN)
    //            5) Erase the old position element from the multimap                     => O(1)
    //
    // 4) Pop the front off the move queue and move to the next one.                      => O(1)
    //<-------------------------------------------------------------------------------------------->

    while (!to_move.empty())
    {
        // Retrieve the hash for the position of the cell to be removed
        hash = h(to_move.front()->position.first,
                 to_move.front()->position.second);

        // Determine which cell in the set is the one to remove
        for (it = this->population.find(hash); it->second != to_move.front(); it++) ;

        // Is it possible to move the cell?
        if (!this->move_cell(it->second))
            it->second->demove();
        else
        {
            // Retrieve an updated hash and store in the pair
            insert.first = h(it->second->position.first,
                             it->second->position.second);

            // Copy the Cell pointer into the pair
            insert.second = it->second;

            // Is the cell within the lumen area (50 um of 150 um tubule)?
            if (distance(insert.second->position, center) <= global.TUBULE_RADIUS)
            {
                // Is the cell an elongated spermatid?
                // CHANGED BY DEBJIT RAY TO KEEP COUNT OF ELONGATED SPERMATIDS ENTERING LUMEN
                if (insert.second->get_type() == ELONGATED_SPERMATIDS)
                {

                    to_remove.push(insert.second);
                    // ADDED BY DEBJIT RAY TO KEEP COUNT OF ELONGATED SPERMATIDS ENTERING LUMEN*****
                    Flag insert_flag;
                    insert_flag.message = "W::";
                    insert_flag.message.append(toString(insert.second->get_type()));
                    insert_flag.message.append(" has reached the lumen area.");
                    insert_flag.time = -1;
                    insert_flag.type = ILLEGAL_CELL_LUMEN_AREA;
                    this->flags.push_back(insert_flag);
                } //*****
                // CHANGED BY DEBJIT RAY TO KEEP ACCOUNT OF OTHER CELLS ENTERING LUMEN
                else
                {
                    to_remove.push(insert.second);
                    Flag insert_flag;
                    insert_flag.message = "ERROR: Cell of type ";
                    insert_flag.message.append(toString(insert.second->get_type()));
                    insert_flag.message.append(" has reached the lumen area.");
                    insert_flag.time = -1;
                    insert_flag.type = ILLEGAL_CELL_LUMEN_AREA;
                    this->flags.push_back(insert_flag);
                }

            }
            this->population.insert(insert);
            this->population.erase(it);
        }

        // Pop the front off the move queue and move to the next one.
        to_move.pop();
    }

    //<-------------------------------------Removal Actions---------------------------------------->
    // 1) Retrieve the hash for the position of the cell to be removed                    => O(1)
    // 2) Determine which cell in the set is the one to remove                            => O(logN)
    //    This is done by iterating through the set and comparing the Cell pointers
    //    in the range set to the one on the queue we're looking for
    // 4) Delete the cell                                                                 => O(1)
    // 5) Erase the element on the multimap                                               => O(1)
    // 6) Pop off the front of the removal queue and move to the next one.                => O(1)
    //<-------------------------------------------------------------------------------------------->

    while (!to_remove.empty())
    {
        // Retrieve the hash for the position of the cell to be removed
        hash = h(to_remove.front()->position.first,
                 to_remove.front()->position.second);

        for (it = this->population.find(hash); it->second != to_remove.front(); it++) ;

        // Erase the element on the multimap, deletes the cell
        this->population.erase(it);

        // Pop off the front of the removal queue and move to the next one.
        to_remove.pop();
    }

    update_data();
}

bool Simulator::populate(unsigned int *typeArray)
{
    // Array of vectors representing the grid squares availiable for that type. One vector per cell
    // type is allocated.
    vector<pair<unsigned int, unsigned int> > spaceArray[10];

    // Array of values which store CellType enum values. Used to map the sorted spaceArray indexes
    // (which coorespond to the indexArray indexes) to the typeArray indexes (which coorespond to
    // the indexArray values).
    // Ex. spaceArray (sorted) stores: 0->diff #spots, 1->elongated #spots, 2->leptotene #spots ...
    //     typeArray stores: 0->undiff #cells, 1->diff #cells, 2->preleptotene #cells ...
    //     indexArray stores: 0->diff, 1->elongated, 2->leptotene ...
    CellType indexArray[10];

    pair<int, int> checkLocation;

    // The center of the environment. We'll define this as 0,0 for now and reassign
    // everything with respect to our unsigned coordinate system later
    pair<int, int> center;
    center.first = 0;
    center.second = 0;

    //<-----------------------------------Determine Availiable Space------------------------------->
    // Get the number of grid squares availiable in the valid initial area for each type. This will
    // be done by counting a quarter of the area and then multiplying by 4. Valid locations are
    // those whose distances (from middle of square to middle of center square) are less than the
    // difference in the TYPE_INITIAL values.
    //
    // 1) Populate the indexArray in CellType enumeration order
    //
    //                              *** Handle the x-intercept ***
    // 2) Explicitly cast the max radius as an int, this will give us a valid integer x intercept
    // 3) Add each valid entry on one axis for each of the 4 quadrents. Exclude center. Modify the
    //    values back to unsigned coordinates for hashing
    // 4) Exclusively add the center (so we don't recount) if needed
    //
    //                              *** Handle the remaining area ***
    // 5) Move the y coordinate up
    // 6) Reset the x coordinate to the radius of the circle
    //
    //    We will count rows and increase our y by one each iteration. When our y grows bigger then
    //    the radius we know we have no more valid squares.
    //
    // 7) Move our checkLocation in until we are within the radius
    //
    //    We now have the farthest right valid square for this row.
    //
    // 8) Add the hashes of each of the values in that row to the appropriate vector (excluding
    //    values on x = 0). Modify back to unsigned coordinates for hashing. Rotate the point 4
    //    times to get a point for each quadrent.
    // 9) Move the y coordinate up
    // 10)Reset the x coordinate to the radius of the circle
    // 11)Is our y bigger than the max radius?
    //    No  --> Goto Step 7
    //
    //                              *** Make Availiable Space Desicion ***
    // 12)Do we have enough space for all types?
    //    No  --> 1) Set an appropriate flag
    //            2) Return false
    //    Yes --> 1) Clear out all the elements in population
    //            2) Reset all of the population counters to 0
    //<-------------------------------------------------------------------------------------------->

    // Populate the indexArray in CellType enumeration order
    for (int i = 0; i < 10; i++)
        indexArray[i] = static_cast<CellType>(i);

    for (int i = 0; i < 10; i++)
    {
        // Explicitly cast the max radius as an int, this will give us a valid integer x intercept
        checkLocation.first = (unsigned int)(global.INITIAL_MAX[i]);
        checkLocation.second = 0;

        // Add each valid entry on one axis for each of the 4 quadrents
        for (int n = checkLocation.first; n >= global.INITIAL_MIN[i] && n > 1; n--)
        {
            //Quadrent I
            spaceArray[i].push_back(pair<unsigned int, unsigned int>(
                                    global.ENVIRONMENT_RADIUS+n,global.ENVIRONMENT_RADIUS));
            //Quadrent II
            spaceArray[i].push_back(pair<unsigned int, unsigned int>(
                                    global.ENVIRONMENT_RADIUS,global.ENVIRONMENT_RADIUS+n));
            //Quadrent III
            spaceArray[i].push_back(pair<unsigned int, unsigned int>(
                                    global.ENVIRONMENT_RADIUS-n,global.ENVIRONMENT_RADIUS));
            //Quadrent IV
            spaceArray[i].push_back(pair<unsigned int, unsigned int>(
                                    global.ENVIRONMENT_RADIUS,global.ENVIRONMENT_RADIUS-n));
        }

        // Exclusively add the center (so we don't recount) if needed
        if (global.INITIAL_MIN[i] == 0)
            spaceArray[i].push_back(pair<unsigned int, unsigned int>(
                                        global.ENVIRONMENT_RADIUS,global.ENVIRONMENT_RADIUS));

        // Move the y coordinate up
        checkLocation.second++;

        // Reset the x coordinate to the radius of the circle
        checkLocation.first = (unsigned int)(global.INITIAL_MAX[i]);

        // Move our checkLocation in until we are within the radius
        while (distance(checkLocation,center) > global.INITIAL_MAX[i])
            checkLocation.first--;

        while (checkLocation.first != 0)
        {
            // Add elements to the vector while we are farther out than the inner circle
            while (distance(checkLocation,center) >= global.INITIAL_MIN[i] &&
                   checkLocation.first != 0)
            {
                //Quadrent I
                spaceArray[i].push_back(pair<unsigned int, unsigned int>(
                                    global.ENVIRONMENT_RADIUS + checkLocation.first,
                                    global.ENVIRONMENT_RADIUS + checkLocation.second));
                //Quadrent II
                spaceArray[i].push_back(pair<unsigned int, unsigned int>(
                                    global.ENVIRONMENT_RADIUS - checkLocation.second,
                                    global.ENVIRONMENT_RADIUS + checkLocation.first));
                //Quadrent III
                spaceArray[i].push_back(pair<unsigned int, unsigned int>(
                                    global.ENVIRONMENT_RADIUS - checkLocation.first,
                                    global.ENVIRONMENT_RADIUS - checkLocation.second));
                //Quadrent IV
                spaceArray[i].push_back(pair<unsigned int, unsigned int>(
                                    global.ENVIRONMENT_RADIUS + checkLocation.second,
                                    global.ENVIRONMENT_RADIUS - checkLocation.first));
                checkLocation.first--;
            }
            // Move the y coordinate up
            checkLocation.second++;

            // Reset the x coordinate to the radius of the circle
            checkLocation.first = (unsigned int)(global.INITIAL_MAX[i]);
            while (distance(checkLocation,center) > global.INITIAL_MAX[i] &&
                   checkLocation.first != 0)
                checkLocation.first--;
        }

        // Remove sertoli nuclei spaces
        for (int n = 0; n < global.SERTOLI_NUMBER; n++)
            for (unsigned int r = 0; r < spaceArray[i].size(); r++)
                if (spaceArray[i][r] == this->sertoli[n])
                {
                    spaceArray[i][r] = spaceArray[i].back();
                    spaceArray[i].pop_back();
                }
    }
    // Do we have enough space for all types?
    for (int i = 0; i < 10; i++)
    {
        if (typeArray[i] > spaceArray[i].size())
        {
            // No...
            Flag insert_flag;
            insert_flag.message = "Warning: The amount of spaces being asked to populate is ";
            insert_flag.message.append("larger than the maximum number of availiable spaces");
            insert_flag.type = INITIAL_EXCEEDS_MAX;
            insert_flag.time = -1;
            this->flags.push_back(insert_flag);
            return false;
        }
    }

    // Yes...
    multimap<unsigned long long, Cell*>::iterator it;
    for (it = this->population.begin(); it != this->population.end(); it++)
        delete it->second;
    this->population.clear();

    //<------------------------------------Attempt to Populate------------------------------------->
    // We need to populate the cell types with the smallest amount of space availiable first in
    // order to ensure they're not "pushed out" by overlapping cell types who may have space
    // elsewhere. Since we only have 10 elements there's not much point in writing a QuickSort
    // or other recursive function (runtime difference is negligible: about 33.22 for O(nlg(n))
    // quicksort vs 55 for bubblesort)
    //
    // 1) Sort spaceArray in increasing order.
    //    --> Swap the spaceArray elements
    //    --> Swap the indexArray elements to maintain the array's mapping functionality
    // 2) Can we populate in order of CellTypes from the smallest space to populate in to CellTypes
    //    with the largest space to populate in?
    //    No  --> 1) Set an appropriate flag
    //            2) Clear the simulation
    //    Yes --> 1) Report a successful population process
    //<-------------------------------------------------------------------------------------------->

    vector<pair<unsigned int, unsigned int> > tmp_num;
    CellType tmp_type;

    // Sort spaceArray in increasing order
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 9-i; j++)
        {
            if (spaceArray[j].size() > spaceArray[j+1].size())
            {
                // Swap the spaceArray elements
                tmp_num = spaceArray[j+1];
                spaceArray[j+1] = spaceArray[j];
                spaceArray[j] = tmp_num;

                // Swap the indexArray elements to maintain the array's mapping functionality
                tmp_type = indexArray[j+1];
                indexArray[j+1] = indexArray[j];
                indexArray[j] = tmp_type;
            }
        }
    }

    // Can we populate in order of CellTypes from the smallest space to populate in to CellTypes
    // with the largest space to populate in?
    for (int i = 0; i < 10; i++)
    {
        if (!populate_type(typeArray[indexArray[i]],indexArray[i],
                           &spaceArray[i]))
        {
            // No...
            multimap<unsigned long long, Cell*>::iterator it;
            for (it = this->population.begin(); it != this->population.end(); it++)
                delete it->second;
            this->population.clear();

            Flag insert_flag;
            stringstream ss;
            ss << "Not enough space to populate " << typeArray[indexArray[i]] << " cells of type ";
            ss << toString(indexArray[i]);
            insert_flag.message = ss.str();
            insert_flag.type = INITIAL_NOT_ENOUGH_SPACE;
            insert_flag.time = -1;

            this->flags.push_back(insert_flag);
            return false;
        }
    }
    // Yes...
    update_data();
    return true;
}

bool Simulator::populate_type(unsigned int number, CellType type,
                              vector<pair<unsigned int,unsigned int> >* spots)
{
    //<---------------------------------Populate Type---------------------------------------------->
    // As the areas to populate different cell types can overlap each other, we have no way of
    // initially predicting what will be in our population space as other cell populations may have
    // randomly spawned inside it. However, we do know which spaces are availiable for our
    // population to spawn in so we can check each of those spaces to see if there is space to spawn
    //
    // 1) Mix up the spots vector a number of times based on it's size
    //
    //    Now we'll iterate through the vector to search each spot for availiable space
    //
    // 2) For each spot:
    //      1) Is there space availiable in the current spot?
    //         Yes --> 1) Populate the cell
    //                 2) Decrement the number of cells to be populated
    //                 4) Is the number of cells to be populated 0?
    //                    Yes --> 1) Return and report success
    //      2) Move to the next spot

   //<-------------------------------------------------------------------------------------------->

    // ADDED BY DEBJIT RAY FOR UNDIFF UNIFORM DISTRIBUTION, REMOVED THE RANDOM INITIALIZATION FOR UNDIFF *****
    pair<unsigned int, unsigned int> center(global.ENVIRONMENT_RADIUS,
                                            global.ENVIRONMENT_RADIUS);
    if (type == UNDIFFERENTIATED_SPERMATOGONIA) {
        for (int i = 0; i < number; i++)
        {
            pair<unsigned long long, Cell*> insert;
            this->undifferentiated.push_back(pair<unsigned int, unsigned int> (
                (int)(sin(5+(i * 2 * global.PI / number)) * global.ENVIRONMENT_RADIUS +
                      global.ENVIRONMENT_RADIUS),
                (int)(cos(5+(i * 2 * global.PI / number)) * global.ENVIRONMENT_RADIUS +
                      global.ENVIRONMENT_RADIUS)));
            // ADDED BY DEBJIT RAY A 5 DEGREE OFFSET FOR UNDIFF CELLS

            if (distance(this->undifferentiated[i], center) > global.ENVIRONMENT_RADIUS)
                this->undifferentiated[i] = this->move(
                        this->find_forward(this->undifferentiated[i], ELONGATED_SPERMATIDS),
                        this->undifferentiated[i], ELONGATED_SPERMATIDS);

            insert.second = new Cell(type, this->undifferentiated[i], this);
            insert.first = h(this->undifferentiated[i].first,this->undifferentiated[i].second);
            this->population.insert(insert);

        }
        return true;  //*****
    }
    else {
        int y = 0;
        int x = 0;
        pair<unsigned int, unsigned int> tmp;

        // Mix up the spots vector a number of times based on it's size
        for (unsigned int i = 0; i < spots->size(); i++)
        {
             x = rand() % spots->size();
             y = rand() % spots->size();
            tmp = (*spots)[x];
            (*spots)[x] = (*spots)[y];
            (*spots)[y] = tmp;
        }

        pair<unsigned long long, Cell*> insert;
        while (!spots->empty())
        {
            // Is there space availiable in the current spot?
            if(this->space_availiable(spots->back(),type))
            {
             if (number == 0)
                 return true;
             // Yes...
             insert.second = new Cell(type, spots->back(), this);  // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
             insert.first = h(spots->back().first,spots->back().second);
                this->population.insert(insert);
                number--;
            }
            // Is there still space availiable in the current spot? (i.e. populated a small cell)
            if (this->space_availiable(spots->back(),type))
            {
             // Mix it up so we do not populate small cells side by side every time
                x = rand() % spots->size();
                tmp = (*spots)[x];
                (*spots)[x] = spots->back();
                 spots->back() = tmp;
            }
            else
            // Move to the next spot - else allows us to check for multiple populations in the same
            // location
                spots->pop_back();

        }
        if (number == 0)
            return true;
        return false;
    }
}

bool Simulator::move_cell(Cell *cell)
{
    //<----------------------Determine Cell's Absolute Forward Direction--------------------------->
    //Note that the Direction enum is defined as follows:
    // 7   0   1
    // 6       2
    // 5   4   3
    //
    // 1) Get a random number between 0 and 99 inclusively (100% moving probability)
    // 2) Assign an array cooresponding to relative directions values based on their #defined
    //    chances of occuring
    // 3) Check the roll against the checkArray values:
    //        1) Is the roll greater than or equal to the checkArray value?
    //           Yes --> 1) Is there room to move in the desired direction?
    //                      No  --> 1) Return and report failure
    //                      Yes --> 1) Move in the desired direction
    //                              2) Return and report success!
    //           No  --> 1) Increment the direction and try again
    // 4) Guard by returning in case the defined movement constants do not sum to 100
    //<-------------------------------------------------------------------------------------------->

    Direction direction = this->find_forward(cell->position, cell->get_type());
    pair<unsigned int, unsigned int> new_location;

    // Get a random number between 0 and 99 inclusively
    int roll = rand() % 100;

    double checker = 100;

    // Assign an array cooresponding to relative directions values based on their #defined
    // chances of occuring
    double checkArray[8];
    checkArray[0] = checker -= global.MOVE_FORWARD[cell->get_type()];
    checkArray[1] = checker -= global.MOVE_FORWARD_RIGHT[cell->get_type()];
    checkArray[2] = checker -= global.MOVE_RIGHT[cell->get_type()];
    checkArray[3] = checker -= global.MOVE_BACKWARD_RIGHT[cell->get_type()];
    checkArray[4] = checker -= global.MOVE_BACKWARD[cell->get_type()];
    checkArray[5] = checker -= global.MOVE_BACKWARD_LEFT[cell->get_type()];
    checkArray[6] = checker -= global.MOVE_LEFT[cell->get_type()];
    checkArray[7] = checker -= global.MOVE_FORWARD_LEFT[cell->get_type()];

    for (int i = 0; i < 8; i++)
    {
        // Is the roll greater than or equal to the checkArray value?
        if (roll >= checkArray[i])
        {
            // Is there room to move in the desired direction?
            if (!space_availiable(move(direction, cell->position, cell->get_type()),
                                  cell->get_type()))
                // No...
                return false;

            //Yes...

            new_location = move (direction, cell->position,cell->get_type());
            if (new_location == cell->position)
                return false;
            cell->position = new_location;
            return true;
        }
        // Increment the direction and try again
        direction++;
    }
    // Guard by returning in case the defined movement constants do not sum to 100
    return false;
}

pair<unsigned int, unsigned int> Simulator::move(Direction direction, pair<unsigned int,
                                                 unsigned int> location, CellType type)
{
    //<-----------------------------------------Make Move------------------------------------------>
    // 1) Store the original location for later use
    // 2) Based on the absolute direction, adjust the location coordinates
    //    Note: some directions are guarded to protect against signed arithmitic. If a coordinate is
    //          to subtracted is not zero, cell move. Otherwise, the original coordinates are
    //          returned.
    //<-------------------------------------------------------------------------------------------->

    // Store the original location for later use
    pair<unsigned int, unsigned int> original = location;

    // Based on the absolute direction, adjust the location coordinates
    switch (direction)
    {
    case UP:
            location.second++;
        break;
    case UP_RIGHT:
        location.second++;
        location.first++;
        break;
    case RIGHT:
        location.first++;
        break;
    case DOWN_RIGHT:
        if (location.second == 0)
            return original;
        location.first++;
        location.second--;
        break;
    case DOWN:
        if (location.second == 0)
            return original;
        location.second--;
        break;
    case DOWN_LEFT:
        if (location.first == 0 || location.second == 0)
            return original;
        location.first--;
        location.second--;
        break;
    case LEFT:
        if (location.first == 0)
            return original;
        location.first--;
        break;
    case UP_LEFT:
        if (location.first == 0)
            return original;
        location.first--;
        location.second++;
        break;
    }

    //<--------------------------------------Check Validity---------------------------------------->
    // 1) According to the distance formula, is the new location within the environment?
    //    No  --> Do not move, return the original coordinates
    //    Yes --> The move is valid, return the new location
    //<-------------------------------------------------------------------------------------------->

    pair<unsigned int, unsigned int> center;
    center.first = global.ENVIRONMENT_RADIUS;
    center.second = global.ENVIRONMENT_RADIUS;

    // ADDED BY DEBJIT RAY FOR 'ALL' CELL CONFINEMENT WITHIN THEIR RESPECTIVE EXPANSE BOTH LOWER AND UPPER*****
    if (type != ELONGATED_SPERMATIDS)
    {
        if (distance(location, center) > global.INITIAL_MAX[type] || distance(location, center) < global.INITIAL_MIN[type] || distance(location, center) > global.ENVIRONMENT_RADIUS)
        {
            return original;
        }
        return location;
    } // *****
    else {
        if (distance(location, center) > global.ENVIRONMENT_RADIUS)
            return original;
        return location;
    }

}

Cell* Simulator::proliferate(Cell* mother)
{
    //<------------------------------Determine Proliferation Location------------------------------>
    // Determine a random direction to proliferate a new cell in
    //
    // 1) Generate a random number in the range of 0 and 7 to set the direction with
    // 2) Increment the direction according to the random number we generated and the cell's forward
    //    direction
    // 3) Is there space availiable in the new location and is it valid?
    //    Yes --> 1) Assign the daughter cell its new location and return it
    //    No  --> 1) Try increment the direction(+1) and try again
    //
    // 4) Is there no space to proliferate the cell in all adjacent spaces?
    //    Yes --> 1) report failure with a NULL address return
    //<-------------------------------------------------------------------------------------------->

    pair<unsigned int, unsigned int> daughter_location;
    bool spot_found = false;

    // Generate a random number in the range of 0 and 7 to set the direction with
    int roll = rand() % 8;

    // Increment the direction
    Direction forward = this->find_forward(mother->position, mother->get_type());
    for (int i = 0; i < roll; i++)
    forward++;

    for (int i = 0; i < 8; i++)
    {
        daughter_location = this->move(forward, mother->position, mother->get_type() );

        // Is there space availiable in the new location and is it valid?
        if (daughter_location != mother->position &&
                this->space_availiable(daughter_location, mother->get_type()))
            // Yes...
            spot_found = true;
        if (spot_found)
            break;
        // No...
        forward++;
    }

    // Is there no space to proliferate the cell in all adjacent spaces?
    //if(!spot_found)   // DELETED BY DEBJIT RAY (SEC CAN FORM 2 ROUNDS IN ONE CELL)
    if(!spot_found && mother->get_type()!=SECOND_SPERMATOCTYE ) //CHANGED BY DEBJIT RAY (SEC CAN FORM 2 ROUNDS IN ONE GRID)

        // Yes...
        return NULL;

    //<--------------------------------------Proliferate Cell-------------------------------------->
    // 1) Generate a random number from 0 to 99 to determine how the cell will proliferate
    // 2) For each proliferation method:
    //        1) Subtract the #defined percentage chance from a comparsion variable (choiceRange)
    //        2) Is the roll greater than the modified choiceRange?
    //           No  --> 1) Check next case
    //           Yes --> Case reset both cells:
    //                      1) Create a new daughter with randomly generated behaviors
    //                      2) Create a new cell for the mother with randomly generated behaviors.
    //                         This will "reset" the mother literally as a brand new cell.
    //               --> Case differentiate both cells:
    //                      1) Increment the type
    //                      2) Perform the same algorithm as the reset cells block...
    //                      ...
    //               --> Case differentiate one cell:
    //                      1) Reset the daughter (with the original type)
    //                      2) Differentiate the mother
    //<-------------------------------------------------------------------------------------------->

    Cell* daughter = NULL;
    Cell* tmpCell;
    CellType type = mother->get_type();

    // CHANGED BY DEBJIT RAY FOR SEPARATE UNDIFF AND DIFF PROLIFERATION
    if (type == UNDIFFERENTIATED_SPERMATOGONIA || type == DIFFERENTIATED_SPERMATOGONIA) {
        if (type == UNDIFFERENTIATED_SPERMATOGONIA) {
            // Generate a random number from 1 to 100 to determine how the cell will proliferate
            roll = rand() % 100 + 1;

            // Subtract the #defined percentage chance from a comparsion variable (choiceRange)
            int choiceRange = 100 - global.PROLIFERATION_RESET_U;

            if (roll > choiceRange)
            {
                // Create a new daughter with randomly generated behaviors
                daughter = new Cell(type, daughter_location, this); // ADDED 'this' BY DEBJIT RAY TO USE THE PROPER CONSTRUCTOR
                // DECLARING A STRING TO APPEND THE MOTHER ID
                stringstream ss; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                ss << mother->id;//* ADDED BY DEBJIT RAY FOR UNIQUE ID
                daughter->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                // Create a new cell for the mother with randomly generated behaviors.
                tmpCell = new Cell(type,mother->position, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                tmpCell->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                *mother = *tmpCell;
                delete tmpCell;
                return daughter;
            }

            choiceRange -= global.PROLIFERATION_DIFFER_U;
            if (roll > choiceRange)
            {
                // Increment the type
                type++;

                stringstream ss;  //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                ss << mother->id; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                // Perform the same algorithm as the reset cells block...
                daughter = new Cell(type, daughter_location, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                daughter->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                tmpCell = new Cell(type, mother->position, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                tmpCell->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                *mother = *tmpCell;
                delete tmpCell;
                return daughter;
            }
            // MAIN LOOP FOR UNDIFF AS PROLIFERATION_SPLIT_U=100
            choiceRange -= global.PROLIFERATION_SPLIT_U;
            if (roll > choiceRange)
            {
                // Reset the daughter (with the original type)
                type++;

                stringstream ss; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                ss << mother->id;//* ADDED BY DEBJIT RAY FOR UNIQUE ID
                daughter = new Cell(type, daughter_location, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                daughter->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                // Differentiate the mother
                type--;
                tmpCell = new Cell(type, mother->position, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                tmpCell->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                *mother = *tmpCell;
                delete tmpCell;
                return daughter;
            }

            return daughter;
        }

        else {
        // Generate a random number from 1 to 100 to determine how the cell will proliferate
            roll = rand() % 100 + 1;

            // Subtract the #defined percentage chance from a comparsion variable (choiceRange)
            int choiceRange = 100 - global.PROLIFERATION_RESET_D;

            // ADDED BY DEBJIT RAY TO ADDRESS DIFFERENTIATION OF DIFF INTO 2 PRELEPS
            if(mother->differentiation==0) {
                choiceRange = 100;
            }
            // MAIN LOOP FOR DIFFERENTIATED CELLS PROLIFERATION_RESET_D=100
            if (roll > choiceRange)
            {
                // Create a new daughter with randomly generated behaviors
                daughter = new Cell(type, daughter_location, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                int id  = daughter->id; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                string parent  = daughter->parent;//* ADDED BY DEBJIT RAY FOR UNIQUE ID
                *daughter = *mother;
                daughter->id = id; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                stringstream ss;   //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                ss << mother->id;  //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                daughter->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                mother->parent = daughter->parent; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                daughter->position = daughter_location;

                //DELETED BY DEBJIT RAY
                // Create a new cell for the mother with randomly generated behaviors.
                //tmpCell = new Cell(type,mother->position, this); // CHANGED BY DEBJIT RAY
                //*mother = *tmpCell;
                //delete tmpCell;
                return daughter;
            }
            choiceRange -= global.PROLIFERATION_DIFFER_D;
            // ADDED BY DEBJIT RAY TO ADDRESS DIFFERENTIATION OF DIFF INTO 2 PRELEPS
            if(mother->differentiation==0) {
                choiceRange = 0;
            }
            if (roll > choiceRange)
            {
                // Increment the type
                type++;
                stringstream ss;    //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                ss << mother->id;   //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                // Perform the same algorithm as the reset cells block...
                daughter = new Cell(type, daughter_location, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                daughter->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                tmpCell = new Cell(type, mother->position, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                tmpCell->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                *mother = *tmpCell;
                delete tmpCell;
                return daughter;
            }

            choiceRange -= global.PROLIFERATION_SPLIT_D;
            if (roll > choiceRange)
            {

                stringstream ss; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                ss << mother->id;//* ADDED BY DEBJIT RAY FOR UNIQUE ID
                // Reset the daughter (with the original type)
                daughter = new Cell(type, daughter_location, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                daughter->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                // Differentiate the mother
                type++;
                tmpCell = new Cell(type, mother->position, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                tmpCell->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                *mother = *tmpCell;
                delete tmpCell;
                return daughter;
            }

            return daughter;
        }
    }
    // CHANGED BY DEBJIT RAY FOR DIPLOTENE AND SECONDARY PROLIFERATION
    else {
        if (type == DIPLOTENE) {
            // Generate a random number from 1 to 100 to determine how the cell will proliferate
            roll = rand() % 100 + 1;

            // Subtract the #defined percentage chance from a comparsion variable (choiceRange)
            int choiceRange = 100 - global.PROLIFERATION_RESET_DIP;

            if (roll > choiceRange)
            {
                // Create a new daughter with randomly generated behaviors
                daughter = new Cell(type, daughter_location, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                int id = daughter->id; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                string parent = daughter->parent; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                *daughter = *mother;
                daughter->id = id; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                stringstream ss;   //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                ss << mother->id;  //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                daughter->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                daughter->position = daughter_location;

                //DELETED BY DEBJIT RAY
                // Create a new cell for the mother with randomly generated behaviors.
                //tmpCell = new Cell(type,mother->position, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                //*mother = *tmpCell;
                //delete tmpCell;
                return daughter;
            }
            choiceRange -= global.PROLIFERATION_DIFFER_DIP;
            // MAIN LOOP FOR DIPLOTENE CELLS AS PROLIFERATION_DIFFER_DIP=100
            if (roll > choiceRange)
            {
                // Increment the type
                type++;

                stringstream ss; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                ss << mother->id;//* ADDED BY DEBJIT RAY FOR UNIQUE ID
                // Perform the same algorithm as the reset cells block...
                daughter = new Cell(type, daughter_location, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                daughter->parent = mother->parent + "." + ss.str();//* ADDED BY DEBJIT RAY FOR UNIQUE ID
                tmpCell = new Cell(type, mother->position, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                tmpCell->parent = mother->parent + "." +ss.str();//* ADDED BY DEBJIT RAY FOR UNIQUE ID
                *mother = *tmpCell;
                delete tmpCell;
                return daughter;
            }

            choiceRange -= global.PROLIFERATION_SPLIT_DIP;
            if (roll > choiceRange)
            {

                stringstream ss; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                ss << mother->id;//* ADDED BY DEBJIT RAY FOR UNIQUE ID
                // Reset the daughter (with the original type)
                daughter = new Cell(type, daughter_location, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                daughter->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                // Differentiate the mother
                type++;
                tmpCell = new Cell(type, mother->position, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                tmpCell->parent = mother->parent + "." +ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                *mother = *tmpCell;
                delete tmpCell;
                return daughter;
            }

            return daughter;
        }

        else {
            // Generate a random number from 1 to 100 to determine how the cell will proliferate
            roll = rand() % 100 + 1;

            // Subtract the #defined percentage chance from a comparsion variable (choiceRange)
            int choiceRange = 100 - global.PROLIFERATION_RESET_SECO;

            if (roll > choiceRange)
            {
                // Create a new daughter with randomly generated behaviors
                daughter = new Cell(type, daughter_location, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                int id  = daughter->id; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                string parent = daughter->parent; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                *daughter = *mother;
                daughter->id =  id; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                stringstream ss;    //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                ss << mother->id;   //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                daughter->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                daughter->position = daughter_location;

                //DELETED BY DEBJIT RAY
                // Create a new cell for the mother with randomly generated behaviors.
                //tmpCell = new Cell(type,mother->position, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                //*mother = *tmpCell;
                //delete tmpCell;
                return daughter;
            }
            choiceRange -= global.PROLIFERATION_DIFFER_SECO;
            // MAIN LOOP FOR SECONDARY CELLS AS PROLIFERATION_DIFFER_SECO=100
            if (roll > choiceRange)
            {
                // Increment the type
                type++;
                stringstream ss; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                ss << mother->id; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                // Perform the same algorithm as the reset cells block...
                //daughter = new Cell(type, daughter_location, this); // DELETED BY DEBJIT RAY FOR PROLIFERATED ROUNDS IN SAME GRID
                daughter = new Cell(type, mother->position, this); // CHANGED BY DEBJIT RAY FOR PROLIFERATED ROUNDS IN SAME GRID
                tmpCell = new Cell(type, mother->position, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                daughter->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                tmpCell->parent = mother->parent + "." + ss.str(); //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                *mother = *tmpCell;
                delete tmpCell;
                return daughter;
            }

            choiceRange -= global.PROLIFERATION_SPLIT_SECO;
            if (roll > choiceRange)
            {
                // Reset the daughter (with the original type)
                daughter = new Cell(type, daughter_location, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR


                // Differentiate the mother
                type++;
                stringstream ss; //* ADDED BY DEBJIT RAY FOR UNIQUE ID
                ss << mother->id;//* ADDED BY DEBJIT RAY FOR UNIQUE ID
                tmpCell = new Cell(type, mother->position, this); // ADDED 'this' BY DEBJIT RAY USE THE PROPER CONSTRUCTOR
                daughter->parent = mother->parent + "." + ss.str();//* ADDED BY DEBJIT RAY FOR UNIQUE ID
                tmpCell->parent = mother->parent + "." + ss.str();//* ADDED BY DEBJIT RAY FOR UNIQUE ID
                *mother = *tmpCell;
                delete tmpCell;
                return daughter;
            }

            return daughter;
        }
    }
}

Direction Simulator::find_forward(pair<unsigned int, unsigned int> position, CellType type)
{
    //<-------------------------------------Determine Center--------------------------------------->
    // We will use the distance formula to calculate the closest square to the designated "center"
    // for all squares surrounding the cell. We will need to redesignate the center for
    // undifferentiated spermatogonia as all cells except these are attracted to the middle of the
    // environment.
    //
    // 1) Is the cell an undifferentiated spermatogonia?
    //    No  --> 1) Set the cell's center to the environment's center.
    //    Yes --> 1) Find the closest blood vessel and assign it to be the cell's center
    //<-------------------------------------------------------------------------------------------->

    pair<unsigned int, unsigned int> center;
    pair<unsigned int, unsigned int> forward_pos = position;
    Direction forward_dir = UP;



    // Is the cell an undifferentiated spermatogonia?
    if (type != UNDIFFERENTIATED_SPERMATOGONIA)
    {
        // No...
        center.first = global.ENVIRONMENT_RADIUS;
        center.second = global.ENVIRONMENT_RADIUS;
    }
    else
    {
        // Yes...
        double closest_distance = distance(this->blood_vessels[0],position);
        int closest = 0;
        for (int i = 1; i < 5; i++)
        {
            if (distance(this->blood_vessels[i],position) < closest_distance)
            {
                closest = i;
                closest_distance = distance(this->blood_vessels[i],position);
            }
        }
        center = this->blood_vessels[closest];
    }

    //<-----------------------------------Find Forward Direction----------------------------------->
    // For each possible direction:
    //      1) Will moving this direction put us on negative coordinates?
    //         Yes --> 1) Skip it
    //         No  --> 1) Temporarily move that direction
    //                 2) Is this the smallest distance to the center we have found?
    //                    Yes --> Store it and the current direction as such
    //      2) Return the direction of smallest distance we have found
    //<-------------------------------------------------------------------------------------------->

    // Will moving this direction put us on negative coordinates?
    if (position.second != 0)
    {
        // Temporarily move that direction
        position.second--;

        // Is this the smallest distance to the center we have found?
        if (distance(position,center) < distance(forward_pos,center))
        {
            // Store it and the current direction as such
            forward_pos = position;
            forward_dir = DOWN;
        }
        position.first++;
        if (distance(position,center) < distance(forward_pos,center))
        {
            forward_pos = position;
            forward_dir = DOWN_RIGHT;
        }
        position.second++;
    }
    else
        position.first++;
    if (distance(position,center) < distance(forward_pos,center))
    {
        forward_pos = position;
        forward_dir = RIGHT;
    }
    position.second++;
    if (distance(position,center) < distance(forward_pos,center))
    {
        forward_pos = position;
        forward_dir = UP_RIGHT;
    }
    position.first--;
    if (distance(position,center) < distance(forward_pos,center))
    {
        forward_pos = position;
        forward_dir = UP;
    }
    if (position.first != 0)
    {
        position.first--;
        if (distance(position,center) < distance(forward_pos,center))
        {
            forward_pos = position;
            forward_dir = UP_LEFT;
        }
        position.second--;
        if (distance(position,center) < distance(forward_pos,center))
        {
            forward_pos = position;
            forward_dir = LEFT;
        }
        if (position.second != 0)
        {
            position.second--;
            if (distance(position,center) < distance(forward_pos,center))
            {
                forward_pos = position;
                forward_dir = DOWN_LEFT;
            }
        }
    }
    // Return the direction of smallest distance we have found
    return forward_dir;
}

bool Simulator::space_availiable(pair<unsigned int, unsigned int> location,
                                 CellType type)
{
    //<---------------------------------Space Availiable------------------------------------------->
    // 1) Ensure we are not conflicting with a Sertoli cell nucleus
    // 2) Get the hash of the passed location so that we can access it in our multimap
    // 4) Is there no entry for the hash?
    //    Yes --> 1) Return true, this space is empty
    // 5) Are we moving any type besides round or elongated spermatids?
    //    Yes --> 1) Return false, our desired type needs an entire space
    // 6) Are there already two or more cells in this spot?
    //    Yes --> 1) Return false, this space is full
    // 7) Is the cell in the space we're looking at a round or elongated spermatid?
    //    Yes --> 1) There is space, return true. There is only one element with this hash and our
    //               desired type and present type both allow for two cells in the same square.
    // 8) We could not find space, return false
    //<-------------------------------------------------------------------------------------------->

    // Ensure we are not conflicting with a Sertoli cell nucleus
    for (int i = 0; i < global.SERTOLI_NUMBER; i++)
    {
        if (location == this->sertoli[i])
            return false;
    }

    // Get the hash of the passed location so that we can access it in our multimap
    unsigned long long hash = h(location.first, location.second);

    // Is there no entry for the hash?
    if (this->population.count(hash) == 0)
        return true;

    // Are we moving any type besides round or elongated spermatids?
    if (type != ROUND_SPERMATIDS && type != ELONGATED_SPERMATIDS)
        return false;

    // Are there already two or more cells in this spot?
    if (this->population.count(hash) > 1)
        return false;

    // Is the cell in the space we're looking at a round or elongated spermatid?
    multimap<unsigned long long, Cell*>::iterator it = this->population.find(hash);
    if (it->second->get_type() == ROUND_SPERMATIDS ||
            it->second->get_type() == ELONGATED_SPERMATIDS)
        return true;

    // We could not find space, return false
    return false;
}

unsigned long long Simulator::h(unsigned int x, unsigned int y)
{
    return (x+y)*(x+y+1)/2+y;
}

double Simulator::distance(pair<unsigned int, unsigned int> point1,
                           pair<unsigned int, unsigned int> point2)
{
    return sqrt((double)((point1.first - point2.first)*
                         (point1.first - point2.first) +
                         (point1.second - point2.second)*
                         (point1.second - point2.second)));
}

void Simulator::update_data( )
{
    //<---------------------------Calculate Average Positions-------------------------------------->
    // 1) Iterate through the population and add each cell's distance from the center to the
    //    to the appropriate element in the averageDistanceCenter array and distanceCenter array
    // 2) Divide the totals in the averageDistanceCenter array elements by the appropriate cell
    //    type population
    //<-------------------------------------------------------------------------------------------->
    multimap<unsigned long long, Cell*>::iterator it;
    pair<unsigned int, unsigned int> center (global.ENVIRONMENT_RADIUS,
                                             global.ENVIRONMENT_RADIUS);
    double distance = 0;
    for (int i = 0; i < 10; i++)
    {
        this->averageDistanceCenter[i] = 0;
        this->distanceCenter[i].clear();
        this->populationSize[i] = 0;
    }

    // add each cell's distance from the center to the to the appropriate element in the array.
    for (it = this->population.begin(); it != this->population.end(); it++)
    {
        distance = this->distance(it->second->position, center);
        this->averageDistanceCenter[it->second->get_type()] += distance;
        this->distanceCenter[it->second->get_type()].push_back(distance);
        this->populationSize[it->second->get_type()]++;
    }
    // Divide the totals in the array by the appropriate cell type population
    for (int i = 0; i < 10; i++)
    {
        if (this->populationSize[i])
            this->averageDistanceCenter[i] /= this->populationSize[i];
        else this->averageDistanceCenter[i] = 0;
    }
}

// FUNCTION ADDED BY DEBJIT RAY FOR COUNTING CELL POPULATION, COUNTER TO RETURN A PARTICULAR CELL TYPE POPULATION SIZE *****
double Simulator::counter(int x)
{
    update_data();
    return this->populationSize[x];
}
//*****
