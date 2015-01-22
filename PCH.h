#pragma once
#include "schedulingmethod.h"

typedef struct taskAttributes{
    double weight;
    double commCost;
    double priority;
    double est;
};

// vector of (taskIndex, taskAttributes)
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
    // order of workflows in round-robin
    vector <unsigned int> order;
    // add all tasks from all workflows to unscheduled
    void InitUnscheduledTasks();
public:
    PCH(DataInfo &d, int uid);
    ~PCH(void);
    double GetWFSchedule(Schedule &out);
};

