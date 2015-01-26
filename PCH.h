#pragma once
#include "schedulingmethod.h"

typedef struct taskAttributes{
    double weight;
    // vector of (childUid, commTime) for best bandwidth
    map<int,double> commCost;
    double priority;
    double est;
};

// map of (taskUID, taskAttributes)
typedef map<int, taskAttributes> taskInfo;


class PCH :
    public SchedulingMethod
{
private:
    // 4 - PCH_MERGE, 5 - PCH_RR
    int uid; 
    // information about unscheduled tasks
    map <unsigned int, taskInfo> unsched;
    // add all tasks from all workflows to unscheduled
    void InitUnscheduledTasks();
	// setting the priorities for unscheduled tasks of current workflow
	void SetPriorities(taskInfo& tasks, const Workflow& wf); 
	// setting the ESTs for unscheduled tasks of current workflow
	void SetESTs(taskInfo& tasks, const Workflow& wf);
public:
    PCH(DataInfo &d, int uid);
    ~PCH(void);
    double GetWFSchedule(Schedule &out);
};

