#include "germcellsimulator.h"
#include "ui_germcellsimulator.h"

GermCellSimulator::GermCellSimulator(bool *valid, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GermCellSimulator)
{
    srand(time(NULL));
    this->sim = NULL;

    // Setup default settings set by Qt Designer
    ui->setupUi(this);

    // Initialize global variables
    if (!initGlobals())
    {
        string message = "Please place the parameters.txt file in the same directory as the ";
        message.append("executable for this program");
        QMessageBox msgBox(QMessageBox::Warning, tr("Parameter Read Error"), tr(message.c_str()));
        msgBox.exec();
        *valid = false;
        return;
    }

    this->sim = new Simulator( );

    // Install an event filter for the Graphics View. This will allow us to change which object
    // catches and handles events. See eventFilter() and wheelEvent().
    ui->graphicsViewVisual->viewport()->installEventFilter(this);

    ui->graphicsViewVisual->setScene(&this->simModel);
    ui->graphicsViewVisual->setBackgroundBrush(QBrush(QColor("Black")));

    this->initializeColors();

    // Connect the timer's timeout signal with the update() slot. This will cause the update() slot
    // function to be called on each timer "tick".
    connect(&this->timer,SIGNAL(timeout()),this, SLOT(update()));

    this->picFileName = "";

    this->flag_time = 0;
    *valid = true;
}

GermCellSimulator::~GermCellSimulator()
{
    if (this->sim != NULL)
        delete this->sim;
    delete ui;
}

void GermCellSimulator::loadSimulation()
{
    this->timer.stop();
    ui->pushButtonPause->setEnabled(false);
    this->stopRecording();
    this->picFileName = "";
    ui->lcdNumberDay->display(0);
    ui->lcdNumberHour->display(0);

    this->sim = new Simulator( );

    // Populate the simulator
    this->sim->populate(global.INITIAL_NUM);

    this->updateGraphics();

    // Set the timer's tick speed in relation to the speed slider's value
    this->timer.setInterval(ui->horizontalSliderSpeed->value()*2);

    ui->pushButtonStart->setEnabled(true);

    // Center and fit the environment to the graphics view
    ui->graphicsViewVisual->fitInView(0, 0, global.ENVIRONMENT_RADIUS * 2 + 1,
                                      global.ENVIRONMENT_RADIUS * 2 + 1, Qt::KeepAspectRatio);

    // Set the zoom slider to default 50%
    ui->verticalSliderZoom->setValue(50);

    ui->verticalSliderZoom->setEnabled(true);
    ui->horizontalSliderSpeed->setEnabled(true);

    // Clear the graph curve data containers for 10 cell types
    // Push the initial condition onto the newly cleared vectors
    for (int i = 0; i < 10; i++)
    {
        this->popData[i].clear();
        this->posData[i].clear();
        this->indiv_posData[i].clear();
        this->popData[i].push_back(this->sim->populationSize[i]);
        this->posData[i].push_back(this->sim->averageDistanceCenter[i]);
        this->indiv_posData[i].push_back(this->sim->distanceCenter[i]);
    }
    this->timeData.clear();
    this->timeData.push_back(0);

    while (!this->handledFlags.empty())
        this->handledFlags.pop();
}

void GermCellSimulator::updateGraphics()
{
    unsigned int x, y;

    // Clear the simulation model as we'll we populate it
    this->simModel.clear();

    // Draw the grid
    //for (int i = 0; i <= ENVIRONMENT_RADIUS * 2; i++)
        //for (int n = 0; n <= ENVIRONMENT_RADIUS * 2; n++)
            //this->simModel.addRect(i, n, 1, 1, QPen(QColor("Blue")), QBrush( ));

    // Draw the environment circle. We'll make it 1 bigger than expected as the environment radius
    // is defined as largest distance the center of a square can be from the center of the circle
    // and we want to enclose the entire cell shapes (not just their centers)
    this->simModel.addEllipse(0, 0,  global.ENVIRONMENT_RADIUS * 2 + 1,
                global.ENVIRONMENT_RADIUS * 2 + 1, QPen(QColor("White")));

    // Draw the tubule circle
    this->simModel.addEllipse(global.ENVIRONMENT_RADIUS - global.TUBULE_RADIUS + .5,
                              global.ENVIRONMENT_RADIUS - global.TUBULE_RADIUS + .5,
                              global.TUBULE_RADIUS * 2, global.TUBULE_RADIUS * 2,
                              QPen(QColor("White")));

    // Draw the sertoli cells
    for (int i = 0; i < global.SERTOLI_NUMBER; i++)
        this->simModel.addRect(sim->sertoli[i].first,
                                  global.ENVIRONMENT_RADIUS * 2 - sim->sertoli[i].second, 1,
                                  1, QPen(QColor("darkGrey")),QBrush(QColor("darkGrey")));

    // Draw the blood vessels
    for (int i = 0; i < global.BLOOD_VESSEL_NUMBER; i++)
        this->simModel.addRect(sim->blood_vessels[i].first,
                               global.ENVIRONMENT_RADIUS * 2 - sim->blood_vessels[i].second, 1, 0.25,
                               QPen(QColor("Red")),QBrush(QColor("Red"))); // CHANGED BY DEBJIT RAY ,1,1 to ,1,0.25

    //<--------------------------------Draw Cells-------------------------------------------------->
    // 1) Iterate through the cell population and draw each cell
    // 2) Assign the cell's position to coordinate variables
    // 3) For each cell type:
    //      Create an ellipse at the given coordinates
    //      Use the cell type's size (converted into grid squares)
    //      Use the cell type's color per the cellColor array
    //<-------------------------------------------------------------------------------------------->

    // Iterate through the cell population
    for (multimap<unsigned long long, Cell*>::iterator it = this->sim->population.begin(); it !=
         this->sim->population.end(); it++)
    {
        // Assign the cell's position to coordinate variables
        x = it->second->position.first;
        y = global.ENVIRONMENT_RADIUS * 2 - it->second->position.second;

        int type_int = static_cast<int>(it->second->get_type());

        // For each cell type excluding Round and Elongated Spermatids:
        //      Create an ellipse at the given coordinates
        //      Use the cell type's size (converted into grid squares)
        //      Use the cell type's color per the cellColor array
        double adjustment = (double)(global.SIZE[type_int]) / (double)(global.GRID_SQUARE_SIZE);
        if (type_int < 8)
            this->simModel.addEllipse(x + (1 - adjustment) / 2, y + (1 - adjustment) / 2,
                                      adjustment, adjustment,
                                      QPen(this->cellColors[type_int]),
                                      QBrush(this->cellColors[type_int]));

        // In the case of Round and Elongated Spermatids:
        //      Does the grid square contain two cells?
        //          Yes: Is this the first cell in the range?
        //                  Yes: Shift the cell to the right
        //                   No: Simply paint the cell
        //           No: Simply paint the cell
        else
        {
            // Adjust the vertical size of the ellipse per cell type
            double vertical = adjustment;
            double vertical_pos = y + (1 - adjustment) / 2;
            if (it->second->get_type() == ELONGATED_SPERMATIDS)
            {
                vertical = 1;
                vertical_pos = y;
            }

            // Does the grid square contain two cells?
            if (this->sim->population.count(it->first) > 1)
            {
                // Is this the first cell in the range?
                if (it == this->sim->population.find(it->first))

                    //Shift the cell to the right
                    this->simModel.addEllipse(x + (1 - adjustment), vertical_pos,
                                              adjustment, vertical,
                                              QPen(this->cellColors[type_int]),
                                              QBrush(this->cellColors[type_int]));
                else

                    // Simply paint the cell
                    this->simModel.addEllipse(x, vertical_pos,
                                              adjustment, vertical,
                                              QPen(this->cellColors[type_int]),
                                              QBrush(this->cellColors[type_int]));
            }
            else

                // Simply paint the cell
                this->simModel.addEllipse(x + (1 - adjustment) / 2, vertical_pos,
                                          adjustment, vertical,
                                          QPen(this->cellColors[type_int]),
                                          QBrush(this->cellColors[type_int]));
        }
    }
}

void GermCellSimulator::initializeColors()
{
    //Initialize a unique color for each cell type
    //Indexed with CellType enum (see GlobalVariables.h)

    this->cellColors[0] = QColor("White");
    this->cellColors[1] = QColor("Yellow");
    this->cellColors[2] = QColor("Gray");
    this->cellColors[3] = QColor("Pink");
    this->cellColors[4] = QColor("Orange");
    this->cellColors[5] = QColor("Green");
    this->cellColors[6] = QColor("Red");
    this->cellColors[7] = QColor("Blue");
    this->cellColors[8] = QColor("Purple");
    this->cellColors[9] = QColor("Brown");
}

void GermCellSimulator::update()
{
    // Advance the LCD timers on the GUI interface by one hour
    if (ui->lcdNumberHour->value() == 23)
    {
        ui->lcdNumberHour->display(0);
        ui->lcdNumberDay->display(ui->lcdNumberDay->value()+1);
    }
    else
        ui->lcdNumberHour->display(ui->lcdNumberHour->value()+1);

    // The Qwt Curves work by taking a set of data parallel data points from two arrays. Push the
    // current time value onto the time vector (array).
    this->timeData.push_back(ui->lcdNumberHour->value()+ ui->lcdNumberDay->value()*24);

    // Advance the simulation by one hour
    this->sim->step();

    // Handle flags
    if (!this->sim->flags.empty())
    {
        for (list<Flag>::iterator it = this->sim->flags.begin(); it != this->sim->flags.end(); it++)
        {
            if (it->time == -1)
                it->time = this->timeData.back();
        }

        if (this->flag_time == 0)
        {
            this->flag_time = 5;
            ui->statusBar->showMessage(QString(this->sim->flags.front().message.c_str()));
            this->handledFlags.push(this->sim->flags.front());
            this->sim->flags.pop_front();
        }
        else
            this->flag_time--;
    }

    // Push an appropriate data point onto the popData and posData vectors (arrays)
    for (int i = 0; i < 10; i ++)
    {
        this->popData[i].push_back(this->sim->populationSize[i]);
        this->posData[i].push_back(this->sim->averageDistanceCenter[i]);
        this->indiv_posData[i].push_back(this->sim->distanceCenter[i]);
    }

    this->updateGraphics();

    if (ui->pushButtonRecordStop->isEnabled() && !ui->pushButtonRecordStart->isEnabled())
        this->savePic();
}

void GermCellSimulator::startSimulation()
{
    // Simulation runs off timer. Therefore to start the simulation, we start the timer.
    this->timer.start();

    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonPause->setEnabled(true);
}

void GermCellSimulator::stopSimulation()
{
    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonStart->setEnabled(true);

    // Simulation runs off timer. Therefore to stop the simulation, we stop the timer.
    this->timer.stop();
}

void GermCellSimulator::changeZoom(int value)
{
    // We are passed a value which represents the new postion of the slider
    // Is this new position value smaller than our old one?
    //      Yes: The user has indicated that they want to zoom out
    //           Zoom out by a magnitude equal to the difference in the new and old values
    //       No: The user has indicated that they want to zoom in
    //           Zoom in by a magnitude equal to the difference in the new and old values

    // Is this position value smaller than our old one?
    if (value < ui->verticalSliderZoom->value())
    {
        // Zoom out by a magnitude equal to the difference in the new and old values
        for (int i = value; i < ui->verticalSliderZoom->value(); i++)
            ui->graphicsViewVisual->scale(1/1.1, 1/1.1);
    }
    else if (value > ui->verticalSliderZoom->value())
    {
        // Zoom in by a magnitude equal to the difference in the new and old values
        for (int i = ui->verticalSliderZoom->value(); i < value; i++)
            ui->graphicsViewVisual->scale(1.1,1.1);
    }
}

void GermCellSimulator::changeSpeed(int value)
{
    //Set the timer's timeout interval (speed) to the value of the slider
    this->timer.setInterval(value);
}

void GermCellSimulator::wheelEvent(QWheelEvent *event)
{
    // Ensure that scrolling is allowed right now
    if (ui->verticalSliderZoom->isEnabled())
    {
        // If we want to zoom in, make sure that we haven't already zoomed in the maximum amount
        // according to our slider.
        if (event->delta() > 0 && ui->verticalSliderZoom->value() !=
                ui->verticalSliderZoom->maximum())
        {
            // Allow our changeZoom function to take care of the actual zooming for us
            this->changeZoom(ui->verticalSliderZoom->value()+1);

            // Update the slider to reflect our changes
            ui->verticalSliderZoom->setValue(ui->verticalSliderZoom->value()+1);
        }
        // If we want to zoom out, make sure that we haven't already zoomed out the maxiumum amount
        // according to our slider.
        else if (event->delta() < 0 && ui->verticalSliderZoom->value() !=
                 ui->verticalSliderZoom->minimum())
        {
            // Allow the changeZoom function to handle the actual zooming
            this->changeZoom(ui->verticalSliderZoom->value()-1);

            // Update the slider to reflect our changes
            ui->verticalSliderZoom->setValue(ui->verticalSliderZoom->value()-1);
        }
    }
    // We've finshed acting on the event, re-enable our graphics view
    ui->graphicsViewVisual->setEnabled(true);
}

bool GermCellSimulator::eventFilter(QObject *object, QEvent *event)
{
    // Check that the passed event and object coorespond to what we want to work with which are a
    // mouse event and the graphicsView viewport respectively.
    if (object == ui->graphicsViewVisual->viewport() &&
            event->type() == QEvent::Wheel)

        // Disable the graphics view. This will allow the mouse wheel event to be passed to the form
        // instead of being eaten by viewport.
        ui->graphicsViewVisual->setEnabled(false);

    //Return using the default event handler
    return QObject::eventFilter(object, event);
}

void GermCellSimulator::saveCSV()
{
    this->stopSimulation();

    string fileName = QFileDialog::getSaveFileName(this, tr("Save Population Statistics"), "",
                                                   tr("CSV Files (*.csv)")).toStdString();
    if (!this->saveCSV_inner(fileName))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Results Save Error"),
                           tr("The file could not be saved to the specified location"));
        msgBox.exec();
    }
}

bool GermCellSimulator::saveCSV_inner(string filename)
{
    if (filename == "")
        return false;
    ofstream outfile(filename.c_str());
    if (outfile.is_open())
    {
        outfile << "Hours Passed,";
        for (int i = 0; i < 10; i++)
            outfile << "," << toString(static_cast<CellType>(i)) << " Population Size";
        outfile << ",";
        for (int i = 0; i < 10; i++)
            outfile << "," << toString(static_cast<CellType>(i)) << " Average Distance From Center";
        outfile << endl;

        for (int i = 0; i < this->timeData.size(); i++)
        {
            outfile << this->timeData[i];
            outfile << ",";
            for (int n = 0; n < 10; n++)
                outfile << "," << this->popData[n][i];
            outfile << ",";
            for (int n = 0; n < 10; n++)
                outfile << "," << this->posData[n][i];
            outfile << endl;
        }
        outfile.close();
        return true;
    }
    else
        return false;
}

void GermCellSimulator::saveData()
{
    this->stopSimulation();

    string fileName = QFileDialog::getSaveFileName(this, tr("Save Snapshot"), "",
                                                   tr("CSV Files (*.csv)")).toStdString();
    if (fileName == "")
        return;
    if (!this->saveData_inner(fileName))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Simulation Save Error"),
                           tr("The file could not be saved to the specified location"));
        msgBox.exec();
    }
}

bool GermCellSimulator::saveData_inner(string filename)
{
    ofstream outfile(filename.c_str());
    if (outfile.is_open())
    {
        outfile << "Cell Type,X Position,Y Position,Proliferation,Apoptosis,Differentiation,";
        //* CHANGED BY DEBJIT RAY FOR UNIQUE ID
        outfile << "Speed (Hours till move), Cell ID, Cell Parent ID" << endl;
        multimap<unsigned long long, Cell*>::iterator it;
        for (it = this->sim->population.begin(); it != this->sim->population.end(); it++)
        {
            outfile << toString(it->second->get_type()) << ",";
            outfile << it->second->position.first << "," << it->second->position.second << ",";
            outfile << it->second->proliferation << ",";
            outfile << it->second->apoptosis << ",";
            outfile << it->second->differentiation << ",";
            //* ADDED BY DEBJIT RAY FOR UNIQUE ID
            outfile << it->second->move << ",";
            //* ADDED BY DEBJIT RAY FOR UNIQUE ID
            outfile << it->second->id << ",";
            outfile << it->second->parent << "," << endl;
        }
        outfile.close();
        return true;
    }
    return false;
}

void GermCellSimulator::savePosData()
{
    this->stopSimulation();

    string usrFileName = QFileDialog::getSaveFileName(
                this, tr("Save Position Data"), tr(""),
                tr("CSV Files (*.csv)")).toStdString();
    if (!savePosData_inner(usrFileName))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Simulation Save Error"),
                           tr("The file could not be saved to the specified location"));
        msgBox.exec();
    }
}

bool GermCellSimulator::savePosData_inner(string usrFileName)
{
    if (usrFileName == "")
        return false;

    QDir().mkpath(usrFileName.c_str());
    if (!QDir(this->picFileName.c_str()).exists())
        return false;

    string fileName;
    for (int i = 0; i < 10; i++)
    {
        fileName = usrFileName;
        fileName.append("/");
        fileName.append(toString(static_cast<CellType>(i)));
        fileName.append(".csv");
        ofstream outfile(fileName.c_str());
        if (outfile.is_open())
        {
            outfile << "Hour,Cell 1, Cell 2, Cell 3, ..." << endl;
            for (int n = 0; n < this->indiv_posData[i].size(); n++)
            {
                outfile << n;
                for (unsigned int j = 0; j < this->indiv_posData[i][n].size(); j++)
                    outfile << "," << this->indiv_posData[i][n][j];
                outfile << endl;
            }
            outfile.close();
        }
    }
    return true;
}

void GermCellSimulator::saveErrors()
{
    this->stopSimulation();

    string fileName = QFileDialog::getSaveFileName(
                this, tr("Save Error Data"), tr(""),
                tr("CSV Files (*.csv)")).toStdString();
    if (fileName == "")
        return;
    if (!this->saveErrors_inner(fileName))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Simulation Error File Save Error"),
                           tr("The file could not be saved to the specified location"));
        msgBox.exec();
    }
}

bool GermCellSimulator::saveErrors_inner(string filename)
{
    ofstream outfile(filename.c_str());
    if (outfile.is_open())
    {
        outfile << "Hour,Error Message" << endl;
        if (!this->handledFlags.empty())
        {
            queue<Flag> tmp_flags = this->handledFlags;
            while (!tmp_flags.empty())
            {
                outfile << tmp_flags.front().time << "," << tmp_flags.front().message;
                outfile << endl;
                tmp_flags.pop();
            }
        }
        if (!this->sim->flags.empty())
        {
            list<Flag> tmp_flags = this->sim->flags;
            while (!tmp_flags.empty())
            {
                if (tmp_flags.front().time == -1)
                    outfile << this->timeData.back() << ",";
                else
                    outfile << tmp_flags.front().time << ",";
                outfile << tmp_flags.front().message << endl;
                tmp_flags.pop_front();
            }
        }
        outfile.close();
        return true;
    }
    return false;
}

void GermCellSimulator::savePic()
{
    QImage img(1024, 1024, QImage::Format_ARGB32_Premultiplied);
    QPainter p(&img);
    this->simModel.setBackgroundBrush(QBrush("Black"));
    this->simModel.render(&p);
    p.end();

    string fileName = this->picFileName;
    fileName.append("/Picture_");

    stringstream fileName_tmp;
    fileName_tmp << this->timeData.back();

    fileName.append(fileName_tmp.str());

    fileName.append(".jpeg");

    img.save(QString(fileName.c_str()));
}

void GermCellSimulator::startRecording()
{
    if (this->picFileName == "")
    {
        this->stopSimulation();
        this->picFileName = QFileDialog::getSaveFileName(this, tr("Picture Save Location"), "",
                                                       tr("JPEG Files (*.jpeg)")).toStdString();
        if (this->picFileName == "")
            return;
        QDir().mkpath(this->picFileName.c_str());
        if (!QDir(this->picFileName.c_str()).exists())
        {
            QMessageBox msgBox(QMessageBox::Warning, tr("Picture Save Location Error"),
                               tr("Files cannot be saved to the specified location"));
            msgBox.exec();
            return;
        }
    }

    ui->pushButtonRecordStart->setEnabled(false);
    ui->pushButtonRecordStop->setEnabled(true);
}

void GermCellSimulator::stopRecording()
{
    ui->pushButtonRecordStop->setEnabled(false);
    ui->pushButtonRecordStart->setEnabled(true);
}

