#ifndef DBOBJECT_H
#define DBOBJECT_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <stdexcept>
#include <map>
#include <string>
#include <limits>

using namespace std;
using namespace oracle::occi;

class DBObject {

private:
    Environment* env;
    Connection* conn;
    map<string, Statement*> stmtMap;
    int salaryCap;
    string currentSeason;
public:
    // Constructor
    DBObject();
    // Destructor
    ~DBObject();

    // LEAGUE MANAGER FUNCTIONS

    int getSalaryCap();

    string getCurrSeason();

    void setSalaryCap(int cap);

    void setCurrSeason(string season);

    void generateStandings();

    void makeGame();

    void updateStandings(int gameID);

    void genPlayoffQual();

    void dumpGameRecords();

    void dumpStandings();

    void skaterGameUpdate(int playerID, int gameID);

    void goaltenderGameUpdate(int playerID, int gameID);

    void addPlayerToTeam();

    // VIEWER FUNCTIONS

    void skaterStats();

    void goaltenderStats();

    void viewStandings();

};


#endif