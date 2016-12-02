#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <yarp/os/Network.h>
#include <yarp/os/RFModule.h>
#include "collatzThreads.h"

class collatzServer:public yarp::os::RFModule
{
    int count;
    int secs;
    int CNT;
    std::vector<int>        stack;
    StackMng                sManager;
    ReqsMng                 rManager;
    yarp::os::Semaphore     semph;
    yarp::os::RpcServer               port;
public:
    collatzServer() {}
    ~collatzServer() {}
    double getPeriod();

    // This is our main function. Will be called periodically every getPeriod() seconds
    bool updateModule();

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
    bool configure(yarp::os::ResourceFinder &rf);

    // Interrupt function.
    bool interruptModule();

    // Close function, to perform cleanup.
    bool close();
};
