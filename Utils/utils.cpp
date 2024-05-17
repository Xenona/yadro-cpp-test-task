#include "utils.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cctype>
#include <string>

void erroredExit(string errMsg)
{
    std::cerr << errMsg << std::endl;
    exit(1);
}

vector<string> splitString(string str, string delimiter)
{
    vector<string> res;

    size_t pos = 0;
    while ((pos = str.find(delimiter)) != string::npos)
    {
        string token = str.substr(0, pos);
        res.push_back(token);
        str.erase(0, pos + delimiter.length());
    }

    if (!str.empty())
    {
        if (str.back() == delimiter.back())
        {
            str.pop_back();
        }
        res.push_back(str);
    }

    return res;
}

bool hasNonNumericChars(const string &str)
{
    for (char c : str)
    {
        if (!isdigit(c))
        {
            return true;
        }
    }
    return false;
}

bool tryParsePositiveInt(string s, int *result, bool canBeZero)
{

    int res;
    try
    {
        res = stoi(s);
    }
    catch (invalid_argument const &e)
    {
        return false;
    }
    catch (out_of_range const &e)
    {
        return false;
    }

    if (res < 0)
        return false;

    if (res == 0 && !canBeZero)
        return false;

    if (result != nullptr)
        *result = res;

    return true;
}

bool tryReadLine(ifstream &file, string &line)
{
    if (file.peek() == EOF)
    {
        return false;
    }

    if (!std::getline(file, line))
        erroredExit("Failed to read the line.");

    return true;
}

int checkTime(const string &str)
{
    auto times = splitString(str, ":");
    if (times.size() != 2)
        return -1;
    if (!(times[0].length() == 2 && times[1].length() == 2))
        return -1;

    int time1, time2;
    if (!(tryParsePositiveInt(times[0], &time1, true) && tryParsePositiveInt(times[1], &time2, true)))
        return -1;
    if (time1 > 24 || time2 > 60)
        return -1;

    return time1 * 60 + time2;
}

bool checkClientName(string &name)
{
    for (char c : name)
    {
        if (!(isalpha(c) && islower(c) || isdigit(c) || c == '_'))
            return false;
    }
    return true;
}

string getTimeFromMins(int minutes)
{
    int h = minutes / 60;
    int m = minutes % 60;

    string res;
    if (h < 10)
        res += "0";
    res += to_string(h) + ":";

    if (m < 10)
        res += "0";
    res += to_string(m);

    return res;
};

void printEvent(int time, int id, string body)
{
    cout
        << getTimeFromMins(time)
        << " "
        << id
        << " "
        << body
        << endl;
}

void printEvent(int time, int id, string clientName, int tableID)
{
    cout
        << getTimeFromMins(time)
        << " "
        << id
        << " "
        << clientName
        << " "
        << tableID
        << endl;
}

int clientHadTable(map<int, Table> &m, string clientName)
{
    for (const auto &pair : m)
    {
        if (pair.second.lastTakenBy == clientName)
        {
            return pair.first;
        }
    }
    return 0;
}

int getFreeTable(map<int, Table> &m, int numOfTables)
{

    for (int i = 1; i <= numOfTables; i++)
    {
        if (!m[i].isTaken)
            return i;
    }

    return 0;
}

void removeClient(vector<string> & v, string clientName) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == clientName) {
            v.erase(v.begin() + i);
            return;
        }
    }
}