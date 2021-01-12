#include "EchoServer.h"
#include "EchoServantImp.h"

using namespace std;

EchoServer g_app;

/////////////////////////////////////////////////////////////////
void
EchoServer::initialize()
{
    //initialize application here:
    //...

    addServant<EchoServantImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".EchoServantObj");
}
/////////////////////////////////////////////////////////////////
void
EchoServer::destroyApp()
{
    //destroy application here:
    //...
}
/////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
    try
    {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception& e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////
