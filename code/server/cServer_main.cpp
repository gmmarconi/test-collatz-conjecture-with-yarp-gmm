#include "collatzServer.h"
#include <yarp/os/all.h>


int main(int argc, char * argv[])
{
    if (argc<2) {
        fprintf(stderr, "Please supply a port name for the server\n");
        return 1;
    }

    /* initialize yarp network */
    yarp::os::Network yarp;
    /* create your module */
    collatzServer Serv;
    /* prepare and configure the resource finder */
    yarp::os::ResourceFinder rf;
    rf.configure(argc, argv);
    rf.setVerbose(true);
    std::cout << "Configuring and starting module. \n";
    Serv.runModule(rf);   // This calls configure(rf) and, upon success, the module execution begins with a call to updateModule()
    std::cout <<"Main returning..."<< std::endl;
    return 0;
}
