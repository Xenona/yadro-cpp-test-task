#include "Event/Event.h"
#include "Event/EventVisitor.h"
#include "Utils/utils.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <variant>
#include <algorithm>

using namespace std;

int main()
{

    // reading file
    string pathToFile;
    cout << "Input path to file: " << endl;
    // TODO:
    // cin >> pathToFile;
    pathToFile = "test.txt";
    ifstream file(pathToFile);
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file: " << pathToFile << std::endl;
        return 1;
    }

    // solving
    Statistics *stats = new Statistics();
    EventVisitor eventVisitor(stats);
    auto events = eventVisitor.parseEvents(file);

    cout << getTimeFromMins(stats->timeStart) << endl;

    for (const auto &event : events)
    {
        std::visit(eventVisitor, event);
    }

    if (stats->clientsInside.size() != 0)
    {
        std::sort(stats->clientsInside.begin(), stats->clientsInside.end());
        for (string &clientName : stats->clientsInside)
        {
            printEvent(stats->timeEnd, EventID::clientKickedOut, clientName);

            int tableID = clientHadTable(stats->tablesUsed, clientName);
            if (tableID)
            {

                int timeDiff = stats->timeEnd - stats->tablesUsed[tableID].lastTimeTaken;

                stats->tablesUsed[tableID].timeTotal += timeDiff;
                stats->tablesUsed[tableID].gain += ceil(timeDiff / 60.0) * stats->costPerHour;
                stats->tablesUsed[tableID].isTaken = false;
            }

            removeClient(stats->clientsInside, clientName);
        }
    }

    cout << getTimeFromMins(stats->timeEnd) << endl;
    
    for (int i = 1; i <= stats->numOfTables; i++)
    {
        if (stats->tablesUsed[i].lastTakenBy != "")
        {
            cout
                << i
                << " "
                << stats->tablesUsed[i].gain
                << " "
                << getTimeFromMins(stats->tablesUsed[i].timeTotal)
                << endl;
        }
    }

    return 0;
}