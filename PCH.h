#pragma once
#include "schedulingmethod.h"

typedef struct taskAttributes{
    double weight;
    double commCost;
    double priority;
    double est;
};

typedef struct unschedTasksInfo{
    unsigned int wfIndex;
    vector <taskAttributes> tasks;
};

class PCH :
    public SchedulingMethod
{
private:
    int uid; // 4 - PCH_MERGE, 5 - PCH_RR
public:
    PCH(DataInfo &d, int uid);
    ~PCH(void);
    double GetWFSchedule(Schedule &out);
};

