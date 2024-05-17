#pragma once

#include <variant>
#include <string>

using namespace std;

enum EventID
{
    clientCame = 1,
    clientChoseTable = 2,
    clientWaiting = 3,
    clientLeft = 4,
    clientKickedOut = 11,
    clientLeftQueue = 12,
    error = 13,
};

enum ErrCode {
    YouShallNotPass = 0,
    NotOpenYet,
    PlaceIsBusy,
    ClientUnknown,
    ICanWaitNoLonger,
};

struct ClientCame
{
    int time;
    int id = EventID::clientCame;
    std::string clientName;
};

struct ClientChoseTable
{
    int time;
    int id = EventID::clientChoseTable;
    string clientName;
    int tableID;
};

struct ClientWaiting
{
    int time;
    int id = EventID::clientWaiting;
    string clientName;
};

struct ClientLeft
{
    int time;
    int id = EventID::clientLeft;
    string clientName;
};

struct ClientKickedOut
{
    int time;
    int id = EventID::clientKickedOut;
    string clientName;
};

struct ClientLeftQueue
{
    int time;
    int id = EventID::clientLeftQueue;
    string clientName;
    int tableID;
};

struct Error
{
    int time;
    EventID id = EventID::error;
    string error;
};

using Event = std::variant<ClientCame, ClientChoseTable, ClientWaiting, ClientLeft>;
