#ifndef GLOBALCONSTRUCTS_H
#define GLOBALCONSTRUCTS_H

#include <string>

using namespace std;

typedef enum
{
    UP,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT,
    LEFT,
    UP_LEFT
} Direction;

/********************************************************************
operator++ overload for the Direction enumeration. Increments the
Direction variable by one.

@param direction
    The Direction object to be incremented
********************************************************************/
inline void operator++(Direction &direction, int)
{
    const int i = static_cast<int>(direction);
    direction = static_cast<Direction>((i + 1) % 8);
}

typedef enum
{
    UNDIFFERENTIATED_SPERMATOGONIA, //Defined as 0
    DIFFERENTIATED_SPERMATOGONIA,   //Defined as 1
    PRELEPTOTENE,
    LEPTOTENE,
    ZYGOTENE,
    PACHYTENE,                      //...
    DIPLOTENE,
    SECOND_SPERMATOCTYE,
    ROUND_SPERMATIDS,               //Defined as 8
    ELONGATED_SPERMATIDS            //Defined as 9
} CellType;

/********************************************************************
operator-- overload for the CellType enumeration. Decrements the
CellType variable by one type by order of development.

@param celltype
    The CellType object to be decremented
********************************************************************/
inline void operator--(CellType &celltype, int)
{
    const int i = static_cast<int>(celltype);
    if (i == 0)
    celltype = static_cast<CellType>(9);
    celltype = static_cast<CellType>(i - 1);
}

/********************************************************************
operator++ overload for the CellType enumeration. Increments the
CellType variable by one type by order of development.

@param celltype
    The CellType object to be incremented
********************************************************************/
inline void operator++(CellType &celltype, int)
{
    const int i = static_cast<int>(celltype);
    if (i == 9)
	celltype = static_cast<CellType>(0);
    celltype = static_cast<CellType>(i + 1);
}

/********************************************************************
Converts the CellType enumeration to a string.

@param type
    The CellType object to be converted to a string
@return
    The converted string
********************************************************************/
inline string toString(CellType type)
{
    string typeStr = "";
    switch (type)
    {
    case UNDIFFERENTIATED_SPERMATOGONIA:
        typeStr = "Undifferentiated Spermatogonia";
        break;
    case DIFFERENTIATED_SPERMATOGONIA:
        typeStr = "Differentiated Spermatogonia";
        break;
    case PRELEPTOTENE:
        typeStr = "Preleptotene";
        break;
    case LEPTOTENE:
        typeStr = "Leptotene";
        break;
    case ZYGOTENE:
        typeStr = "Zygotene";
        break;
    case PACHYTENE:
        typeStr = "Pachytene";
        break;
    case DIPLOTENE:
        typeStr = "Diplotene";
        break;
    case SECOND_SPERMATOCTYE:
        typeStr = "Second Spermatocyte";
        break;
    case ROUND_SPERMATIDS:
        typeStr = "Round Spermatid";
        break;
    case ELONGATED_SPERMATIDS:
        typeStr = "Elongated Spermatid";
        break;
    }
    return typeStr;
}

/********************************************************************
Converts the Direction enumeration to a string.

@param type
    The Direction object to be converted to a string
@return
    The converted string
********************************************************************/
inline string toString(Direction dir)
{
    string dirStr = "";
    switch (dir)
    {
    case UP:
        dirStr = "Up";
        break;
    case UP_RIGHT:
        dirStr = "Up Right";
        break;
    case RIGHT:
        dirStr = "Right";
        break;
    case DOWN_RIGHT:
        dirStr = "Down Right";
        break;
    case DOWN:
        dirStr = "Down";
        break;
    case DOWN_LEFT:
        dirStr = "Down Left";
        break;
    case LEFT:
        dirStr = "Left";
        break;
    case UP_LEFT:
        dirStr = "Up Left";
        break;
    }
    return dirStr;
}

typedef enum
{
    ILLEGAL_CELL_LUMEN_AREA,
    INITIAL_EXCEEDS_MAX,
    INITIAL_NOT_ENOUGH_SPACE
} FlagType;

struct Flag
{
    FlagType type;
    string message;
    int time;
};

#endif // GLOBALCONSTRUCTS_H
