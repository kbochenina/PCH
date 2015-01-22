#pragma once
#include "schedulingmethod.h"
class PCH :
    public SchedulingMethod
{
public:
    PCH(DataInfo &d, int uid);
    ~PCH(void);
    double GetWFSchedule(Schedule &out);
};

