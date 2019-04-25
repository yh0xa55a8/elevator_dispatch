//
// Created by 杨晗 on 2019-04-23.
//

#ifndef OS_ASSIGNMENT1_ELEVATOR_H
#define OS_ASSIGNMENT1_ELEVATOR_H

#include <QThread>
#include <QMetaType>
#include <queue>
#include <mutex>

extern const unsigned buildingHeight;
extern const unsigned elevatorNum;

using std::priority_queue;
using std::vector;
using std::mutex;


// entity of elevator. handle all status and operations of self, also manage targets dispatched to.
class Elevator: public QThread {
    Q_OBJECT
public:
    // struct for status and target
    struct Status{
        unsigned floor;
        enum Direction:signed char{
            STOP=0,HOLDING_UP=1,HOLDING_DOWN=-1,GOING_UP=2,GOING_DOWN=-2
        } direction;
        bool isDisable;
    };
    struct Target{
        unsigned floor;
        enum Direction:int{
            UNDEFINED=0,UPWARD=1,DOWNWARD=-1
        }direction;
        Target():floor(1),direction(UNDEFINED){}
        Target(unsigned f,Direction d):floor(f),direction(d){}
        struct greater{bool operator()(const Target& lhs,const Target& rhs){return lhs.floor>rhs.floor;}};
        struct less{bool operator()(const Target& lhs,const Target& rhs){return lhs.floor<rhs.floor;}};
    };
private:
    // target queue for both directions
    priority_queue<Target,vector<Target>,Target::greater> upwardTargets;
    priority_queue<Target,vector<Target>,Target::less> downwardTargets;
    // mutex for status and targets to keep thread safe
    mutex targetsMutex;

    Status status;
protected:
    void run()override;
public:
    explicit Elevator(QObject* =nullptr);
    friend class Dispatcher;
signals:
    void statusChange(Status);  //emit when status changed, keep view correct
    void targetFinished(Target);//emit when target finished, keep view correct
};


#endif //OS_ASSIGNMENT1_ELEVATOR_H
