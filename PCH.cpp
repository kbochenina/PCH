#include "stdafx.h"
#include "PCH.h"


PCH::PCH(DataInfo &d, int uid) : SchedulingMethod(d,uid)
{
    this->uid = uid;
    unschedCount = 0;
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
    unsigned currentWf = 0;
    InitUnscheduledTasks();
    while (unschedCount != 0){
        unsigned firstTask = 0;
        // PCH_MERGE
        if (uid == 4){
            FindCurrentWorkflow(currentWf, firstTask);
        }
        // PCH_RR
        else {

        }
        vector <unsigned> cluster;
        cluster.push_back(firstTask);
        FindCluster(currentWf, firstTask, cluster);
        unschedCount -= cluster.size();
    }

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
      unschedCount += wf.GetPackageCount();
    }
}

// for PCH_MERGE - find workflow which cluster will be formed next, and first task of the cluster
void PCH::FindCurrentWorkflow(unsigned &wfUID, unsigned& taskIndex){
    double highestPriority = 0.0;
    // we should find an index of workflow having the task with highest priority
    for (auto& wf: unsched){
        for (auto& task: wf.second){
            double &priority = task.second.priority;
            if (priority > highestPriority){
                highestPriority = priority;
                wfUID = wf.first;
                taskIndex = task.first;
            }
        }
    }
}

// find cluster
void PCH::FindCluster(unsigned currentWf, unsigned firstTask, vector<unsigned>& cluster){
    unsigned currentTask = firstTask;
    const Workflow& wf = data.Workflows(currentWf);
    taskInfo& tasks = unsched[wf.GetUID()];
    do {
        vector<int> children;
        wf.GetOutput(currentTask, children);
        double highestPriority = 0.0;
        unsigned bestChildNum = 0;
        bool isAnyChildWithAllParentsScheduled = false;
        // find task with highest priority and all parents scheduled
        for (auto& child : children){
            vector<int> parents;
            wf.GetInput(child, parents);
            bool allParentsScheduled = true;
            for (auto& parent: parents){
                if (tasks.count(parent) != 0){
                    allParentsScheduled = false;
                    break;
                }
            }
            if (allParentsScheduled){
                isAnyChildWithAllParentsScheduled = false;
                double& priority = tasks[child].priority;
                if (priority > highestPriority){
                    bestChildNum = child;
                    highestPriority = priority;
                }
            }
        }
        // if all children have unscheduled parents, it is the end of the current cluster
        if (!isAnyChildWithAllParentsScheduled)
            return;
        cluster.push_back(bestChildNum);
        currentTask = bestChildNum;
     } while (!wf.IsPackageLast(currentTask));

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
