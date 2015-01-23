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
     if (uid == 5){
        // PCH_RR
        // get order in accordance with starting time
        //SortTStart(order);
    }
   
    // current workflow to be scheduled
//    unsigned int currentWf = order[0];
    InitUnscheduledTasks();

    return 0.0;
}

void PCH::InitUnscheduledTasks(){
    for (auto& wf: data.Workflows()){
        unschedTasksInfo wfTasksInfo;
        wfTasksInfo.wfIndex = wf.GetUID();
        taskInfo tasks;
        for (size_t i = 0; i < wf.GetPackageCount(); i++){
            taskAttributes task;
            task.weight = wf.GetMinExecTime(i);
            vector<int> children;
            wf.GetOutput(i, children);
            for (auto& child : children){
                double transfer = wf.GetTransfer(i, child);
                double bestBandwidth = data.GetMaxBandwidth();
                task.commCost.push_back(make_pair(child, transfer / bestBandwidth));
            }

            tasks.push_back(make_pair(i, task));
        }
        cout << endl;
    }
}

PCH::~PCH(void)
{
}
