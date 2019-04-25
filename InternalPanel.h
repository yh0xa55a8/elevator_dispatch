//
// Created by 杨晗 on 2019-04-22.
//

#ifndef OS_ASSIGNMENT1_INTERNAL_PANEL_H
#define OS_ASSIGNMENT1_INTERNAL_PANEL_H

#include <QWidget>
#include <QLabel>
#include <QLCDNumber>
#include <QGridLayout>
#include <QButtonGroup>
#include <QSpacerItem>
#include <vector>

#include "StatusButton.h"
#include "Elevator.h"

using std::vector;

extern const unsigned buildingHeight;
extern const unsigned elevatorNum;

// control panel that inside the elevator with direction indicator, floor display, emergency button, and floor button.
class InternalPanel: public QWidget {
    Q_OBJECT
private:
    // sub-widgets
    QLabel *title;
    QLabel *arrowLabel;
    QLCDNumber *floorDisplay;
    QButtonGroup *buttonGroup;
    vector<StatusButton *> floorButtons;
    // abstract set to control buttons
    StatusButton *emergencyButton;
public:
    explicit InternalPanel(const QString&,QWidget * =nullptr);
    friend class MainWindowController;
    void setLabelStyle(Elevator::Status::Direction);
};


#endif //OS_ASSIGNMENT1_INTERNAL_PANEL_H
