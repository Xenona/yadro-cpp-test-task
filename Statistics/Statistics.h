#pragma once
#include <vector>
#include <string>
#include <queue>
#include <map>

using namespace std;

struct Table {
    int timeTotal = 0;
    int gain = 0;
    bool isTaken = false;
    int lastTimeTaken;
    string lastTakenBy;
};

class Statistics {
    public:
    Statistics();

    int numOfTables;
    int timeStart;
    int timeEnd;
    int costPerHour;

    vector<string> clientsInside;
    queue<string> clientsWaiting;
    map<int, Table> tablesUsed;

};