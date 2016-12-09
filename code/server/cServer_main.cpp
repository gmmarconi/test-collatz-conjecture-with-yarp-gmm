#include "collatzServer.h"
#include <yarp/os/all.h>


int main(int argc, char * argv[])
{
    /* initialize yarp network */
    yarp::os::Network yarp;
    if (!yarp.checkNetwork()) { yError("unable to find YARP server!"); return 1; }
    /* create your module */
    collatzServer Serv;
    /* prepare and configure the resource finder */
    yarp::os::ResourceFinder rf;
    rf.configure(argc, argv);
    rf.setVerbose(true);
    return  Serv.runModule(rf);   // This calls configure(rf) and, upon success, the module execution begins with a call to updateModule();
}
