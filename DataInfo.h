#include "Workflow.h"
#include "ResourceType.h"
#include "ModelingContext.h"
#include "memory.h"


using namespace std;

typedef vector <TimeCore> AllTimeCore;

#pragma once
class DataInfo
{
	// friend classes
	friend class ScheduleToXML;
	// ATTRIBUTES
	// modeling time characteristics
	ModelingContext context;
	// vector of init global package number for wf
	vector<int> initPackageNumbers;
	// vector of model workflows
	vector <Workflow> workflows;
	// vector of model resource types
	vector <ResourceType> resources;
	// priorities according to finishing time (vector of package numbers)
	vector <int> priorities;
	// sum of resource types processors count
	int processorsCount;
	// min length for experiments
	double minL;
   // koefficient for wxperiments
   double koeff;
   // bandwidth matrix
   vector<vector<double>> bandwidth;
	//OPERATIONS
	// init data placement settings
	void Init(string fName);
	// init resources
	void InitResources(string fName, bool canExecuteOnDiffResources);
   // init bandwidth speed
	void InitBandwidth(string fName);
	// init workflows
	void InitWorkflows(string fName);
	// init workflows from DAX
	void InitWorkflowsFromDAX(string fName);
	// init finishing times
	void InitFinishingTimes();
	// setting data transfer size according to CCR, h and average resources' power
   void  SetTransferValues();
   // get average performance of resources
   double GetAvgPerf();
   // get maximum performance
   double GetMaxPerf();
  
public:
	DataInfo(){}
	DataInfo(string fSettings, double mL, double k);
	// get WF count
	inline int GetWFCount() {return workflows.size();}
	inline int GetResourceCount() {return resources.size(); }
	// get full cores count
	inline int GetprocessorsCount() {return processorsCount;}
	int GetResourceType (int coreNumber);
	void FixBusyIntervals();
	void ResetBusyIntervals();
	void SetInitBusyIntervals();
	void GetCurrentIntervals(vector<vector<BusyIntervals>> &storedIntervals);
	void SetCurrentIntervals(vector<vector<BusyIntervals>> &storedIntervals);
	// getter 
	const vector<Workflow> & Workflows() const { return workflows; }
	const vector<ResourceType> & Resources() const { return resources; }
	const Workflow& Workflows(int wfNum) const ;
	ResourceType& Resources(int resNum);
   // get number of resources types
   unsigned GetTypesCount() {return resources.size();}
	double GetCCR() {return context.GetCCR();}
	// get initial processor index of resource type
	int GetInitResourceTypeIndex(int type);
	// get all packages count
	unsigned int GetPackagesCount();
	// set priorities to whole packages (packages numbered from zero according to wf order)
	void SetPriorities();
	// getting next package with smallest finishing time
	int GetNextPackage();
	// get wfNum and local package number for global package number
	void GetLocalNumbers (const int &current, int &wfNum, int &localNum) const;
	int GetT() {return context.GetT();}
	void SetT(double newT) {context.SetT(newT);}
	// get global processor index 
	int GetGlobalProcessorIndex(int resource, int local);
	// get resource type by global index
	int GetResourceTypeIndex(int globalIndex);
	// set different wf priorities
	void SetWfPriorities();
	// get init package number by wfNum
	int GetInitPackageNumber(int wfNum);
	// remove some numbers from priorities
	void RemoveFromPriorities(const vector<int>& toRemove);
	const double GetDeadline(int wfNum);
	double GetDeadline();
   double GetTStart();
	int GetPrioritiesSize() {return priorities.size();}
    double GetBandwidth(const int& from, const int& to) const;
   // get average transfer time from globalPackage to all its childs
    double GetAvgTransferFrom(const int& globalPackage) const;
	// get maximum exec time of last task of wfNum (for fine calculating)
	double GetMaxLastTasksExecTime(int wfNum) const;
   // merge all workflows into one and add it to the end of vector of workflows
   void MergeWorkflows();
   // delete last workflow from the vector of workflows
   void DeleteLastWorkflow();
   // store connectivity matrix to file
   void PrintConnectivityMatrixToFile(unsigned wfNum);
   // store transfer matrix to file
   void PrintTransferMatrixToFile(unsigned wfNum);
   // get maximum bandwidth
   double GetMaxBandwidth();
	~DataInfo(void);
};

