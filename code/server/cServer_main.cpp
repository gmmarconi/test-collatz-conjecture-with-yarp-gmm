#include "collatzServer.h"
#include <yarp/os/all.h>


int main(int argc, char * argv[])
{
    /* initialize yarp network */
    //yarp::os::Network yarp;
    /* create your module */
    collatzServer Serv;
    /* prepare and configure the resource finder */
    yarp::os::ResourceFinder rf;
    rf.configure(argc, argv);
    rf.setVerbose(true);
    std::cout << "Configuring and starting module. \n";
    return  Serv.runModule(rf);   // This calls configure(rf) and, upon success, the module execution begins with a call to updateModule();
}
