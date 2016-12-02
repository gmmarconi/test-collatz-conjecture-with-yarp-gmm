#include "collatzThreads.h"

using namespace std;
using namespace yarp::os;

ReqsMng::ReqsMng()
    :Thread()
{
    CNT = 1;
}
// The objects are passed by reference to not fetch copies
// then we deeference the object themselves to feed the memory
// address to the pointers
// This is done because we can't define references or classes
// in the class definition, both should be istantiated upon creation and
// this is not possible
bool ReqsMng::start(Semaphore& smph, vector<int> &stk, RpcServer &prt, int &cnt)
{
    CNT   = cnt;
    semph = &smph;
    stack = &stk;
    port  = &prt;
    return Thread::start();
}
bool ReqsMng::threadInit()
{
    printf("Starting request manager, /collatz-server port opened\n");
    return true;
}
void ReqsMng::run()
{
    while (!isStopping()) {
        port->read(cmd,true);
        if (!cmd.get(1).isInt()) yError() << "ReqsMng: The client didn't send a proper bottle with a number\n";
        // Client is requesting a number to test
        if (cmd.get(1).asInt() == 0)    response = returnPair();
        // Client is telling that cmd.get(0) satisfies collatz conj
        else if (cmd.get(1).asInt() > 0)
        {
            removeNumber(cmd.get(0).asInt());
            response = Bottle("0");
        }
        else yError() << "ReqMng: The number given by the client is negative\n";
        port->write(response);
    }
}
void ReqsMng::threadRelease()
{
    printf("Goodbye from request manager\n");
}
Bottle ReqsMng::returnPair()
{
    if (semph->waitWithTimeout(10))
    {
        Bottle b;
        CNT += 1;
        stack->push_back(CNT);
        b.addInt(CNT);
        b.addInt(stack->front());
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
    if (semph->waitWithTimeout(10))
    {
        for ( vector<int>::iterator iterator = stack->begin(); iterator != stack->end(); ++iterator)
        {
            if  (*iterator == t)
            {
                stack->erase(iterator);
                iterator = stack->end();
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

/// --- StackMng methods --- ///

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
void StackMng::afterStart(bool s)
{
    if (s)
        printf("stack manager started successfully\n");
    else
        printf("stack manager did not start\n");
}
void StackMng::run()
{
    if (semph->waitWithTimeout(1))
    {
        int j = 0;
        printf("Inside Stack check\n");
        for (vector<int>::const_iterator iterator = stack->begin(), end = stack->end(); iterator != end; ++iterator, ++j)
        {
            printf("Element #%d of the stack: %d\n", j, *iterator);
        }
        semph->post();
    }
}
void StackMng::threadRelease()
{
    printf("Goodbye from stack manager\n");
}
