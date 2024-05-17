#pragma once
#include <vector>
#include <string>
#include <map>
#include "../Statistics/Statistics.h"

using namespace std;

vector<string> splitString(const string str, const string delimiter);

bool hasNonNumericChars(const std::string &str);

bool tryParsePositiveInt(string s, int *result = nullptr, bool canBeZero = false);

bool tryReadLine(ifstream &file, string &line);

void erroredExit(string errMsg);

int checkTime(const string &str);

bool checkClientName(string &name);

string getTimeFromMins(int minutes);

template <typename T>
bool elementInVector(const vector<T> &v, const T &el)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == el)
        {
            return true;
        }
    }
    return false;
}

void printEvent(int time, int id, string body);
void printEvent(int time, int id, string clientName, int tableID);

int clientHadTable(map<int, Table> &m, string clientName);
int getFreeTable(map<int, Table> &m, int numOfTables);
void removeClient(vector<string> & v, string clientName);