#include "../lib/CommandHandler.h"
#include "../lib/Network.h"

using namespace std;

int main()
{
    Network* network = new Network();
    CommandHandler commandHandler = CommandHandler(network);
    string choosenService;
    while(getline(cin, choosenService))
    {
        try
        {
            commandHandler.getQuery(choosenService);
        }
        catch(...){}
    }
    return 0;
}