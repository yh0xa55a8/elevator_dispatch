#include <QApplication>

#include "MainWindowController.h"

// enter point of program
int main(int argc,char** argv) {
    QApplication app(argc,argv);
    MainWindowController vc;
    vc.show();
    return QApplication::exec();
}

