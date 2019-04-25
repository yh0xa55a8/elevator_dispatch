//
// Created by 杨晗 on 2019-04-22.
//

#include "ExternalPanel.h"

ExternalPanel::ExternalPanel(QWidget *parent):
    QWidget(parent),
    buttons(buildingHeight,std::make_pair(nullptr,nullptr)),
    labels(buildingHeight,nullptr){
    auto* layout = new QGridLayout();
    buttonGroup = new QButtonGroup();
    for(int i=1;i<=buildingHeight;++i){
        labels[i-1] = new QLabel(QString("Floor %1").arg(i));
        buttons[i-1] = std::make_pair(new StatusButton("↑"),new StatusButton("↓"));
        layout->addWidget(labels[i-1],buildingHeight-i,0);
        layout->addWidget(buttons[i-1].first,buildingHeight-i,1);
        buttonGroup->addButton(buttons[i-1].first,(i-1)*2);
        layout->addWidget(buttons[i-1].second,buildingHeight-i,2);
        buttonGroup->addButton(buttons[i-1].second,(i-1)*2+1);
    }
    buttons[0].second->hide();
    buttons[buildingHeight-1].first->hide();
    this->setLayout(layout);
}
