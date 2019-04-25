//
// Created by 杨晗 on 2019-04-22.
//

#ifndef OS_ASSIGNMENT1_EXTERNAL_PANEL_H
#define OS_ASSIGNMENT1_EXTERNAL_PANEL_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QButtonGroup>
#include <vector>
#include "StatusButton.h"

using std::vector;
using std::pair;

extern const unsigned buildingHeight;
extern const unsigned elevatorNum;

// panel that outside the elevator with up and down button for every floor
class ExternalPanel: public QWidget {
    Q_OBJECT
private:
    // abstract set to control buttons
    QButtonGroup *buttonGroup;
    // sub-widgets
    vector<pair<StatusButton*,StatusButton*>> buttons;
    vector<QLabel*> labels;
public:
    explicit ExternalPanel(QWidget * =nullptr);
    friend class MainWindowController;
};


#endif //OS_ASSIGNMENT1_EXTERNAL_PANEL_H
