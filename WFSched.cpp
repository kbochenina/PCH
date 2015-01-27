// WFSched.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DataInfo.h"
#include "Scheduler.h"
#include <cstdlib>
#include <stdlib.h>
#include "direct.h"
#include "windows.h"


using namespace std;

enum SchedulingTypes { ONLY_BELLMAN = 1, ONLY_GREEDY = 2, CLUST = 3, PCH_M = 4, PCH_R = 5 };
enum SchedulingSchemes { STAGED = 1, EFF_ORDERED = 2, SIMPLE = 3, RESERVED_ORDERED = 4, CLUSTERED = 5, PCH_MERGE = 6, PCH_RR = 7};


int _tmain(int argc, wchar_t**argv)
{
	// settings (minLength, periodsCount, experCount) are read from command line
   // other settings are in the file "settings.txt" and are used by DataInfo::Init()
   double minLInit = 0.0, koeff = 0.0;
	int periodsCount = 1, experCount = 1;
   _chdir("D:\\ITMO\\Degree\\Programs\\Clustered\\Clustered");
   if (argc != 5){
       cout << "Too few arguments (4 required).\nSyntax: WFSched minLengthInit koeff periodsCount experCount" << endl;
       system("pause");
       exit (100);
   }
   minLInit = _wtoi(argv[1]);
   koeff = _wtof(argv[2]);
	periodsCount = _wtoi(argv[3]);
   experCount = _wtoi(argv[4]);
	
	srand(time(NULL));
	
	for (int i = 0; i < periodsCount; i++){
		// set data
		double minLength = minLInit + koeff *i * minLInit;
		cout << "Min length = " << minLength << endl;
     //system("pause");
      for (int j = 0; j < experCount; j++){
          _chdir("D:\\ITMO\\Degree\\Programs\\Clustered\\Clustered\\Output");
          string metricsFileName = "fullmetrics";
          metricsFileName.append(to_string(i+1));
          metricsFileName.append("-");
          metricsFileName.append(to_string(j+1));
          metricsFileName.append(".txt");
          ofstream m(metricsFileName, ios::trunc);
		    m.close();
          _chdir("D:\\ITMO\\Degree\\Programs\\Clustered\\Clustered");
		    DataInfo data("settings.txt", minLength, koeff);
		    Scheduler sched(data);
		    ofstream f("fullmetrics.txt", ios::trunc);
		    f.close();
		    sched.SetSchedulingStrategy(ONLY_GREEDY);	
		    sched.GetSchedule(SIMPLE);
		    sched.GetMetrics("simple_metrics.txt", "SimpleSched",metricsFileName);
		    sched.TestSchedule();
		    cout << "***************************************************" << endl;
		    sched.GetSchedule(RESERVED_ORDERED);
		    sched.GetMetrics("reserved_metrics.txt", "StagedReservedTime",metricsFileName);
		    sched.TestSchedule();
		    cout << "***************************************************" << endl;
		    /*sched.GetSchedule(EFF_ORDERED);
		    sched.GetMetrics("eff_metrics.txt", "StagedEfficiency");
		    sched.TestSchedule();
		    cout << "***************************************************" << endl;*/
		    sched.SetSchedulingStrategy(CLUST);
		    sched.GetSchedule(CLUSTERED);
		    sched.GetMetrics("clustered.txt", "Clustered", metricsFileName);
		    sched.TestSchedule();
		    cout << "***************************************************" << endl;
          //  PCH + merge-based
          sched.SetSchedulingStrategy(PCH_M);
		    sched.GetSchedule(PCH_MERGE);
		    sched.GetMetrics("pch_merge.txt", "PCH_MERGE", metricsFileName);
		    sched.TestSchedule();
		    cout << "***************************************************" << endl;
          //  PCH + round-robin
          sched.SetSchedulingStrategy(PCH_R);
		    sched.GetSchedule(PCH_RR);
		    sched.GetMetrics("pch_rr.txt", "PCH_ROUND-ROBIN", metricsFileName);
		    sched.TestSchedule();
		    cout << "***************************************************" << endl;
		    _chdir("D:\\ITMO\\Degree\\Programs\\Clustered\\Clustered");
      }
      
	}
	/*sched.GetSchedule(STAGED);
	sched.GetMetrics("staged_metrics.txt");
	sched.TestSchedule();*/
	//system("pause");
	return 0;
}

