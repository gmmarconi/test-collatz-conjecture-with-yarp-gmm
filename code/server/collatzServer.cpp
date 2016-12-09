#include <yarp/os/ConstString.h>
#include "collatzServer.h"

using namespace std;
using namespace yarp::os;

double collatzServer::getPeriod()
{
    // module periodicity (seconds), called implicitly by the module.
    return 1;
}
bool collatzServer::updateModule()
{
    return true;
}
bool collatzServer::configure(yarp::os::ResourceFinder &rf)
{
    printf("Configuring server...\n");
    count     = 0;
    CNT       = 1;
    semph.post();
    ConstString name;
    if ( rf.check("name") )
    {
        name = rf.find("name").asString();
    }
    else
    {
        name = "/collatzS";
    }
    port.open(name);
    bool ok = sManager.setRate(10000);
    ok = sManager.start(semph, stack);
    ok = ok && rManager.start(semph, stack, port, CNT);
    if (!ok)
    {
        yError() << "One of the threads failed to initialize, returning\n";
        return false;
    }
    return true;
}
bool collatzServer::interruptModule()
{
    printf("Closing Server\n");
    return true;
}
bool collatzServer::close()
{
    printf("Calling close function\n");
    sManager.stop();
    //printf("bleep\n");
    rManager.stop();
    //printf("blop\n");
    port.close();
    return true;
}
