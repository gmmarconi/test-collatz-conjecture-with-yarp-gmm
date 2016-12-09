//#ifndef COLLATZCLIENT_H
//#define COLLATZCLIENT_H

#include <stdio.h>
#include <yarp/os/Network.h>
#include <yarp/os/RpcClient.h>
#include <yarp/os/RFModule.h>

class collatzClient:public yarp::os::RFModule
{
    int                     N;
    int                     n;
    int                     T;
    double                  period;
    int                     client_id;
    yarp::os::Network       yarp;
    yarp::os::Bottle        sent_b;
    yarp::os::Bottle        received_b;
    yarp::os::RpcClient     port;

public:

    collatzClient();

    ~collatzClient(){}

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

//#endif // COLLATZCLIENT_H
