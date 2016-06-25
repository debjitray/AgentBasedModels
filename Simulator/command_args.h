#ifndef COMMAND_ARGS_H
#define COMMAND_ARGS_H

#include "germcellsimulator.h"
#include "GlobalVariables.h"

Globals global;

inline int handle_command_args(int argc, char *argv[])
{
    // Handle help (--help)
    if (string(argv[1]) == "--help")
    {
        cout << endl;
        cout << "Usage: GermCellSimulator [OPTION]... HOURS" << endl;
        cout << "Create and run a Germ Cell Simulation for HOURS hours" << endl;
        cout << "When and OPTION is specified, files are saved in the ";
        cout << "LOCATION as .csv files" << endl;
        cout << endl;
        cout << "  -d [LOCATION] \t\t save population statistics data" << endl;
        cout << "  -p [LOCATION] \t\t save cell position data" << endl;
        cout << "  -e [LOCATION] \t\t save error statistic data" << endl;
        cout << "  -r [LOCATION] [START] [END] \t record a picture per hour ";
        cout << "from hour START to hour END" << endl;
        cout << "  -s [LOCATION] [START] [END] \t save a snapshot of the simulation per hour ";
        cout << "from hour START to hour END" << endl;
        return 0;
    }

    // Command has options (command line)
    string statsSave = "";
    string posSave = "";
    string picSave = "";
    string recSave = "";
    string errSave = "";

    int picStart = 0;
    int picEnd = 0;
    int recStart = 0;
    int recEnd = 0;

    string invalid = "Invalid command syntax. Please see documentation or try --help";

    int i = 1;
    while (i < argc)
    {
        if (i == argc - 1)
        {
            // Further options have not been specified
            int x = 0;
            while (argv[i][x] != '\0')
            {
                if (argv[i][x] > '9' || argv[i][x] < '0')
                {
                    cout << invalid << endl;
                    return 0;
                }
                x++;
            }
            i++;
        }
        else if (argv[i][0] == '-' && argv[i][2] == '\0')
        {
            // Further options have been specified
            while (i < argc - 1)
            {
                if (argv[i][1] == 'd' || argv[i][1] == 'p' || argv[i][1] == 'e')
                {
                    // Single additional argument option specified
                    i++;
                    if (i == argc - 1)
                    {
                        cout << invalid << endl;
                        return 0;
                    }
                    if (argv[i-1][1] == 'd')
                        statsSave = argv[i];
                    if (argv[i-1][1] == 'p')
                        posSave = argv[i];
                    if (argv[i-1][1] == 'e')
                        errSave = argv[i];

                    i++;
                }
                else if (argv[i][1] == 'r' || argv[i][1] == 's')
                {
                    // 3 additional arguments option specified
                    i++;
                    if (i == argc - 1)
                    {
                        cout << invalid << endl;
                        return 0;
                    }
                    if (argv[i-1][1] == 'r')
                        recSave = argv[i];
                    if (argv[i-1][1] == 's')
                        picSave = argv[i];

                    i++;
                    if (i == argc - 1)
                    {
                        cout << invalid << endl;
                        return 0;
                    }
                    int x = 0;
                    while (argv[i][x] != '\0')
                    {
                        if (argv[i][x] > '9' || argv[i][x] < '0')
                        {
                            cout << invalid << endl;
                            return 0;
                        }
                        x++;
                    }
                    if (argv[i-2][1] == 'r')
                        recStart = atoi(argv[i]);
                    if (argv[i-2][1] == 's')
                        picStart = atoi(argv[i]);

                    i++;
                    if (i == argc - 1)
                    {
                        cout << invalid << endl;
                        return 0;
                    }
                    x = 0;
                    while (argv[i][x] != '\0')
                    {
                        if (argv[i][x] > '9' || argv[i][x] < '0')
                        {
                            cout << invalid << endl;
                            return 0;
                        }
                        x++;
                    }
                    if (argv[i-3][1] == 'r')
                        recEnd = atoi(argv[i]);
                    if (argv[i-3][1] == 's')
                        picEnd = atoi(argv[i]);

                    i++;
                }
                else
                {
                    cout << invalid << endl;
                    return 0;
                }
            }
        }
        else
        {
            cout << invalid << endl;
            return 0;
        }
    }

    // Command is valid
    int time = atoi(argv[argc-1]);
    QApplication a(argc, argv);
    bool valid;
    GermCellSimulator w(&valid);
    if (valid)
    {
        // Run simulator
        w.loadSimulation();
        
        if (picSave != "")
        {
            w.picFileName = picSave;
            QDir().mkpath(w.picFileName.c_str());
            if (!QDir(w.picFileName.c_str()).exists())
            {
                cout << "Could not create picture folder" << endl;
                return false;
            }
        }

        if (recSave != "")
        {
            QDir().mkpath(recSave.c_str());
            if (!QDir(recSave.c_str()).exists())
            {
                cout << "Could not create snapshot file container folder" << endl;
                return false;
            }
        }
        
        w.picFileName = picSave;

        /*time_t rawtime;
        time ( &rawtime);
        struct tm * timeinfo;
        rawtime = time (NULL);
        timeinfo = localtime ( &rawtime );
        string recFileName = asctime (timeinfo);
        recFileName.resize(recFileName.length() - 1);*/
        string recFileName = "snapshot-";

        for (int n = 0; n < time; n++)
        {
            if (picSave != "" && n >= picStart && n <= picEnd)
                w.savePic();
            if (recSave != "" && n >= recStart && n <= recEnd)
            {
                stringstream ss;
                ss << n;
                string tmpFileName = recFileName;
                tmpFileName.append(ss.str());
                string saveFile = recSave;
                saveFile.append("/");
                saveFile.append(tmpFileName);
                w.saveData_inner(saveFile);
            }
            w.update();
        }
        w.saveCSV_inner(statsSave);
        w.savePosData_inner(posSave);
        w.saveErrors_inner(errSave);
    }
    else
    {
        cout << "parameters.txt not found. Please ensure it is placed in the same" << endl;
        cout << "directory as the executable" << endl;
    }
    return 0;
}

#endif
