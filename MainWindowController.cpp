//
// Created by 杨晗 on 2019-04-22.
//

#include "MainWindowController.h"

const unsigned buildingHeight = 20;
const unsigned elevatorNum = 5;

MainWindowController::MainWindowController(QWidget *parent):QMainWindow(parent),internalPanels(elevatorNum,nullptr) {
    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);
    auto *layout = new QGridLayout();
    for(int i=0;i<elevatorNum;++i){
        internalPanels[i] = new InternalPanel(QString("Elevator %1").arg(i+1));

        layout->addWidget(internalPanels[i],0,i);
        // spacer makes view looks better
        layout->addItem(new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Expanding),1,i);
    }
    externalPanel = new ExternalPanel(this);
    layout->addWidget(externalPanel,0,elevatorNum,2,1);
    dispatcher = new Dispatcher(this);
    centralWidget->setLayout(layout);
    connectSignals();
}

void MainWindowController::connectSignals() {
    for(auto* panel:internalPanels){
        connect(panel->buttonGroup,
                static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),
                this,
                &MainWindowController::internalPanelSignalForward);
    }

    connect(this,&MainWindowController::internalPanelSignal,dispatcher, &Dispatcher::internalInputHandle);

    connect(externalPanel->buttonGroup,
            static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),
            this,
            &MainWindowController::externalPanelSignalForward);
    connect(this,&MainWindowController::externalPanelSignal,dispatcher,&Dispatcher::externalInputHandle);

    connect(dispatcher,&Dispatcher::statusChanged,this,&MainWindowController::updateUiByStatus);
    connect(dispatcher,&Dispatcher::targetFinished,this,&MainWindowController::updateUiForTargetFinished);

}

void MainWindowController::internalPanelSignalForward(QAbstractButton *sender) {
    for(int i=0;i<internalPanels.size();++i){
        int tmpId = internalPanels[i]->buttonGroup->id(sender);
        if(tmpId!=-1){  // make sure id exist
            auto* tmpButton = dynamic_cast<StatusButton*>(sender);
            if(tmpId==110){ // emergency button
                tmpButton->setStatus(!tmpButton->getStatus());
                emit internalPanelSignal(i,tmpId);
                return;
            }
            else if(!tmpButton->getStatus()){   // floor button
                tmpButton->setStatus(true);
                emit internalPanelSignal(i,tmpId);
                return;
            }
        }
    }
}

void MainWindowController::externalPanelSignalForward(QAbstractButton *sender) {
    auto* tmpButton = dynamic_cast<StatusButton*>(sender);
    int tmpId = externalPanel->buttonGroup->id(sender);
    if(!tmpButton->getStatus()){
        tmpButton->setStatus(true);
        emit externalPanelSignal(tmpId/2,tmpId%2);  //calculate from id to floor and direction
    }
}

void MainWindowController::updateUiByStatus(Elevator::Status s, int i) {
    auto& panelToSet = internalPanels[i];
    panelToSet->setLabelStyle(s.direction);
    panelToSet->emergencyButton->setStatus(s.isDisable);
    panelToSet->floorDisplay->display(static_cast<int>(s.floor)+1);
}

void MainWindowController::updateUiForTargetFinished(Elevator::Target t, int i) {
    if(t.direction==Elevator::Target::UNDEFINED){
        auto& panelToSet = internalPanels[i];
        panelToSet->floorButtons[t.floor]->setStatus(false);
    }
    else if(t.direction==Elevator::Target::UPWARD){
        externalPanel->buttons[t.floor].first->setStatus(false);
    }
    else{
        externalPanel->buttons[t.floor].second->setStatus(false);
    }
}
