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
    // current number of unconsidered tasks (for all workflows)
    unsigned unschedCount;
    // add all tasks from all workflows to unscheduled
    void InitUnscheduledTasks();
    // setting the priorities for unscheduled tasks of current workflow
    void SetPriorities(taskInfo& tasks, const Workflow& wf); 
    // setting the ESTs for unscheduled tasks of current workflow
    void SetESTs(taskInfo& tasks, const Workflow& wf);
    // for PCH_MERGE - find workflow which cluster will be formed next, and first task of the cluster
    void FindCurrentWorkflow(unsigned &wfUID, unsigned& taskIndex);
    // find cluster
    void FindCluster(unsigned currentWf, unsigned firstTask, vector<unsigned>& cluster);
    // delete clusterized tasks from unscheduled
    void DeleteClusterTasksFromUnsched(unsigned currentWf, vector<unsigned>& cluster);
    // search for cluster's schedule
    void FindScheduleForCluster(Schedule& out, unsigned currentWf, vector<unsigned>& cluster);
public:
    PCH(DataInfo &d, int uid);
    ~PCH(void);
    double GetWFSchedule(Schedule &out);
};

