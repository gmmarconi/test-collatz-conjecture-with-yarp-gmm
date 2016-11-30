#include <stdlib.h>
#include <stdio.h>
//#include <vector>
//#include <string>
#include <iostream>
#include <iomanip>
#include <yarp/os/Network.h>
#include <yarp/os/RFModule.h>
#include "collatzThreads.h"

using namespace std;
using namespace yarp::os;

class collatzServer:public RFModule
{

    int count;
    int secs;
    int CNT;
    vector<int> *stackP;
    StackMng *sManager;
    ReqsMng *rManager;
    Semaphore *semphP;
    RpcServer *port;

public:
    ~collatzServer()
    {
      delete semphP;
      delete sManager;
      delete rManager;
      delete port;
    }
    double getPeriod()
    {
        // module periodicity (seconds), called implicitly by the module.
        return 10;
    }
    // This is our main function. Will be called periodically every getPeriod() seconds
    bool updateModule()
    {
        count++;
        cout << "[" << count << "]" << " updateModule..." << endl;
        return true;
    }
    // Message handler. Just echo all received messages.
    // bool respond(const Bottle& command, Bottle& reply)
    // {
    //     cout << "Got something, echo is on" << endl;
    //     if (command.get(0).asString() == "quit")
    //         return false;
    //     else
    //         reply = command;
    //     return true;
    // }
    // Configure function. Receive a previously initialized
    // resource finder object. Use it to configure your module.
    // If you are migrating from the old module, this is the function
    // equivalent to the "open" method.
    bool configure(yarp::os::ResourceFinder &rf)
    {
        cout << "Configuring server...\n";
        count = 0;
        CNT = 1;
        string name = rf.find("name").asString().c_str();
        stackP = new vector<int>;
        semphP = new Semaphore();
        semphP->post();
        sManager = new StackMng(10000);
        rManager = new ReqsMng();
        port = new RpcServer();
        port->open(name);
        bool ok = sManager->start(semphP, stackP);
        ok = ok && rManager->start(semphP, stackP, port, CNT);
        if (!ok)
        {
          printf("One of the threads failed to initialize, returning\n");
          return -1;
        }
        // optional, attach a port to the module
        // so that messages received from the port are redirected
        // to the respond method
        // handlerPort.open("collatz-server");
        // attach(handlerPort);
        // cout << "Server configured: /collatz-server port opened\n";
        return true;
    }
    // Interrupt function.
    bool interruptModule()
    {
        cout << "Interrupting your module, for port cleanup" << endl;
        return true;
    }
    // Close function, to perform cleanup.
    bool close()
    {
        // optional, close port explicitly
        cout << "Calling close function\n";
        //
        sManager->stop();
        rManager->stop();
        //
        //handlerPort.close();
        return true;
    }

};
