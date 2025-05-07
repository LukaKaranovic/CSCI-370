// File to make functions for viewers.

#include "DBObject.h"
#include <ctime>
using namespace std;
using namespace oracle::occi;

void DBObject::skaterStats() {
    int command = -1;

    while (command != 0) {
        cout << "Welcome to the skater stats viewer." << endl;
        cout << "0 - Leave the stats viewer." << endl;
        cout << "1 - View the current points leaders." << endl;
        cout << "2 - View the current goals leaders." << endl;
        cout << "3 - View the current assists leaders." << endl;
        cout << "4 - View the skaters with the most games played." << endl;
        cin >> command;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid integer next time." << endl;
            command = -1;
        }
        int counter = 0;
        switch (command) {
            case 0: {
                cout << "Thanks for using the skater stats viewer." << endl;
                return;
            }
            case 1: {
                ResultSet* pointsLB = stmtMap["sks_points"]->executeQuery();
                cout << "Rank    Player Name         Points" << endl;
                cout << "----    -----------         ------" << endl;
                counter = 0;
                while (pointsLB && pointsLB->next() && counter < 10) {
                    counter++;
                    string name = pointsLB->getString(1);
                    int points = pointsLB->getInt(2);
                    cout << left << setw(8) << counter << setw(20) << name << setw(15) << points << endl;
                }
                stmtMap["sks_points"]->closeResultSet(pointsLB);
                cout << "----------------------------------" << endl;
                break;
            }
            case 2: {
                ResultSet* goalsLB = stmtMap["sks_goals"]->executeQuery();
                cout << "Rank    Player Name         Goals" << endl;
                cout << "----    -----------         -----" << endl;
                counter = 0;
                while (goalsLB && goalsLB->next() && counter < 10) {
                    counter++;
                    string name = goalsLB->getString(1);
                    int goals = goalsLB->getInt(2);
                    cout << left << setw(8) << counter << setw(20) << name << setw(15) << goals << endl;
                }
                stmtMap["sks_goals"]->closeResultSet(goalsLB);
                cout << "---------------------------------" << endl;
                break;
            }
            case 3: {
                ResultSet* assistsLB = stmtMap["sks_assists"]->executeQuery();
                cout << "Rank    Player Name         Assists" << endl;
                cout << "----    -----------         -------" << endl;
                counter = 0;
                while (assistsLB && assistsLB->next() && counter < 10) {
                    counter++;
                    string name = assistsLB->getString(1);
                    int assists = assistsLB->getInt(2);
                    cout << left << setw(8) << counter << setw(20) << name << setw(15) << assists << endl;
                }
                stmtMap["sks_assists"]->closeResultSet(assistsLB);
                cout << "------------------------------------" << endl;
                break;
            }
            case 4: {
                ResultSet* gpLB = stmtMap["sks_gp"]->executeQuery();
                cout << "Rank    Player Name         Games Played" << endl;
                cout << "----    -----------         ------------" << endl;
                counter = 0;
                while (gpLB && gpLB->next() && counter < 10) {
                    counter++;
                    string name = gpLB->getString(1);
                    int gp = gpLB->getInt(2);
                    cout << left << setw(8) << counter << setw(20) << name << setw(15) << gp << endl;
                }
                stmtMap["sks_gp"]->closeResultSet(gpLB);
                cout << "----------------------------------------" << endl;
                break;
            }
            default: {
                cout << "Invalid command. Try again." << endl;
                command = -1;
                break;
            }
        }
    }
    return;
}

void DBObject::goaltenderStats() {
    int command = -1;

    while (command != 0) {
        cout << "Welcome to the goaltender stats viewer." << endl;
        cout << "0 - Leave the stats viewer." << endl;
        cout << "1 - View the current wins leaders." << endl;
        cout << "2 - View the current save percentage leaders." << endl;
        cout << "3 - View the current save leaders." << endl;
        cout << "4 - View the current goals against average leaders." << endl;
        cout << "5 - View the current goals against leaders." << endl;
        cout << "6 - View the players with the most games played." << endl;
        cin >> command;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid integer next time." << endl;
            command = -1;
        }
        int counter = 0;
        switch (command) {
            case 0: {
                cout << "Thanks for using the goaltender stats viewer." << endl;
                return;
            }
            case 1: {
                ResultSet* winsLB = stmtMap["gos_wins"]->executeQuery();
                cout << "Rank    Player Name         Wins" << endl;
                cout << "----    -----------         ----" << endl;
                counter = 0;
                while (winsLB && winsLB->next() && counter < 10) {
                    counter++;
                    string name = winsLB->getString(1);
                    int wins = winsLB->getInt(2);
                    cout << left << setw(8) << counter << setw(20) << name << setw(15) << wins << endl;
                }
                stmtMap["gos_wins"]->closeResultSet(winsLB);
                cout << "----------------------------------" << endl;
                break;
            }
            case 2: {
                ResultSet* savePctLB = stmtMap["gos_savePct"]->executeQuery();
                cout << "Rank    Player Name         Save Pct" << endl;
                cout << "----    -----------         --------" << endl;
                counter = 0;
                while (savePctLB && savePctLB->next() && counter < 10) {
                    counter++;
                    string name = savePctLB->getString(1);
                    float savePct = savePctLB->getFloat(2);
                    cout << left << setw(8) << counter << setw(20) << name << setw(15) << savePct << endl;
                }
                stmtMap["gos_savePct"]->closeResultSet(savePctLB);
                cout << "------------------------------------" << endl;
                break;
            }
            case 3: {
                ResultSet* savesLB = stmtMap["gos_saves"]->executeQuery();
                cout << "Rank    Player Name         Saves" << endl;
                cout << "----    -----------         -----" << endl;
                counter = 0;
                while (savesLB && savesLB->next() && counter < 10) {
                    counter++;
                    string name = savesLB->getString(1);
                    int saves = savesLB->getInt(2);
                    cout << left << setw(8) << counter << setw(20) << name << setw(15) << saves << endl;
                }
                stmtMap["gos_saves"]->closeResultSet(savesLB);
                cout << "----------------------------------" << endl;
                break;
            }
            case 4: {
                ResultSet* GAALB = stmtMap["gos_GAA"]->executeQuery();
                cout << "Rank    Player Name         Goals Against Average" << endl;
                cout << "----    -----------         ---------------------" << endl;
                counter = 0;
                while (GAALB && GAALB->next() && counter < 10) {
                    counter++;
                    string name = GAALB->getString(1);
                    float GAA = GAALB->getFloat(2);
                    cout << left << setw(8) << counter << setw(20) << name << setw(15) << GAA << endl;
                }
                stmtMap["gos_GAA"]->closeResultSet(GAALB);
                cout << "------------------------------------" << endl;
                break;
            }
            case 5: {
                ResultSet* GALB = stmtMap["gos_GA"]->executeQuery();
                cout << "Rank    Player Name         Goals Against" << endl;
                cout << "----    -----------         -------------" << endl;
                counter = 0;
                while (GALB && GALB->next() && counter < 10) {
                    counter++;
                    string name = GALB->getString(1);
                    int GA = GALB->getInt(2);
                    cout << left << setw(8) << counter << setw(20) << name << setw(15) << GA << endl;
                }
                stmtMap["gos_GA"]->closeResultSet(GALB);
                cout << "----------------------------------" << endl;
                break;
            }
            case 6: {
                ResultSet* gpLB = stmtMap["gos_gp"]->executeQuery();
                cout << "Rank    Player Name         Games Played" << endl;
                cout << "----    -----------         ------------" << endl;
                counter = 0;
                while (gpLB && gpLB->next() && counter < 10) {
                    counter++;
                    string name = gpLB ->getString(1);
                    int gp = gpLB ->getInt(2);
                    cout << left << setw(8) << counter << setw(20) << name << setw(15) << gp << endl;
                }
                stmtMap["gos_gp"]->closeResultSet(gpLB);
                cout << "----------------------------------------" << endl;
                break;
            }
            default: {
                cout << "Invalid command. Try again." << endl;
                command = -1;
                break;
            }
        }
    }
    return;
}

void DBObject::viewStandings() {
    int command = -1;
    int counter = 0;

    while (command != 0) {
        cout << "Welcome to the standings viewer." << endl;
        cout << "0 - Leave the standings viewer." << endl;
        cout << "1 - View the league standings." << endl;
        cout << "2 - View the division standings." << endl;
        cout << "3 - View the teams with best goal diferential." << endl;
        cin >> command;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid integer next time." << endl;
            command = -1;
        }
        switch (command) {
            case 0: {
                cout << "Thanks for using the standings viewer." << endl;
                return;
            }
            case 1: {
                ResultSet* standings = stmtMap["vst_league"]->executeQuery();
                cout << "Rank  Team   GP   W   L   OT  PTS   PointPct   Playoff  Status" << endl;
                cout << "----  ----   --   -   -   --  ---   --------   ---------------" << endl;
                counter = 0;
                while (standings && standings->next()) {
                    counter++;
                    string teamCode = standings->getString(1);
                    int gamesPlayed = standings->getInt(2);
                    int wins = standings->getInt(3);
                    int losses = standings->getInt(4);
                    int otLosses = standings->getInt(5);
                    int points = standings->getInt(6);
                    float pointPct = standings->getFloat(7);
                    string playoffStatus = standings->getString(8);
            
                    cout << left << setw(6) << counter
                         << setw(7) << teamCode
                         << setw(5) << gamesPlayed
                         << setw(4) << wins
                         << setw(4) << losses
                         << setw(4) << otLosses
                         << setw(6) << points
                         << setw(11) << fixed << setprecision(3) << pointPct
                         << setw(12) << playoffStatus
                         << endl;
                }
                stmtMap["vst_league"]->closeResultSet(standings);
                cout << "------------------------------------------------------------" << endl;
                break;
            }
            case 2: {
                ResultSet* divstandings = stmtMap["vst_division"]->executeQuery();
                cout << "Division       Team   GP   W   L   OT  PTS   PointPct   Playoff  Status" << endl;
                cout << "--------       ----   --   -   -   --  ---   --------   ---------------" << endl;
                while (divstandings && divstandings->next()) {
                    counter++;
                    string division = divstandings->getString(1);
                    string teamCode = divstandings->getString(2);
                    int gamesPlayed = divstandings->getInt(3);
                    int wins = divstandings->getInt(4);
                    int losses = divstandings->getInt(5);
                    int otLosses = divstandings->getInt(6);
                    int points = divstandings->getInt(8);
                    float pointPct = divstandings->getFloat(8);
                    string playoffStatus = divstandings->getString(9);
            
                    cout << left << setw(15) << division
                         << setw(7) << teamCode
                         << setw(5) << gamesPlayed
                         << setw(4) << wins
                         << setw(4) << losses
                         << setw(4) << otLosses
                         << setw(6) << points
                         << setw(11) << fixed << setprecision(3) << pointPct
                         << setw(12) << playoffStatus
                         << endl;
                }
                stmtMap["vst_division"]->closeResultSet(divstandings);
                cout << "-----------------------------------------------------------------------" << endl;
                break;
            }
            case 3: {
                ResultSet* goalDiff = stmtMap["vst_goaldiff"]->executeQuery();
                cout << "Rank  Team   GF    GA    Diff" << endl;
                cout << "----  ----   ---   ---   -----" << endl;
                counter = 0;
                while (goalDiff && goalDiff->next()) {
                    counter++;
                    string teamCode = goalDiff->getString(1);
                    int goalsFor = goalDiff->getInt(2);
                    int goalsAgainst = goalDiff->getInt(3);
                    int goalDifferential = goalDiff->getInt(4);

                    cout << left << setw(6) << counter
                        << setw(7) << teamCode
                        << setw(6) << goalsFor
                        << setw(6) << goalsAgainst
                        << setw(6) << goalDifferential
                        << endl;
                }

                stmtMap["vst_goaldiff"]->closeResultSet(goalDiff);
                cout << "-------------------------------" << endl;
            }
            default: {
                cout << "Invalid command. Try again." << endl;
                command = -1;
                break;
            }
        }
    }
}