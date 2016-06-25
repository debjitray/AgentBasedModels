#include <QtGui/QApplication>
#include "command_args.h"


int main(int argc, char *argv[])
{
    // Run GUI
    if (argc == 1)
    {
        QApplication a(argc, argv);
        bool valid;
        GermCellSimulator w(&valid);
        if (valid)
            w.show();
        return a.exec();
    }
    return handle_command_args(argc, argv);
}
