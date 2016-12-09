#include "collatzClient.h"
#include <yarp/os/all.h>


int main(int argc, char * argv[])
{
    /* initialize yarp network */
    /* create your module */
    collatzClient Client;
    /* prepare and configure the resource finder */
    yarp::os::ResourceFinder rf;
    rf.configure(argc, argv);
    rf.setVerbose(true);
    return  Client.runModule(rf);   // This calls configure(rf) and, upon success, the module execution begins with a call to updateModule();
}
