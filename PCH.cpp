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
        taskInfo tasks;
        for (size_t i = 0; i < wf.GetPackageCount(); i++){
            taskAttributes task;
            task.weight = wf.GetMinExecTime(i);
            vector<int> children;
            wf.GetOutput(i, children);
            for (auto& child : children){
                double transfer = wf.GetTransfer(i, child);
                double bestBandwidth = data.GetMaxBandwidth();
                task.commCost[child] = transfer / bestBandwidth;
            }
            tasks[i] = task;
        }
		SetPriorities(tasks, wf);
		SetESTs(tasks, wf);
		unsched[wf.GetUID()] = tasks;
    }
}


// setting the priorities for unscheduled tasks of current workflow
void PCH::SetPriorities(taskInfo& tasks, const Workflow& wf){
	// set priorities 
	vector<int> prioritiesQueue;

	// find last tasks
	for (size_t i = 0; i < wf.GetPackageCount(); i++){
		if (wf.IsPackageLast(i)){
			tasks[i].priority = tasks[i].weight;
			vector<int> parents;
			wf.GetInput(i, parents);
			prioritiesQueue.insert(prioritiesQueue.begin(), parents.begin(), parents.end());
		}
	}

	while (prioritiesQueue.size() != 0)
	{
		vector <int> candidates;
		while (prioritiesQueue.size() != 0)
		{
			int task = *(prioritiesQueue.end()-1);
			prioritiesQueue.pop_back();
			double max = 0.0;
			for (auto& childrenComm : tasks[task].commCost)	{
				int children = childrenComm.first;
				double commPlusPriority = childrenComm.second + tasks[children].priority;
				if (commPlusPriority > max){
					max = commPlusPriority;
				}
			}
			tasks[task].priority = max + tasks[task].weight;
			vector<int> parents;
			wf.GetInput(task, parents);
			candidates.insert(candidates.end(), parents.begin(), parents.end());
		}
		prioritiesQueue.insert(prioritiesQueue.end(), candidates.begin(), candidates.end());
	}
}

// setting the ESTs for unscheduled tasks of current workflow
void PCH::SetESTs(taskInfo& tasks, const Workflow& wf){
	// set ESTs
	vector<int> ESTqueue;

	// find init tasks
	for (size_t i = 0; i < wf.GetPackageCount(); i++){
		if (wf.IsPackageInit(i)){
			tasks[i].est = 0.0;
			vector <int> children;
			wf.GetOutput(i,children);
			ESTqueue.insert(ESTqueue.begin(), children.begin(), children.end());
		}
	}

	while (ESTqueue.size() != 0)
	{
		vector <int> candidates;
		while (ESTqueue.size() != 0)
		{
			int task = *(ESTqueue.end()-1);
			ESTqueue.pop_back();
			double max = 0.0;
			vector<int> parents;
			wf.GetInput(task, parents);

			for (auto& parent : parents)	{
				double weightCommEST = tasks[parent].weight + tasks[parent].commCost[task] + tasks[parent].est;
				if (weightCommEST > max){
					max = weightCommEST;
				}
			}
			tasks[task].est = max;
			vector<int> children;
			wf.GetOutput(task, children);
			candidates.insert(candidates.end(), children.begin(), children.end());
		}
		ESTqueue.insert(ESTqueue.end(), candidates.begin(), candidates.end());
	}
}
PCH::~PCH(void)
{
}
