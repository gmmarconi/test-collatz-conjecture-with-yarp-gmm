#include <yarp/os/ConstString.h>
#include "collatzServer.h"

using namespace std;
using namespace yarp::os;

double collatzServer::getPeriod()
{
    // module periodicity (seconds), called implicitly by the module.
    return 60;
}
bool collatzServer::updateModule()
{
    //count++;
    //cout << "[" << count << "]" << " updateModule..." << endl;
    return true;
}
bool collatzServer::configure(yarp::os::ResourceFinder &rf)
{
    cout << "Configuring server...\n";
    count = 0;
    CNT   = 1;
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
    printf("Server listening on port %s\n", name.c_str());
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
    cout  << "Interrupting your module, for port cleanup" << endl;
    return true;
}
bool collatzServer::close()
{
    // optional, close port explicitly
    cout << "Calling close function\n";
    //
    sManager.stop();
    yDebug() << "Closed sManager\n";
    rManager.stop();
    yDebug() << "Closed rManager\n";
    port.close();
    yDebug() << "Closed RPC Server\n";
    //
    //handlerPort.close();
    return true;
}
