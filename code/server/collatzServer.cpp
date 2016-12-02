#include <yarp/os/ConstString.h>
#include "collatzServer.h"

using namespace std;
using namespace yarp::os;

double collatzServer::getPeriod()
{
    // module periodicity (seconds), called implicitly by the module.
    return 10;
}
bool collatzServer::updateModule()
{
    count++;
    cout << "[" << count << "]" << " updateModule..." << endl;
    return true;
}
bool collatzServer::configure(yarp::os::ResourceFinder &rf)
{
    cout << "Configuring server...\n";
    count = 0;
    CNT = 1;
    ConstString name;
    if ( rf.check("name") )
    {
        name = rf.find("name").asString().c_str();
    }
    else
    {
        name = "/birillo";
    }
    //stack       = vector<int>();
    semph.post();
    port.open(name);
    cout << "Bleep\n";
    bool ok = sManager.setRate(5000);
    ok = sManager.start(semph, stack);
    ok = ok && rManager.start(semph, stack, port, CNT);
    if (!ok)
    {
        yError() << "One of the threads failed to initialize, returning\n";
        return -1;
    }
    cout << "blorp\n";
    // optional, attach a port to the module
    // so that messages received from the port are redirected
    // to the respond method
    // handlerPort.open("collatz-server");
    // attach(handlerPort);
    // cout << "Server configured: /collatz-server port opened\n";
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
