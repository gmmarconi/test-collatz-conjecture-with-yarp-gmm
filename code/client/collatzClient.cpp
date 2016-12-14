#include "collatzClient.h"
#include <yarp/os/Time.h>
#include <yarp/os/Log.h>
#include <climits>

using namespace std;
using namespace yarp::os;

collatzClient::collatzClient()
{
    N         = 0;
    n         = 0;
    T         = 0;
    period    = 0.01;
    client_id = 0;
}
double collatzClient::getPeriod()
{
    // module periodicity (seconds), called implicitly by the module.
    return period;
}
bool collatzClient::updateModule()
{
    if (n == 0)
    {
        sent_b.clear();
        sent_b.addInt(client_id);
        sent_b.addInt(0);
        // Sending the request for a couple of numbers
        port.write(sent_b,received_b);
        N = received_b.get(0).asInt();
        n = N;
        T = received_b.get(1).asInt();
        printf("Received from server the number %d to check against threshold %d\n", N, T);
    }
    if (n > T)
    {
        if ((n % 2) == 0)   n = n / 2;
        else                n = (n * 3) + 1;
    }
    else
    {
        Bottle final_b;
        Bottle ack_b;
        final_b.addInt(client_id);
        final_b.addInt(N);
        port.write(final_b, ack_b);
        n = 0;
        printf("Number %d converged, client #%d is requesting another number now.\n\n", N, client_id);
        //Time::delay(2);
        //close();
    }

    return true;
}
bool collatzClient::configure(yarp::os::ResourceFinder &rf)
{
    cout << "Configuring client...\n";
    ConstString name;
    name = rf.check("name", Value("/collatzC"), "Getting Client port name").asString();
    ConstString server_name;
    server_name = rf.check("server_name", Value("/collatzS"), "Getting Server port name").asString();
    if (rf.check("period"))
    {
        period = rf.find("period").asInt();
    }
    else
    {
        period = 0.5;
    }
    if (!yarp.checkNetwork()) { yError("unable to find YARP server!"); return false; }
    port.open(name);
    if(rf.check("auto-connect") && rf.find("auto-connect").asBool() == true )
    {
        if (!yarp.connect(name, server_name))
        {
            printf("Failed network connection to port %s\n", server_name.c_str());
        }
    }
    else
    {
        while(port.getOutputCount() == 0)
        {
            printf("Client waiting to be connected to a server\n");
            Time::delay(10);
            // waits to be connected
        }
    }
    // Prepare request bottle:
    // sent_b  =  || ID | 0 ||
    long long_client_id = rf.find("name").asString().hash();
    long_client_id = long_client_id + Time::now();
    client_id = static_cast<int>(long_client_id & INT_MAX);
    printf("Server id: %d \t Writing on port: \"%s\" \t Connected to server port: \"%s\"\n", client_id, name.c_str(), server_name.c_str());
    return true;
}
bool collatzClient::interruptModule()
{
    printf("Interrupting client\n");
    if (port.asPort().isOpen()) port.interrupt();
    return true;
}
bool collatzClient::close()
{
    if (port.asPort().isOpen()) port.close(); // always check if a port is open before closing it
    printf("Closed RPC Server\n");
    return true;
}
