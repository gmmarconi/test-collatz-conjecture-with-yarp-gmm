#include <vector>
#include <stdio.h>
#include <string>
#include <yarp/os/LogStream.h>
#include <yarp/os/Time.h>
#include <yarp/os/Thread.h>
#include <yarp/os/RateThread.h>
#include <yarp/os/Semaphore.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/ConstString.h>

using namespace std;
using namespace yarp::os;

class ReqsMng : public Thread {
public:

  int CNT;
  Semaphore *semphP;
  vector<int> *stackP;
  RpcServer *port;
  string name;
  Bottle cmd;
  Bottle response;

  ReqsMng()
  :Thread()
  {
    semphP = 0;
    stackP = 0;
    port = 0;
    CNT = 1;
  }
  bool start(Semaphore *smph, vector<int> *stk, RpcServer *prt, int &cnt)
  {
    CNT = cnt;
    bool ok;
    ok = Thread::start();
    if (smph && stk && prt)
    {
      semphP = smph;
      stackP = stk;
      port = prt;
      return ok;
    }
    yError() << "ReqMng: Passed an uninitiliazed pointer\n";
    return false;
  }
  virtual bool threadInit()
  {
    printf("Starting request manager, /collatz-server port opened\n");
    return true;
  }
  virtual void run() {
    while (!isStopping()) {
      port->read(cmd,true);
      if ( cmd.get(1).asInt() == 0 ) // Client is requesting a number to test
      {
        response = returnPair();
      }
      else if ( cmd.get(1).asInt() > 0) // Client is telling that cmd.get(0) satisfies collatz conj
      {
        removeNumber(cmd.get(0).asInt());
        response = Bottle("0");
      }
      else
      {
        yError() << "ReqMng: The number given by the client is negative\n";
      }
      port->write(response);
    }
  }
  virtual void threadRelease()
  {
    printf("Goodbye from request manager\n");
  }
private:
  Bottle returnPair()
  {
    if (semphP->waitWithTimeout(10))
    {
      Bottle b;
      CNT +=1;
      stackP->push_back(CNT);
      b.addInt(CNT);
      b.addInt(stackP->front());
      semphP->post();
      return b;
    }
    else
    {
      yError() << "ReqsMng: the semaphore remained red for too long\n";
      std::exit;
    }
  }
  void removeNumber(int t)
  {
    if (semphP->waitWithTimeout(10))
    { // check the ++iterator, is it correct?
      for (vector<int>::const_iterator iterator = stackP->begin(), end = stackP->end(); iterator != end; ++iterator)
      {
        if  (*iterator == t)
        {
          stackP->erase(iterator);
          iterator = stackP->end();
        }
      }
      semphP->post();
    }
    else
    {
      yError() << "ReqsMng: the semaphore remained red for too long\n";
      std::exit;
    }
  }
};

class StackMng : public RateThread {
public:

  Semaphore *semphP;
  vector<int> *stackP;

  StackMng(int r)
  :RateThread(r)
  {
    semphP = 0;
    stackP = 0;
  }
  bool start( Semaphore *smph, vector<int> *stk)
  {
    bool ok;
    ok = RateThread::start();
    if (smph && stk)
    {
      semphP = smph;
      stackP = stk;
      return ok;
    }
    yError() << "StackMng: Passed an uninitiliazed pointer\n";
    return false;
  }
  virtual bool threadInit()
  {
    printf("Starting stack manager\n");
    return true;
  }
  //called by start after threadInit, s is true iff the thread started
  //successfully
  virtual void afterStart(bool s)
  {
    if (s)
    printf("stack manager started successfully\n");
    else
    printf("stack manager did not start\n");
  }
  virtual void run()
  {
    if (semphP->waitWithTimeout(1))
    {
      cout << "Inside Stack check\n";
      for (vector<int>::const_iterator iterator = stackP->begin(), end = stackP->end(); iterator != end; ++iterator)
      {
        cout << *iterator << "\n";
      }
      semphP->post();
    }
    printf("Hello, from stack manager\n");
  }
  virtual void threadRelease()
  {
    printf("Goodbye from stack manager\n");
  }
};
