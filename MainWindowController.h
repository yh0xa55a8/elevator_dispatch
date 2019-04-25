//
// Created by 杨晗 on 2019-04-22.
//

#ifndef OS_ASSIGNMENT1_MAINWINDOW_CONTROLLER_H
#define OS_ASSIGNMENT1_MAINWINDOW_CONTROLLER_H

#include <QMainWindow>
#include <QGridLayout>
#include <QSpacerItem>
#include <vector>

#include "InternalPanel.h"
#include "ExternalPanel.h"
#include "Dispatcher.h"

using std::vector;

// ViewController for main window
class MainWindowController: public QMainWindow {
    Q_OBJECT
private:
    // sub-widgets
    QWidget *centralWidget;
    vector<InternalPanel *> internalPanels;
    ExternalPanel *externalPanel;

    // model
    Dispatcher *dispatcher;

    inline void connectSignals();
public:
    explicit MainWindowController(QWidget * =nullptr);
private slots:
    // receive signals from sub-widgets and resend
    void internalPanelSignalForward(QAbstractButton*);
    void externalPanelSignalForward(QAbstractButton*);
public slots:
    // update ui with data from model
    void updateUiByStatus(Elevator::Status,int);
    void updateUiForTargetFinished(Elevator::Target,int);
signals:
    // signals sent to model with addtional info
    void internalPanelSignal(int elevatorIndex,int buttonId);
    void externalPanelSignal(int floor,bool isDown);
};


#endif //OS_ASSIGNMENT1_MAINWINDOW_CONTROLLER_H
