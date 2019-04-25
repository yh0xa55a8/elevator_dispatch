//
// Created by 杨晗 on 2019-04-23.
//

#include "Elevator.h"

Elevator::Elevator(QObject *parent) :
    QThread(parent),
    status({0,Status::STOP,false}){
    qRegisterMetaType<Status>("Status");
    qRegisterMetaType<Target>("Target");
}

void Elevator::run() {
    while(!isInterruptionRequested()){
        if(!status.isDisable){  // for emergency situation, stop all operation
            targetsMutex.lock();
            switch(status.direction){
                case Status::STOP:
                    // when receive new target, restart from stop
                    if(!upwardTargets.empty()){
                        if(upwardTargets.top().floor==status.floor){
                            // handle target from where self stops
                            status.direction=Status::HOLDING_UP;
                            emit targetFinished(upwardTargets.top());
                            upwardTargets.pop();
                        }
                        else{
                            // target above makes directly going up
                            status.direction=Status::GOING_UP;
                        }
                        emit statusChange(status);
                    }
                    // similar to above
                    else if(!downwardTargets.empty()){
                        if(downwardTargets.top().floor==status.floor){
                            status.direction=Status::HOLDING_DOWN;
                            emit targetFinished(downwardTargets.top());
                            downwardTargets.pop();
                        }
                        else{
                            status.direction=Status::GOING_DOWN;
                        }
                        emit statusChange(status);
                    }
                    break;
                case Status::HOLDING_UP:
                    // stop when finished all targets or arrive at top
                    if(upwardTargets.empty()||status.floor==buildingHeight-1){
                        status.direction=Status::STOP;
                    }
                    else{
                        // adjust targets on the opposite or with wrong direction
                        while(!upwardTargets.empty()&&
                              (status.floor>upwardTargets.top().floor||
                               (upwardTargets.top().floor==status.floor&&
                                upwardTargets.top().direction==Target::DOWNWARD))){
                            downwardTargets.push(upwardTargets.top());
                            upwardTargets.pop();
                        }
                        if(upwardTargets.empty()){
                            status.direction=Status::STOP;
                        }
                        else{
                            status.direction=Status::GOING_UP;
                        }
                    }
                    emit statusChange(status);
                    break;
                case Status::HOLDING_DOWN:
                    // similar to HOLDING_UP
                    if(downwardTargets.empty()||status.floor==0){
                        status.direction=Status::STOP;
                    }
                    else{
                        while(!downwardTargets.empty()&&
                              (status.floor<downwardTargets.top().floor||
                               (downwardTargets.top().floor==status.floor&&
                                downwardTargets.top().direction==Target::UPWARD))){
                            upwardTargets.push(downwardTargets.top());
                            downwardTargets.pop();
                        }
                        if(downwardTargets.empty()){
                            status.direction=Status::STOP;
                        }
                        else{
                            status.direction=Status::GOING_DOWN;
                        }
                    }
                    emit statusChange(status);
                    break;
                case Status::GOING_UP:
                    // adjust targets on the opposite
                    while(!downwardTargets.empty()&&status.floor>upwardTargets.top().floor){
                        downwardTargets.push(upwardTargets.top());
                        upwardTargets.pop();
                    }
                    // arrive at a target
                    if(status.floor==upwardTargets.top().floor){
                        // another direction
                        if(upwardTargets.top().direction==Target::DOWNWARD){
                            // last target on current direction, holding
                            if(upwardTargets.size()==1){
                                emit targetFinished(upwardTargets.top());
                                upwardTargets.pop();
                                status.direction=Status::HOLDING_DOWN;
                            }
                            // adjust it to another direction targets
                            else{
                                downwardTargets.push(upwardTargets.top());
                                upwardTargets.pop();
                                status.floor++;
                            }
                        }
                        // same direction
                        else{
                            status.direction=Status::HOLDING_UP;
                            emit targetFinished(upwardTargets.top());
                            upwardTargets.pop();
                        }
                    }
                    // no special, going up
                    else {
                        status.floor++;
                    }
                    emit statusChange(status);
                    break;
                case Status::GOING_DOWN:
                    // similar to GOING_UP
                    while(!upwardTargets.empty()&&status.floor<downwardTargets.top().floor){
                        upwardTargets.push(downwardTargets.top());
                        downwardTargets.pop();
                    }
                    if(status.floor==downwardTargets.top().floor){
                        if(downwardTargets.top().direction==Target::UPWARD){
                            if(downwardTargets.size()==1){
                                emit targetFinished(downwardTargets.top());
                                downwardTargets.pop();
                                status.direction=Status::HOLDING_UP;
                            }
                            else{
                                upwardTargets.push(downwardTargets.top());
                                downwardTargets.pop();
                                status.floor--;
                            }
                        }
                        else{
                            status.direction=Status::HOLDING_DOWN;
                            emit targetFinished(downwardTargets.top());
                            downwardTargets.pop();
                        }
                    }
                    else {
                        status.floor--;
                    }
                    emit statusChange(status);
                    break;
            }
            targetsMutex.unlock();
        }
        sleep(1);
    }
}
