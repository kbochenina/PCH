#pragma once
#include "schedulingmethod.h"

typedef struct taskAttributes{
    double weight;
    // vector of (childUid, commTime) for best bandwidth
    vector <pair<int,double>> commCost;
    double priority;
    double est;
};

// vector of (taskUID, taskAttributes)
typedef vector <pair<int, taskAttributes>> taskInfo;

typedef struct unschedTasksInfo{
    unsigned int wfIndex;
    taskInfo tasks;
};

class PCH :
    public SchedulingMethod
{
private:
    // 4 - PCH_MERGE, 5 - PCH_RR
    int uid; 
    // information about unscheduled tasks
    vector <unschedTasksInfo> unsched;
    // add all tasks from all workflows to unscheduled
    void InitUnscheduledTasks();
public:
    PCH(DataInfo &d, int uid);
    ~PCH(void);
    double GetWFSchedule(Schedule &out);
};

