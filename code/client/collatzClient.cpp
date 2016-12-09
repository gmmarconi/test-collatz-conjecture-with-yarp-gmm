#include "collatzClient.h"
#include <yarp/os/Time.h>
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
    if (rf.check("name"))
    {
        name = rf.find("name").asString().c_str();
    }
    else
    {
        name = ConstString("/collatzC").c_str();
    }
    ConstString server_name;
    if (rf.check("server_name"))
    {
        server_name = rf.find("server_name").asString().c_str();
    }
    else
    {
        server_name = ConstString("/collatzS").c_str();
    }
    if (rf.check("period"))
    {
        period = rf.find("period").asInt();
    }
    else
    {
        period = 0.01;
    }
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
        while(port.getInputCount() == 0)
        {
            printf("Client waiting to be connected to a server\n");
            Time::delay(1);
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
    cout  << "Interrupting your module, for port cleanup" << endl;
    return true;
}
bool collatzClient::close()
{
    port.close();
    printf("Closed RPC Server\n");
    return true;
}
