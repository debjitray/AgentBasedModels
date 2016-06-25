#ifndef GERMCELLSIMULATOR_H
#define GERMCELLSIMULATOR_H

#define _CRT_SECURE_NO_WARNINGS

// Qt libaries
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QWheelEvent>
#include <QVector>
#include <QFileDialog>
#include <QMessageBox>

#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "simulator.h"

using namespace std;

namespace Ui {
class GermCellSimulator;
}

class GermCellSimulator : public QMainWindow
{
    //Macro: Marks the class as a Qt Object
    Q_OBJECT
    
public:
    /********************************************************************
    Default constructor

    @param *parent
        A pointer to the Qt widget this object belongs to
    ********************************************************************/
    explicit GermCellSimulator(bool *valid, QWidget *parent = 0);

    /********************************************************************
    Default deconstructor
    ********************************************************************/
    ~GermCellSimulator();

public slots:
    /********************************************************************
    Runs one simulation step and updates the graphic visualization
    objects accordingly.
    ********************************************************************/
    void update();

    /********************************************************************
    Recieves a string from the calling function and saves the current
    simulation data as a .csv file (child of saveCSV())

    @filename
        A string containing the file location to save to
    @return
        Indicates whether or not the save was successful
    ********************************************************************/
    bool saveCSV_inner(string filename);

    /********************************************************************
    Recieves a string from the calling function and saves the current
    simulation position data as a .csv file (child of savePosData())

    @filename
        A string containing the file location to save to
    @return
        Indicates whether or not the save was successful
    ********************************************************************/
    bool savePosData_inner(string usrFileName);

    /********************************************************************
    Recieves a string from the calling function and saves a snapshot of
    the current simulation as a .csv file (child of saveData())

    @filename
        A string containing the file location to save to
    @return
        Indicates whether or not the save was successful
    ********************************************************************/
    bool saveData_inner(string filename);

    /********************************************************************
    Recieves a string from the calling function and saves the errors of
    the current simulation as a .csv file (child of saveErrors())

    @filename
        A string containing the file location to save to
    @return
        Indicates whether or not the save was successful
    ********************************************************************/
    bool saveErrors_inner(string filename);

    /********************************************************************
    Initialize the simulation object and ready GUI objects for running the
    simulation.
    ********************************************************************/
    void loadSimulation();

    /********************************************************************
    Saves a png of the graphics view to the designated save location
    as per the picFileName variable with the current timestamp appended
    ********************************************************************/
    void savePic();

private slots:
    /********************************************************************
    Begins the simulation and updates the appropriate GUI objects
    ********************************************************************/
    void startSimulation();

    /********************************************************************
    Stops the simulation and updates the appropriate GUI objects
    ********************************************************************/
    void stopSimulation();

    /********************************************************************
    Begins saving pictures on every time step and updates the
    appropriate GUI objects
    ********************************************************************/
    void startRecording();

    /********************************************************************
    Stops saving pictures on every time step and updates the appropriate
    GUI objects
    ********************************************************************/
    void stopRecording();

    /********************************************************************
    Resizes the visualization graphics.

    @param value
        The value to compare against for zooming actions. If the value
        is greater than the old one, the visualization is made larger.
        If the value is lesser, than the visualization is made smaller.
    ********************************************************************/
    void changeZoom(int value);

    /********************************************************************
    Changes the rate that the simulation object makes steps.

    @param value
        The value to compare against for speed change actions. If the
        value is greater than the old one, the visualization is made
        faster. If the value is lesser, than the visualization is made
        slower.
    ********************************************************************/
    void changeSpeed(int value);

    /********************************************************************
    Retrieves a filename from the user and saves the current simulation
    data as a .csv file
    ********************************************************************/
    void saveCSV();

    /********************************************************************
    Saves the current simulation data entirely so that it can be loaded
    later
    ********************************************************************/
    void saveData();

    /********************************************************************
    Saves the simulation data containing the distances to the center of
    every cell per cell type. Saves output as 10 different .csv files,
    one for each cell type
    ********************************************************************/
    void savePosData();

    /********************************************************************
    Saves a list of the errors and/or warnings generated over the
    lifetime of the current simulator
    ********************************************************************/
    void saveErrors();

public:
    // File name prefix from which to store simulation pictures
    string picFileName;

private:
    /********************************************************************
    Overloads inherited QWidget::wheelEvent(). Handles mouse wheel
    events.

    @param *event
        The captured wheel event
    ********************************************************************/
    void wheelEvent(QWheelEvent *event);

    /********************************************************************
    Overloads inherited QWidget::eventFilter(). Handles unspecialized
    events.

    @param *object
        The object being effected by the event
    @param *event
        The caputured event
    ********************************************************************/
    bool eventFilter(QObject *object, QEvent *event);

    /********************************************************************
    Initializes default colors to the cellColors array
    ********************************************************************/
    void initializeColors();

    /********************************************************************
    Updates visual graphics
    ********************************************************************/
    void updateGraphics();

    //Pointer used for accessing objects generated with Qt Designer
    Ui::GermCellSimulator *ui;

    //The graphics model used to contain the simulation graphics
    QGraphicsScene simModel;

    //Timer used for stepping through the simulation
    QTimer timer;

    //Pointer to the simulation object (see simulator.h)
    Simulator *sim;

    //Array of colors for painting germ cells. Indexed with CellType enum.
    QColor cellColors[10];

    //Data vectors. Vectors are parallel and indexed using the CellType enum
    QVector< vector< double > > indiv_posData[10];
    QVector<double> popData[10];
    QVector<double> posData[10];
    QVector<double> timeData;

    // Storage for Flags which have been handled
    queue<Flag> handledFlags;

    //Counter for allowing flags to be seen for a period of time
    int flag_time;
};

#endif // GERMCELLSIMULATOR_H
