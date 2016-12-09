#include "collatzThreads.h"

using namespace std;
using namespace yarp::os;

ReqsMng::ReqsMng()
    :Thread()
{
    CNT = 1;
    cmd.addInt(0);
}
ReqsMng::~ReqsMng()
{
    port = 0;
}
// The objects are passed by reference to not fetch copies;
// we then deeference the object themselves to feed the memory
// address to the pointers
// This is done because we can't define references or classes
// in the class definition, both should be istantiated upon creation and
// this is not possible
// The pass-by-reference operator is private (unaccessible) for RPC Servers
// in YARP, so we get the pointer
bool ReqsMng::start(Semaphore &smph, vector<int> &stk, RpcServer &prt, int &cnt)
{
    CNT   = cnt;
    semph = &smph;
    stack = &stk;
    port  = &prt;
    return Thread::start();
}
bool ReqsMng::threadInit()
{
    printf("Starting request manager, collatz-server port opened\n");
    return true;
}
void ReqsMng::run()
{
    while (!isStopping()) {
        //printf("Before %s\n", isStopping() ? "true" : "false");
        port->read(cmd,true);
        //printf("After %s\n", isStopping() ? "true" : "false");
        // Client is requesting a number to test
        if ((cmd.get(1).isInt()) && (cmd.get(1).asInt() == 0))
        {
            printf("Client #%d sent a request for a number\n", cmd.get(0).asInt());
            response = returnPair();
            port->reply(response);
        }
        // Client is telling that cmd.get(0) satisfies collatz conj
        else if ((cmd.get(1).isInt()) && (cmd.get(1).asInt() > 0))
        {
            int id = cmd.get(0).asInt();
            int N  = cmd.get(1).asInt();
            printf("Client #%d asserted that number %d converges\n", id, N);
            removeNumber(N);
            response = Bottle("0 0");
            port->reply(response);
        }
        else if (cmd.get(0) == 0)
        {
            break;
        }

    }
}
void ReqsMng::threadRelease()
{
    printf("Goodbye from request manager\n");
}

void ReqsMng::close()
{
    printf("Closing Request Manager\n");
}

Bottle ReqsMng::returnPair()
{
    if (semph->waitWithTimeout(1))
    {
        Bottle b;
        CNT += 1;
        stack->push_back(CNT);
        b.addInt(CNT);
        b.addInt((stack->front()) - 1);
        semph->post();
        return b;
    }
    else
    {
        yError() << "ReqsMng: the semaphore remained red for too long\n";
        std::exit;
    }
}
void ReqsMng::removeNumber(int t)
{
    if (semph->waitWithTimeout(1))
    {
        //yDebug () << "Removing number " << t << " from the stack\n";
        for ( vector<int>::iterator iterator = stack->begin(); iterator != stack->end(); ++iterator)
        {
            if  (*iterator == t)
            {
                stack->erase(iterator);
                break;
            }
        }
        semph->post();
    }
    else
    {
        yError() << "ReqsMng: the semaphore remained red for too long\n";
        std::exit;
    }
}

///--------------------------///
/// --- StackMng methods --- ///
///--------------------------///

StackMng::StackMng(int r):RateThread(r){}

bool StackMng::start(Semaphore &smph, vector<int> &stk)
{
    semph = &smph;
    stack = &stk;
    return RateThread::start();
}
bool StackMng::threadInit()
{
    printf("Starting stack manager\n");
    return true;
}
void StackMng::run()
{
    if (semph->waitWithTimeout(1))
    {
        int j = 0;
        for (vector<int>::const_iterator iterator = stack->begin(), end = stack->end(); iterator != end; ++iterator, ++j)
        {
            printf("Element #%d of the stack: %d\n", j+1, *iterator);
        }
        semph->post();
    }
}
void StackMng::threadRelease()
{
    printf("Goodbye from stack manager\n");
}
