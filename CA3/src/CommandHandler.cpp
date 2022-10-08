#include "../lib/CommandHandler.h"

using namespace std;
using namespace std::chrono;


vector<string> CommandHandler::splitLine(std::string &choosenService, char splitor)
{
    vector<string> words;
    stringstream iss(choosenService);
    string currentWord;

    while(getline(iss,currentWord,splitor))
        words.push_back(currentWord);

    return words;
}

bool CommandHandler::topologyIsValid()
{
    vector<string> connection;
    int count = wordsOfLine.size();
    vector<vector<int>> link(CN_NODE_COUNT+1, vector<int>(CN_NODE_COUNT+1, -1));

    for (int i = 1; i < count; i++)
    {
        connection = splitLine(wordsOfLine[i], D_SPLITOR);
        if (connection[FIRST_NODE] == connection[SECOND_NODE])
            throw IDsAlike();
        int src = stoi(connection[FIRST_NODE]) -1;
        int des = stoi(connection[SECOND_NODE]) -1;
        int cost = stoi(connection[COST]);
        link[src][des] = cost;
        link[des][src] = cost;
    }
    for (int i = 0; i < CN_NODE_COUNT; i++)
        link[i][i] = 0;

    route = link;
    return true;
}

bool CommandHandler::showIsValid()
{
    // if (showLength == NO_TOPOLOGY) // we don't need to check this
        // throw ShowEmptyTable();
    
    return true;
}

bool CommandHandler::lsrpIsValid()
{
    return true;

}

bool CommandHandler::dvrpIsValid()
{
    return true;

}

bool CommandHandler::modifyIsValid()
{
    modifyInfo = splitLine(wordsOfLine[MODIFY_IDX], D_SPLITOR);

    if (modifyInfo[FIRST_NODE] == modifyInfo[SECOND_NODE])
        throw ModifySameID();

    return true;
}

bool CommandHandler::removeIsValid()
{
    return true;
}

void CommandHandler::topology()
{
    // call network class pointer to implement CN
    network->implementCN(route);

}

void CommandHandler::show()
{
    network->showTopology();
}

void CommandHandler::lsrp()
{
    if (wordsOfLine.size() == NO_ARG)
    {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < CN_NODE_COUNT; i++)
        {
            cout << "lsrp for node "<< i+1<< ":" << endl;
            network->runLSRP(i);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by LSRP function: "<< duration.count() << " microseconds" << endl;
    }
    else
    {
        network->runLSRP(stoi(wordsOfLine[NO_ARG])-1);
    }
}

void CommandHandler::dvrp()
{
    if (wordsOfLine.size() == NO_ARG)
    {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < CN_NODE_COUNT; i++)
        {
            cout << "dvrp for node "<< i+1<< ":" << endl;
            network->runDVRP(i);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by DVRP function: "<< duration.count() << " microseconds" << endl;
    }
    else
    {
        network->runDVRP(stoi(wordsOfLine[NO_ARG])-1);
    }

}

void CommandHandler::modify()
{
    //call network modify function
    network->modifyCost(stoi(modifyInfo[FIRST_NODE])-1, stoi(modifyInfo[SECOND_NODE])-1, stoi(modifyInfo[COST]));
}

void CommandHandler::remove()
{
    vector<string> removeID = splitLine(wordsOfLine[REMOVE_IDX], D_SPLITOR);
    network->removeConnection(stoi(removeID[FIRST_NODE])-1, stoi(removeID[SECOND_NODE])-1);

}


void CommandHandler::diagnoseQuery()
{
    if (wordsOfLine[QUERY_TYPE] == TOPOLOGY)
        topologyIsValid() ? topology() : throw NotFoundError() ;    
    else if (wordsOfLine[QUERY_TYPE] == SHOW)
        showIsValid() ? show() : throw NotFoundError() ;
    else if (wordsOfLine[QUERY_TYPE] == LSRP)
        lsrpIsValid() ? lsrp() : throw NotFoundError() ;
    else if (wordsOfLine[QUERY_TYPE] == DVRP)
        dvrpIsValid() ? dvrp() : throw NotFoundError() ;
    else if (wordsOfLine[QUERY_TYPE] == MODIFY)
        modifyIsValid() ? modify() : throw NotFoundError() ;
    else if (wordsOfLine[QUERY_TYPE] == REMOVE)
        removeIsValid() ? remove() : throw NotFoundError() ;
    else
        throw BadRequestError();
}

void CommandHandler::getQuery(string query)
{
    wordsOfLine = splitLine(query, S_SPLITOR);

    if (wordsOfLine.empty())       
        throw BadRequestError();

    diagnoseQuery();
}
