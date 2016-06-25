#include "cell.h"

// ADDED BY DEBJIT RAY FOR UNIQUE ID
int Cell::cnt;

Cell::Cell(CellType type, pair<unsigned int, unsigned int> location)
{
    this->type = type;
    this->position = location;

    // Assign cell behaviors based on the cell type
    this->assign_apoptosis( );
    this->assign_proliferation( );
    this->assign_move( );
    this->assign_differentiation( );

    // ADDED BY DEBJIT RAY FOR UNIQUE ID
    this->id = ++this->cnt;
    this->parent = "P";

}

// FUNCTION ADDED BY DEBJIT RAY FOR THE UNDIFF PROLIFERATION DEPENDING ON PRELEP, CONSTRUCTOR SAME AS ABOVE CELL CONSTRUCTOR BUT HAS 3 PARAMETERS
Cell::Cell(CellType type, pair<unsigned int, unsigned int> location, Simulator *s)
{
    this->type = type;
    this->position = location;
    this->sim = s;   // ADDED BY DEBJIT RAY, DIFFERENT FROM ABOVE, PASSING THE INSTANCE FOR THE SIMULATOR

    // Assign cell behaviors based on the cell type
    this->assign_apoptosis( );
    this->assign_proliferation( );
    this->assign_move( );
    this->assign_differentiation( );

    // ADDED BY DEBJIT FOR UNIQUE ID
    this->id = ++this->cnt;
    this->parent = "P";

}


void Cell::step(bool *proliferate, bool *die, bool *moved, bool *grow)
{
    // Housekeeping: set the boolean pointer arguments to a default of false
    *proliferate = false;
    *die = false;
    *moved = false;
    *grow = false;

    // Decrement the class member variables storing the cell behavior timers. Each time step
    // corresponds to one hour of real time
    //  ADDED BY DEBJIT RAY FOR THE UNDIFF PROLIFERATION DEPENDING ON PRELEPTOTENE CELL NUMBER *****
    if (this->proliferation != -1) {
        if (type == UNDIFFERENTIATED_SPERMATOGONIA) {
            // LATEST ADDED BY DEBJIT RAY
            if (this->proliferation >global.INITIALUNDIFF_PROLIFERATION_MAX || global.PROLIFERATION_MAX[0]==global.INITIALUNDIFF_PROLIFERATION_MAX) {
                if (this->sim->counter(2)>global.CUTOFF && this->proliferation <  (global.PROLIFERATION_MAX[0]-global.DIFFERENTIATION_MAX[2]) ) {
                    this->proliferation = 0;
                 }
                else {
                    this->proliferation--;
                }
            }
            else {
                this->proliferation--;
            }
        }
        else {
              this->proliferation--;
        }// *****
    }
    //this->proliferation--;   // DELETED BY DEBJIT RAY, ALREADY COVERED IN THE ELSE LOOP ABOVE
    this->apoptosis--;
    this->differentiation--;
    this->move--;

    //---------------
    // Cell Actions
    //---------------
    // Proliferation: Has our proliferation timer reached 0?
    if (this->proliferation == 0)
    {
        // Yes: Set the proliferate pointer to reflect a proliferation
        // Assign a new proliferation timer value based on cell type
        *proliferate = true;
        this->assign_proliferation( );
    }
    // Apoptosis: Has our apoptosis timer reached 0?
    if (this->apoptosis == 0)
        *die = true;

    // Movement: Has our move timer reached 0?
    if (this->move == 0)
    {
        *moved = true;
        // Assign a new move timer based on cell type
        this->assign_move();
    }
    // Differentiation: Has our differentiation timer reached 0?
    if (this->differentiation == 0 && this-> type !=DIFFERENTIATED_SPERMATOGONIA) // CHANGED BY DEBJIT RAY AS DIFF PRODUCES 2 PRELEPS
    {
        if (this->type == ELONGATED_SPERMATIDS)

            // Handle elongated spermatids, but do not act upon them
            this->differentiation++;
        else
        {
            *grow = true;
            // Evolve the cell to its next stage
            this->evolve( );
        }
    }
    // ADDED BY DEBJIT RAY AS DIFF DIFFERENTIATES INTO 2 PRELEPS *****
	if (this->differentiation==0 && this->type == DIFFERENTIATED_SPERMATOGONIA) {
        *proliferate = true;
        this->assign_proliferation( );
    } //*****
}

void Cell::evolve( )
{
    //Increment the cell type only if it is not already fully developed
    if (this->type != ELONGATED_SPERMATIDS)
	this->type++;

    //Assign new cell behaviors based on new cell type.
    this->assign_apoptosis( );
    this->assign_proliferation( );
    this->assign_differentiation( );
    this->assign_move( );
}

void Cell::assign_apoptosis( )
{
    int detectZero;

    //<-------------------------Assign Apoptosis Based on Cell Type-------------------------------->
    // 1) Initially assign the apoptosis value to the defined min.
    // 2) Are the defined min and defined max the same?
    //    Note: This check protects from division by 0 when performing the modulus operation.
    // No --> 1) Assign a random value between the defined min and max
    //<-------------------------------------------------------------------------------------------->

    int type_int = static_cast<int>(this->type);

    // Initially assign the apoptosis value to the defined min.
    this->apoptosis = global.APOPTOSIS_MIN[type_int];

    // Are the defined min and defined max the same?
    detectZero = global.APOPTOSIS_MAX[type_int] - global.APOPTOSIS_MIN[type_int];
    if (detectZero)

        // Assign a random value between the defined min and max
        this->apoptosis = rand() % detectZero + global.APOPTOSIS_MIN[type_int];

}

void Cell::assign_proliferation( )
{
    int detectZero = 0;

    int type_int = static_cast<int>(this->type);

    //<-------------------Assign a Proliferation Value Based on Type------------------------------->
    // Only Undifferentiated and Differentiated Spermatogonia can proliferate.
    // 1) Is the cell one of these types?
    //    Yes --> 1) Initially assign the proliferation value to the defined min
    //            2) Do we have a different defined max than defined min?
    //               Note: This check protects from dividing by 0 during the modulus operation.
    //            Yes --> 1) Assign a random value between the max and min defined values
    //    No  --> 1) Assign a default value of 0 for the cell's proliferation
    //<-------------------------------------------------------------------------------------------->

    // Is the cell of type Undifferentiated or Differentiated Spermatogonia?
    if (type_int == 6 || type_int == 7 || type_int < 2)  // CHANGED BY DEBJIT RAY FOR INCLUDING DIPLOTENE AND SECONDARY PROLIFERATION
    {
        // LATEST ADDED BY DEBJIT RAY FOR INITIAL UNDIFF PROLIFERATION (WE ENTER THIS LOOP AT INITIAL CONDITION)***
        if (type_int == 0 && this->sim->counter(2) == 0 && this->sim->counter(5) == 0 && this->sim->counter(9) == 0) {
            this->proliferation = global.INITIALUNDIFF_PROLIFERATION_MIN;
            detectZero = global.INITIALUNDIFF_PROLIFERATION_MAX - global.INITIALUNDIFF_PROLIFERATION_MIN;
            if (detectZero)
                this->proliferation = rand() % detectZero + global.INITIALUNDIFF_PROLIFERATION_MIN;
        }
        // ***
        else {
            this->proliferation = global.PROLIFERATION_MIN[type_int];
            detectZero = global.PROLIFERATION_MAX[type_int] - global.PROLIFERATION_MIN[type_int];
            if (detectZero)
                this->proliferation = rand() % detectZero + global.PROLIFERATION_MIN[type_int];
        }
    }
    else
        this->proliferation = -1;
}

void Cell::assign_differentiation( )
{
    int detectZero = 0;

    int type_int = static_cast<int>(this->type);

    //<------------------------Assign Differentiation Based on Cell Type--------------------------->
    // 1) Initially assign the differentiation to the min defined value
    // 2) Are the defined min and defined max values different?
    //    Note: This check protects from dividing by 0 during the modulus operation.
    // Yes --> 1) Assign a random value between the defined min and max values
    //<-------------------------------------------------------------------------------------------->

    // Initially assign the differentation to the min defined value
    this->differentiation = global.DIFFERENTIATION_MIN[type_int];

    // Are the defined min and defined max values different?
    detectZero = global.DIFFERENTIATION_MAX[type_int] - global.DIFFERENTIATION_MIN[type_int];
    if (detectZero) {// MEANS DETECTZERO not ZERO
        this->differentiation = rand() % detectZero + global.DIFFERENTIATION_MIN[type_int];
    }
    // ADDED BY DEBJIT RAY TO TAKE CARE OF A4 INITIAL DIFF CELL DIFFERENTIATION *****
    // LATEST ADDED BY DEBJIT RAY FOR A4 INITIAL DIFF DIFFERENTIATION
    if (type == DIFFERENTIATED_SPERMATOGONIA && this->sim->counter(5) == global.INITIAL_NUM[5] && this->sim->counter(9) == global.INITIAL_NUM[9]) {
        this->differentiation = global.INITIALDIFF_DIFFERENTIATION_MIN;
        detectZero = global.INITIALDIFF_DIFFERENTIATION_MAX - global.INITIALDIFF_DIFFERENTIATION_MIN;
        if (detectZero) {// MEANS DETECTZERO not ZERO
            this->differentiation = rand() % detectZero + global.INITIALDIFF_DIFFERENTIATION_MIN;
        }
    } //*****
}

void Cell::assign_move()
{
    int detectZero = 0;

    int type_int = static_cast<int>(this->type);

    //<-------------------------Assign Movement Timer Based on Cell Type--------------------------->
    // 1) Initially assign the move timer to the min defined value
    // 2) Is the min defined value the same as the max defined value?
    //    Note: This check is done to protect from division by 0 during the modulus operation.
    // Yes --> 1) Assign a random number between the min and max defined values
    //<-------------------------------------------------------------------------------------------->

    // Initially assign the move timer to the min defined value
    this->move = global.SPEED_MIN[type_int];

    // Is the min defined value the same as the max defined value?
    detectZero = global.SPEED_MAX[type_int] - global.SPEED_MIN[type_int];
    if (detectZero)

        // Assign a random number between the min and max defined values
        this->move = rand() % detectZero + global.SPEED_MIN[type_int];
}

CellType Cell::get_type()
{
    return this->type;
}

void Cell::operator=(Cell* rhs)
{
    // Copy all elements of rhs to the current cell
    this->apoptosis = rhs->apoptosis;
    this->differentiation = rhs->differentiation;
    this->move = rhs->move;
    this->position = rhs->position;
    this->proliferation = rhs->proliferation;
    this->type = rhs->type;
}

void Cell::deproliferate()
{
    // Cause the cell to proliferate on the next turn.
    this->proliferation = 1;

}

void Cell::demove()
{
    // Cause the cell to move on the next turn.
    this->move = 1;
}
