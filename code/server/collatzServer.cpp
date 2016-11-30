#include "collatzServer.h"
#include <yarp/os/all.h>


int main(int argc, char * argv[])
{
  if (argc<2) {
      fprintf(stderr, "Please supply a port name for the server\n");
      return 1;
  }

    /* initialize yarp network */
    Network yarp;
    /* create your module */
    collatzServer Serv;
    /* prepare and configure the resource finder */
    ResourceFinder rf;
    rf.configure(argc, argv);
    rf.setVerbose(true);
    cout << "Configuring and starting module. \n";
    Serv.runModule(rf);   // This calls configure(rf) and, upon success, the module execution begins with a call to updateModule()
    cout<<"Main returning..."<<endl;
    return 0;
}
