//
// Created by 杨晗 on 2019-04-23.
//

#include "Dispatcher.h"

Dispatcher::Dispatcher(QObject *parent) :
    QObject(parent),
    elevators(elevatorNum,nullptr){
    for(auto& elevator:elevators){
        elevator = new Elevator(this);
        elevator->start();
    }
    for(int i=0;i<elevatorNum;++i){
        // use lambda as slot to forward signal with addtional info
        connect(elevators[i],
                &Elevator::statusChange,
                this,
                [=](Elevator::Status e){
                    emit this->statusChanged(e,i);
                    redispatch(i);
                });
        connect(elevators[i],
                &Elevator::targetFinished,
                this,
                [=](Elevator::Target t){emit this->targetFinished(t,i);});
    }
}

void Dispatcher::internalInputHandle(int elevatorId, int buttonFloorIndex) {
    auto& elevator = elevators[elevatorId];
    elevator->targetsMutex.lock();
    if(buttonFloorIndex==110){  // stop elevator when emergency
        elevator->status.isDisable=!elevator->status.isDisable;
        emit statusChanged(elevator->status,elevatorId);
        return;
    }
    // normal target
    if(buttonFloorIndex>elevator->status.floor){
        elevator->upwardTargets.emplace(buttonFloorIndex,Elevator::Target::UNDEFINED);
    }
    else if(buttonFloorIndex<elevator->status.floor){
        elevator->downwardTargets.emplace(buttonFloorIndex,Elevator::Target::UNDEFINED);
    }
    // handle target with floor that currently at
    else{
        switch(elevator->status.direction){
            case Elevator::Status::GOING_DOWN:
            case Elevator::Status::STOP:
                elevator->upwardTargets.emplace(buttonFloorIndex,Elevator::Target::UNDEFINED);
                break;
            case Elevator::Status::HOLDING_UP:
            case Elevator::Status::HOLDING_DOWN:
                emit targetFinished(Elevator::Target(static_cast<unsigned>(buttonFloorIndex),Elevator::Target::UNDEFINED),elevatorId);
                break;
            case Elevator::Status::GOING_UP:
                elevator->downwardTargets.emplace(buttonFloorIndex,Elevator::Target::UNDEFINED);
        }
    }
    elevator->targetsMutex.unlock();
}

void Dispatcher::externalInputHandle(int floorIndex,bool isDown) {
    int minDistance = 0xcfcfcf;
    Elevator* minElevator = nullptr;
    for(int i=0;i<elevatorNum;++i){ // loop to find a shortest distance on running direction
        auto elevator = elevators[i];
        elevator->targetsMutex.lock();
        if(elevator->status.isDisable){
            continue;
        }
        if(isDown){
            int tmpDistance = elevator->status.floor-floorIndex;
            if(elevator->status.direction==Elevator::Status::STOP){// for stop elevator, treat as same direction
                tmpDistance = tmpDistance>0?tmpDistance:-tmpDistance;
                if(minDistance>tmpDistance){
                    minDistance=tmpDistance;
                    minElevator=elevator;
                }
            }
            else if(tmpDistance>0&&
               (elevator->status.direction==Elevator::Status::HOLDING_DOWN||
                elevator->status.direction==Elevator::Status::GOING_DOWN)){// for holding and going in same direction
                if(minDistance>tmpDistance){
                    minDistance=tmpDistance;
                    minElevator=elevator;
                }
            }
            else if(tmpDistance==0){// for elevator at target floor, dispatch directly
                if(elevator->status.direction==Elevator::Status::HOLDING_DOWN){
                    emit targetFinished(Elevator::Target(floorIndex,Elevator::Target::DOWNWARD),i);
                    elevator->targetsMutex.unlock();
                    return;
                }
                if(elevator->status.direction==Elevator::Status::STOP) {
                    elevator->downwardTargets.emplace(floorIndex, Elevator::Target::DOWNWARD);
                    elevator->targetsMutex.unlock();
                    return;
                }
            }
        }
        else{// another direction, similar
            int tmpDistance = floorIndex-elevator->status.floor;
            if(elevator->status.direction==Elevator::Status::STOP){
                tmpDistance = tmpDistance>0?tmpDistance:-tmpDistance;
                if(minDistance>tmpDistance){
                    minDistance=tmpDistance;
                    minElevator=elevator;
                }
            }
            else if(tmpDistance>0&&
               (elevator->status.direction==Elevator::Status::HOLDING_UP||
                elevator->status.direction==Elevator::Status::GOING_UP)){
                if(minDistance>tmpDistance){
                    minDistance=tmpDistance;
                    minElevator=elevator;
                }
            }
            else if(tmpDistance==0){
                if(elevator->status.direction==Elevator::Status::HOLDING_UP){
                    emit targetFinished(Elevator::Target(floorIndex,Elevator::Target::UPWARD),i);
                    elevator->targetsMutex.unlock();
                    return;
                }
                if(elevator->status.direction==Elevator::Status::STOP) {
                    elevator->upwardTargets.emplace(floorIndex, Elevator::Target::UPWARD);
                    elevator->targetsMutex.unlock();
                    return;
                }
            }
        }
        elevator->targetsMutex.unlock();
    }
    if(minElevator){    //got a target elevator
        minElevator->targetsMutex.lock();
        if(floorIndex<minElevator->status.floor){
            minElevator->downwardTargets.emplace(floorIndex,isDown?Elevator::Target::DOWNWARD:Elevator::Target::UPWARD);
        }
        else{
            minElevator->upwardTargets.emplace(floorIndex,isDown?Elevator::Target::DOWNWARD:Elevator::Target::UPWARD);
        }
        minElevator->targetsMutex.unlock();
    }
    else{
        // currently no elevator available
        targetPool.emplace_back(floorIndex,isDown?Elevator::Target::DOWNWARD:Elevator::Target::UPWARD);
    }
}

Dispatcher::~Dispatcher() {
    for(auto& elevator:elevators){
        elevator->requestInterruption();
        elevator->quit();
    }
    for(auto& elevator:elevators){
        elevator->wait();
    }
}

void Dispatcher::redispatch(int elevatorIndex) {
    if(targetPool.empty()){
        return;
    }
    auto* elevator = elevators[elevatorIndex];
    elevator->targetsMutex.lock();
    auto tmpGoing = targetPool.front().direction==Elevator::Target::UPWARD?Elevator::Status::GOING_UP:Elevator::Status::GOING_DOWN;
    auto tmpHolding = targetPool.front().direction==Elevator::Target::UPWARD?Elevator::Status::HOLDING_UP:Elevator::Status::HOLDING_DOWN;

    if(elevator->status.direction==Elevator::Status::STOP||
       elevator->status.direction==tmpGoing||
       elevator->status.direction==tmpHolding){
        auto tmpPool = targetPool;
        targetPool = list<Elevator::Target>();
        elevator->targetsMutex.unlock();
        for(auto i:tmpPool){
            externalInputHandle(i.floor,i.direction==Elevator::Target::DOWNWARD);
        }
    }
    else{
        elevator->targetsMutex.unlock();
    }
}
