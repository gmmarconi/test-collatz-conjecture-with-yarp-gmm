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
#include <yarp/os/RpcServer.h>


//using namespace yarp::os;

class ReqsMng : public yarp::os::Thread
{
    int CNT;
    // Since we can't define references or classes in the class definition
    // the next tre objects are defined as pointers that will be initialized
    // with the addresses of the respective objects
    yarp::os::Semaphore*    semph;
    std::vector<int>*       stack;
    yarp::os::RpcServer*    port;
    std::string             name;
    yarp::os::Bottle        cmd;
    yarp::os::Bottle        response;

public:

    ReqsMng();

    ~ReqsMng();

    bool start(yarp::os::Semaphore &smph, std::vector<int> &stk, yarp::os::RpcServer &prt, int &cnt);

    virtual bool threadInit();

    virtual void run();

    virtual void threadRelease();

    virtual void close();

private:
    yarp::os::Bottle returnPair();

    void removeNumber(int t);
};

class StackMng : public yarp::os::RateThread {
public:

    yarp::os::Semaphore*    semph;
    std::vector<int>*  stack;

    StackMng(int r = 500);

    bool start(yarp::os::Semaphore &smph, std::vector<int> &stk);

    virtual bool threadInit();

    virtual void run();

    virtual void threadRelease();
};
