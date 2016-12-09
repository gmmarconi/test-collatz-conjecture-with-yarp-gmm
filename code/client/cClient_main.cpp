#include "collatzClient.h"
#include <yarp/os/all.h>


int main(int argc, char * argv[])
{
    if (argc<2) {
        printf("Default port name will be /birillo\n");
    }
    /* initialize yarp network */
    //yarp::os::Network yarp;
    /* create your module */
    collatzClient Client;
    /* prepare and configure the resource finder */
    yarp::os::ResourceFinder rf;
    rf.configure(argc, argv);
    rf.setVerbose(true);
    std::cout << "Configuring and starting module. \n";
    return  Client.runModule(rf);   // This calls configure(rf) and, upon success, the module execution begins with a call to updateModule();
}
