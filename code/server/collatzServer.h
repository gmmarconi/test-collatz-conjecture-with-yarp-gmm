#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <yarp/os/Network.h>
#include <yarp/os/RFModule.h>
#include "collatzThreads.h"

class collatzServer:public yarp::os::RFModule
{
    int  count;
    int  secs;
    int  CNT;
    std::vector<int>        stack;
    StackMng                sManager;
    ReqsMng                 rManager;
    yarp::os::Semaphore     semph;
    yarp::os::RpcServer     port;
    yarp::os::Network       yarp;

public:

    collatzServer(){}

    ~collatzServer(){}

    double getPeriod();

    // This is our main function. Will be called periodically every getPeriod() seconds
    bool updateModule();

    // Configure function. Receive a previously initialized
    // resource finder object.
    bool configure(yarp::os::ResourceFinder &rf);

    // Interrupt function.
    bool interruptModule();

    // Close function, to perform cleanup.
    bool close();
};
