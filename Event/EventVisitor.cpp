#include "EventVisitor.h"
#include <cmath>
#include <fstream>
#include "../Utils/utils.h"
#include "Event.h"

using namespace std;

EventVisitor::EventVisitor(Statistics *stats) : stats(stats) {}

EventVisitor::~EventVisitor()
{
    delete stats;
}

vector<Event> EventVisitor::parseEvents(ifstream &file)
{
    vector<Event> events;

    {
        // reading number of tables
        string stringNumOfTables;
        tryReadLine(file, stringNumOfTables);
        if (hasNonNumericChars(stringNumOfTables) || !tryParsePositiveInt(stringNumOfTables, &stats->numOfTables))
            erroredExit("Error on line: " + stringNumOfTables);
    }

    {
        // reading working hours
        string workingHours;
        tryReadLine(file, workingHours);
        auto hours = splitString(workingHours, " ");
        if (hours.size() != 2)
            erroredExit("Error on line: " + workingHours);

        int time1 = checkTime(hours[0]);
        int time2 = checkTime(hours[1]);
        if ((time1 == -1) && (time2 == -1))
            erroredExit("Error on line: " + workingHours);

        stats->timeStart = time1;
        stats->timeEnd = time2;
    }

    {
        // reading cost per hour
        string stringCost;
        tryReadLine(file, stringCost);
        if (hasNonNumericChars(stringCost) || !tryParsePositiveInt(stringCost, &stats->costPerHour))
            erroredExit("Error on line: " + stringCost);
    }

    {
        // reading all the events
        string line;
        while (tryReadLine(file, line))
        {
            // getting info that should 100% exist
            // i.e. time, id, name

            // info can be 3 or 4, otherwise exiting
            auto info = splitString(line, " ");
            if (info.size() < 3 || info.size() > 4)
                erroredExit("Error on line: " + line);

            int time = checkTime(info[0]);
            if (time == -1)
                erroredExit("Error on line: " + line);

            int id;
            if (!tryParsePositiveInt(info[1], &id))
                erroredExit("Error on line: " + line);

            string clientName = info[2];
            if (!checkClientName(clientName))
                erroredExit("Error on line: " + line);

            // creating even for this exact line
            Event newEv;
            if (id == EventID::clientCame)
            {
                newEv = ClientCame{time, id, clientName};
            }
            else if (id == EventID::clientChoseTable)
            {
                // this event has the 4th param, checking it
                if (info.size() != 4)
                    erroredExit("Error on line: " + line);

                int tableID;
                if (!tryParsePositiveInt(info[3], &tableID))
                    erroredExit("Error on line: " + line);

                newEv = ClientChoseTable{time, id, clientName, tableID};
            }
            else if (id == EventID::clientWaiting)
            {
                newEv = ClientWaiting{time, id, clientName};
            }
            else if (id == EventID::clientLeft)
            {
                newEv = ClientLeft{time, id, clientName};
            }
            else // id is wrong
            {
                erroredExit("Error on line: " + line);
            }
            events.push_back(newEv);
        }
    }

    return events;
}
void EventVisitor::operator()(const ClientCame &event)
{
    printEvent(event.time, event.id, event.clientName);

    // client is already inside
    if (elementInVector(stats->clientsInside, event.clientName))
        return printEvent(event.time, EventID::error, Err[ErrCode::YouShallNotPass]);

    // client came too early
    if (event.time < stats->timeStart)
        return printEvent(event.time, EventID::error, Err[ErrCode::NotOpenYet]);

    stats->clientsInside.push_back(event.clientName);
}

void EventVisitor::operator()(const ClientChoseTable &event)
{
    printEvent(event.time, event.id, event.clientName, event.tableID);

    // client does not exist
    if (!elementInVector(stats->clientsInside, event.clientName))
        return printEvent(event.time, EventID::error, Err[ErrCode::ClientUnknown]);

    // client tries to take an already taken table
    // if table exists
    if (stats->tablesUsed.find(event.tableID) != stats->tablesUsed.end())
    {
        // if table is taken
        if (stats->tablesUsed[event.tableID].isTaken)
            return printEvent(event.time, EventID::error, Err[ErrCode::PlaceIsBusy]);
        else // user changes their table or just takes this
        {

            // if user had table, looking for it
            int tableID = clientHadTable(stats->tablesUsed, event.clientName);
            if (tableID)
            {
                int timeDiff = event.time - stats->tablesUsed[tableID].lastTimeTaken;

                stats->tablesUsed[tableID].timeTotal += timeDiff;
                stats->tablesUsed[tableID].gain += ceil(timeDiff / 60.0) * stats->costPerHour;
                stats->tablesUsed[tableID].isTaken = false;
            }
            else // just moving user to their table
            {
                stats->tablesUsed[event.tableID].isTaken = true;
                stats->tablesUsed[event.tableID].lastTimeTaken = event.time;
                stats->tablesUsed[event.tableID].lastTakenBy = event.clientName;
            }
        }
    }
    else // table never was taken before
    {
        Table table;
        stats->tablesUsed.insert({event.tableID, table});
        stats->tablesUsed[event.tableID].isTaken = true;
        stats->tablesUsed[event.tableID].lastTimeTaken = event.time;
        stats->tablesUsed[event.tableID].lastTakenBy = event.clientName;
    }
}

void EventVisitor::operator()(const ClientWaiting &event)
{
    printEvent(event.time, event.id, event.clientName);

    // if there is a free table, client takes it
    int tableID = getFreeTable(stats->tablesUsed, stats->numOfTables);
    if (tableID)
    {
        // Table table;

        // stats->tablesUsed.insert({tableID, table});
        // stats->tablesUsed[tableID].isTaken = true;
        // stats->tablesUsed[tableID].lastTimeTaken = event.time;
        // stats->tablesUsed[tableID].lastTakenBy = event.clientName;
        return printEvent(event.time, EventID::error, Err[ErrCode::ICanWaitNoLonger]);
    }

    // if no free tables, client waits;

    // if the queue is full, client leaves
    if (stats->clientsWaiting.size() >= stats->numOfTables) {
        removeClient(stats->clientsInside, event.clientName);
        return printEvent(event.time, EventID::clientKickedOut, event.clientName);
    }

    // if there's place, queue client
    stats->clientsWaiting.push(event.clientName);
}

void EventVisitor::operator()(const ClientLeft &event)
{
    printEvent(event.time, event.id, event.clientName);

    // client does not exist
    if (!elementInVector(stats->clientsInside, event.clientName))
        return printEvent(event.time, EventID::error, Err[ErrCode::ClientUnknown]);

    int tableID = clientHadTable(stats->tablesUsed, event.clientName);
    if (tableID)
    {

        int timeDiff = event.time - stats->tablesUsed[tableID].lastTimeTaken;

        stats->tablesUsed[tableID].timeTotal += timeDiff;
        stats->tablesUsed[tableID].gain += ceil(timeDiff / 60.0) * stats->costPerHour;
        stats->tablesUsed[tableID].isTaken = false;

        // one table got free, need to check the queue
        // TODO: assuming clients can't wait after taking a place?
        if (!stats->clientsWaiting.empty())
        {
            string clientName = stats->clientsWaiting.front();
            stats->clientsWaiting.pop();
            stats->tablesUsed[tableID].isTaken = true;
            stats->tablesUsed[tableID].lastTimeTaken = event.time;
            stats->tablesUsed[tableID].lastTakenBy = clientName;

            printEvent(event.time, EventID::clientLeftQueue, clientName, tableID);
        }
    }

    removeClient(stats->clientsInside, event.clientName);
}
