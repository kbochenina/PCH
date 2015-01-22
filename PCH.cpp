#include "stdafx.h"
#include "PCH.h"


PCH::PCH(DataInfo &d, int uid) : SchedulingMethod(d,uid)
{
    this->uid = uid;
}

// workflows are numbered from 0
double PCH::GetWFSchedule(Schedule &out){
    // order of workflows in round-robin
    vector <unsigned int> order;
    if (uid == 4){
        // PCH_MERGE
        // MergeWorkflows creates new workflow from existing
        // and adds it to the end of vector of workflows
        data.MergeWorkflows();
        // ordering vector will contain one, last workflow
        order.push_back(data.GetWFCount()-1);
    }
    else {
        // get order in accordance with starting time
        //SortTStart(order);
    }
    // current workflow to be scheduled
    unsigned int currentWf = order[0];
    // information about unscheduled tasks
    vector <unschedTasksInfo> unsched;

    if (uid == 4){
        // delete merged workflow from data structure
        data.DeleteLastWorkflow();
    }
    return 0.0;
}

PCH::~PCH(void)
{
}
