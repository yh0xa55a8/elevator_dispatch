//
// Created by 杨晗 on 2019-04-23.
//

#ifndef OS_ASSIGNMENT1_DISPATCHER_H
#define OS_ASSIGNMENT1_DISPATCHER_H

#include <QObject>
#include <vector>
#include <list>

#include "Elevator.h"

using std::vector;
using std::list;

// handle all target and dispatch to elevator
class Dispatcher: public QObject{
    Q_OBJECT
private:
    // elevators that manages
    vector<Elevator *>elevators;
    // targets hanged
    list<Elevator::Target> targetPool;
    void redispatch(int);
public:
    explicit Dispatcher(QObject * =nullptr);
    ~Dispatcher()override;
public slots:
    void internalInputHandle(int, int);
    void externalInputHandle(int, bool);
signals:
    void statusChanged(Elevator::Status,int);
    void targetFinished(Elevator::Target,int);
};


#endif //OS_ASSIGNMENT1_DISPATCHER_H
