#include "stdafx.h"
#include "SchedulingFactory.h"

enum Algorithms{ BELLMAN = 1, GREEDY = 2, CLUSTERED = 3, PCH_MERGED = 4, PCH_RR = 5 };

SchedulingFactory::SchedulingFactory(void)
{
}

SchedulingFactory::~SchedulingFactory(void)
{
}

unique_ptr<SchedulingMethod> SchedulingFactory::GetMethod(DataInfo &d, int uid, int wfNum){
	try{
		switch (uid){
		    case BELLMAN:
			    //return unique_ptr<SchedulingMethod> (new BellmanScheme(d, uid, wfNum));
		    case GREEDY:
			    return unique_ptr<SchedulingMethod>(new Greedy(d, uid, wfNum));
		    case CLUSTERED:
			    return unique_ptr<SchedulingMethod>(new Clustered(d,uid));
          case PCH_MERGED:
              return unique_ptr<SchedulingMethod>(new PCH(d,uid));
          case PCH_RR:
              return unique_ptr<SchedulingMethod>(new PCH(d,uid));
		    default:
			    throw UserException("SchedulingFactory::GetMethod() error. No valid algorithm found. Current algorithm uid = " + to_string(uid));
		    }
	}
	catch (UserException& e){
		cout << "error : " << e.what() <<endl;
		std::system("pause");
		exit(EXIT_FAILURE);
	}
}

