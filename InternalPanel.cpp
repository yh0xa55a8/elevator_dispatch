//
// Created by 杨晗 on 2019-04-22.
//

#include "InternalPanel.h"

InternalPanel::InternalPanel(const QString& titleContent,QWidget *parent):QWidget(parent),floorButtons(buildingHeight,nullptr) {
    title = new QLabel(titleContent,this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:30px");
    arrowLabel = new QLabel(" ",this);
    arrowLabel->setStyleSheet("font-size:50px");
    floorDisplay = new QLCDNumber(2, this);
    floorDisplay->display(1);
    floorDisplay->setMinimumHeight(100);
    buttonGroup = new QButtonGroup(this);
    emergencyButton = new StatusButton("E",this);
    buttonGroup->addButton(emergencyButton,110);
    auto* panelLayout = new QGridLayout(this);
    auto* buttonsLayout = new QGridLayout();
    for(int i=1;i<=buildingHeight;++i){
        floorButtons[i-1] = new StatusButton(QString("%1").arg(i));
        floorButtons[i-1]->setMinimumSize(25,25);
        buttonsLayout->addWidget(floorButtons[i-1],(i-1)/5,(i-1)%5,Qt::AlignCenter);
        buttonGroup->addButton(floorButtons[i-1],i-1);
    }
    int minimumRowHeight = 30;
    panelLayout->addWidget(title,0,0);
    panelLayout->setRowMinimumHeight(0,3*minimumRowHeight);
    panelLayout->addWidget(arrowLabel,1,0,Qt::AlignCenter);
    panelLayout->setRowMinimumHeight(1,3*minimumRowHeight);
    panelLayout->addWidget(floorDisplay,2,0);
    panelLayout->setRowMinimumHeight(2,3*minimumRowHeight);
    panelLayout->addItem(new QSpacerItem(0,5*minimumRowHeight,QSizePolicy::Minimum,QSizePolicy::Fixed),3,0);
    panelLayout->addWidget(emergencyButton,4,0,Qt::AlignCenter);
    panelLayout->setRowMinimumHeight(4,1*minimumRowHeight);
    panelLayout->addLayout(buttonsLayout,5,0,Qt::AlignCenter);
    panelLayout->setRowMinimumHeight(5,buildingHeight/5*minimumRowHeight);
    this->setLayout(panelLayout);
}

// arrow direction indicates moving direction. red arrow for moving, white arrow for holding
void InternalPanel::setLabelStyle(Elevator::Status::Direction d) {
    arrowLabel->setText(" ");
    arrowLabel->setStyleSheet("color:white;font-size:50px");
    switch(d){
        case Elevator::Status::STOP:
            break;
        case Elevator::Status::GOING_UP:
            arrowLabel->setStyleSheet("color:red;font-size:50px");
        case Elevator::Status::HOLDING_UP:
            arrowLabel->setText("↑");
            break;
        case Elevator::Status::GOING_DOWN:
            arrowLabel->setStyleSheet("color:red;font-size:50px");
        case Elevator::Status::HOLDING_DOWN:
            arrowLabel->setText("↓");
            break;
    }
}
