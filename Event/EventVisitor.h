
#pragma once
#include "../Statistics/Statistics.h"
#include "Event.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

class EventVisitor
{
public:
    EventVisitor(Statistics *stats);
    ~EventVisitor();
    vector<Event> parseEvents(ifstream &file);
    void removeClientAndCalcTable(int time, int tableID);

    void operator()(const ClientCame &event);
    void operator()(const ClientChoseTable &event);
    void operator()(const ClientWaiting &event);
    void operator()(const ClientLeft &event);

private:
    Statistics *stats;
    map<ErrCode, string> Err = {
        {YouShallNotPass, "YouShallNotPass"},
        {NotOpenYet, "NotOpenYet"},
        {PlaceIsBusy, "PlaceIsBusy"},
        {ClientUnknown, "ClientUnknown"},
        {ICanWaitNoLonger, "ICanWaitNoLonger!"}};
};