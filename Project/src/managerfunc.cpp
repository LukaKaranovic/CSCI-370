// File to make functions for league managers.

#include "DBObject.h"
#include <ctime>
using namespace std;
using namespace oracle::occi;


int DBObject::getSalaryCap() {
    return this->salaryCap;
}

string DBObject::getCurrSeason() {
    return this->currentSeason;
}

void DBObject::setSalaryCap(int cap) {
    cout << "Salary cap updated from " << this->salaryCap << " to " << cap << endl;
    this->salaryCap = cap;
    return;
}

void DBObject::setCurrSeason(string season) {
    cout << "Current season updated from " << this->currentSeason << " to " << season << endl;
    this->currentSeason = season;
    return;
}

/*
    @brief Generates standings for the current season based on all current data in the games table.

    Only use this method when constructing the standings from a big games table and the standings table is empty. Otherwise will run into errors.
    This function does not give playoffStatus, for that, use calcPlayoffQual().
    If updating standings after a game, use updateStandings(). 
    @return Void.
*/
void DBObject::generateStandings() {
    ResultSet* sanity = stmtMap["st_sanCheck"]->executeQuery();
    if (sanity->next()) {
        cout << "Standings records already exist. Aborting." << endl;
        cout << "If you wish to generate new standings, you can start a new season or dump standings." << endl;
        cout << "If you wish to update standings game by game, use the 'add_game' command." << endl;
        return;
    }
    stmtMap["st_sanCheck"]->closeResultSet(sanity);
    
    // Variables to fill each column
    string teamCode, teamName, conference, division;
    int gamesPlayed = 0, wins = 0, losses = 0, overtimeLosses = 0, points = 0, goalsFor = 0, goalsAgainst = 0, goalDifferential = 0;    
    float pointPct = 0.0f;

    ResultSet* teamData = stmtMap["st_getTeamData"]->executeQuery();
    while (teamData && teamData->next()) {
        // Get team info
        teamCode = teamData->getString(1);
        teamName = teamData->getString(2);
        conference = teamData->getString(3);
        division = teamData->getString(4);
        
        // Get games played
        stmtMap["st_getGamesPlayed"]->setString(1, teamCode);
        ResultSet* rsGP = stmtMap["st_getGamesPlayed"]->executeQuery();
        if (rsGP && rsGP->next()) {
            gamesPlayed = rsGP->getInt(1);
        }
        stmtMap["st_getGamesPlayed"]->closeResultSet(rsGP);

        // Get wins
        stmtMap["st_getWins"]->setString(1, teamCode);
        ResultSet* rsWins = stmtMap["st_getWins"]->executeQuery();
        if (rsWins && rsWins->next()) {
            wins = rsWins->getInt(1);
        }
        stmtMap["st_getWins"]->closeResultSet(rsWins);

        // Get losses
        stmtMap["st_getLosses"]->setString(1, teamCode);
        ResultSet* rsLosses = stmtMap["st_getLosses"]->executeQuery();
        if (rsLosses && rsLosses->next()) {
            losses = rsLosses->getInt(1);
        }
        stmtMap["st_getLosses"]->closeResultSet(rsLosses);

        // Get overtime losses
        stmtMap["st_getOTLosses"]->setString(1, teamCode);
        ResultSet* rsOTL = stmtMap["st_getOTLosses"]->executeQuery();
        if (rsOTL && rsOTL->next()) {
            overtimeLosses = rsOTL->getInt(1);
        }
        stmtMap["st_getOTLosses"]->closeResultSet(rsOTL);

        // Get points
        points = 2 * wins + overtimeLosses;

        // Get pointPct
        if (gamesPlayed > 0) {
            pointPct = static_cast<float>(points) / (2.0f * gamesPlayed);
        } else {
            pointPct = 0.0f;
        }

        // Get goals for
        stmtMap["st_getGFHome"]->setString(1, teamCode);
        ResultSet* rsGFHome = stmtMap["st_getGFHome"]->executeQuery();
        if (rsGFHome && rsGFHome->next()) {
            goalsFor += rsGFHome->getInt(1);
        }
        stmtMap["st_getGFHome"]->closeResultSet(rsGFHome);

        stmtMap["st_getGFAway"]->setString(1, teamCode);
        ResultSet* rsGFAway = stmtMap["st_getGFAway"]->executeQuery();
        if (rsGFAway && rsGFAway->next()) {
            goalsFor += rsGFAway->getInt(1);
        }
        stmtMap["st_getGFAway"]->closeResultSet(rsGFAway);


        // Get goals against
        stmtMap["st_getGAHome"]->setString(1, teamCode);
        ResultSet* rsGAHome = stmtMap["st_getGAHome"]->executeQuery();
        if (rsGAHome && rsGAHome->next()) {
            goalsAgainst += rsGAHome->getInt(1);
        }
        stmtMap["st_getGAHome"]->closeResultSet(rsGAHome);

        stmtMap["st_getGAAway"]->setString(1, teamCode);
        ResultSet* rsGAAway = stmtMap["st_getGAAway"]->executeQuery();
        if (rsGAAway && rsGAAway->next()) {
            goalsAgainst += rsGAAway->getInt(1);
        }
        stmtMap["st_getGAAway"]->closeResultSet(rsGAAway);

        // Get goal differential
        goalDifferential = goalsFor - goalsAgainst;

        // Insert
        stmtMap["st_insertRecord"]->setString(1, this->currentSeason);
        stmtMap["st_insertRecord"]->setString(2, teamCode);
        stmtMap["st_insertRecord"]->setString(3, teamName);
        stmtMap["st_insertRecord"]->setString(4, conference);
        stmtMap["st_insertRecord"]->setString(5, division);
        stmtMap["st_insertRecord"]->setInt(6, gamesPlayed);
        stmtMap["st_insertRecord"]->setInt(7, wins);
        stmtMap["st_insertRecord"]->setInt(8, losses);
        stmtMap["st_insertRecord"]->setInt(9, overtimeLosses);
        stmtMap["st_insertRecord"]->setInt(10, points);
        stmtMap["st_insertRecord"]->setFloat(11, pointPct);
        stmtMap["st_insertRecord"]->setInt(12, goalsFor);
        stmtMap["st_insertRecord"]->setInt(13, goalsAgainst);
        stmtMap["st_insertRecord"]->setInt(14, goalDifferential);
        stmtMap["st_insertRecord"]->executeUpdate();
        // Reset counters to be safe
        gamesPlayed = wins = losses = overtimeLosses = points = goalsFor = goalsAgainst = goalDifferential = 0;
        pointPct = 0.0f;
    }
    stmtMap["st_getTeamData"]->closeResultSet(teamData);
    cout << "Standings generated!" << endl;
}

/*
    @brief Allows user to create a game from the command line interface. Updates games and standings tables automatically.

    Assumes the game was played the day this function is called.
    @return Void.
*/

void DBObject::makeGame() {
    cout << "Welcome to the game creator. Follow the prompts and input the correct game data." << endl;
    // Get gameID
    ResultSet* getID = stmtMap["mg_getID"]->executeQuery();
    int gameID = 1;
    if (getID && getID->next()) {
        gameID = getID->getInt(1) + 1;
    }
    // Get teams
    string homeTeam, awayTeam, winner, isOvertime;
    cout << "Enter the home team of the game (3 letter code):" << endl;
    cin >> homeTeam;
    stmtMap["mg_teamCheck"]->setString(1, homeTeam);
    ResultSet* hteamData = stmtMap["mg_teamCheck"]->executeQuery();
    if (!hteamData->next()) {
        cout << "Home team doesn't exist. Leaving game creator." << endl;
        return;
    }
    homeTeam = hteamData->getString(1);

    cout << "Enter the away team of the game (3 letter code):" << endl;
    cin >> awayTeam;
    stmtMap["mg_teamCheck"]->setString(1, awayTeam);
    ResultSet* ateamData = stmtMap["mg_teamCheck"]->executeQuery();
    if (!ateamData->next()) {
        cout << "Away team doesn't exist. Leaving game creator." << endl;
        return;
    }
    awayTeam = ateamData->getString(1);

    int goalsHome = 0, goalsAway = 0, shotsHome = 0, shotsAway = 0;
    // Get goals home + away and shots home + away 
    do {
        cout << "Enter the amount of goals the home team scored: ";
        cin >> goalsHome;

        // Handle bad inputs
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid integer next time." << endl;
            return;
        }

        // Teams are probably never going to score 30 goals in a game unless the game is rigged. Then there are bigger problems to deal with...
        if (goalsHome < 0 || goalsHome > 30) {
            cout << "GoalsHome is out of range. Leaving game creator." << endl;
            return;
        }

        cout << "Enter the amount of goals the away team scored: ";
        cin >> goalsAway;

        // Handle bad inputs
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid integer next time." << endl;
            return;
        }
        // Teams are probably never going to score 30 goals in a game unless the game is rigged. Then there are bigger problems to deal with...
        if (goalsAway < 0 || goalsAway > 30) {
            cout << "GoalsAway is out of range. Leaving game creator." << endl;
            return;
        }

        // Print error message if there is no winner.
        if (goalsHome == goalsAway) {
            cout << "GoalsHome and goalsAway can't have the same value. There must be a winner. Re-enter values." << endl;
        }
    } while (goalsHome == goalsAway);
    
    cout << "Enter the amount of shots on goal the home team had: ";
    cin >> shotsHome;

    // Handle bad inputs
    if (cin.fail()) {
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid integer next time." << endl;
        return;
    }

    if (shotsHome < 0) {
        cout << "ShotsHome is out of range. Leaving game creator." << endl;
        return;
    }

    cout << "Enter the amount of shots on goal the away team had: ";
    cin >> shotsAway;

    // Handle bad inputs
    if (cin.fail()) {
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid integer next time." << endl;
        return;
    }

    if (shotsAway < 0) {
        cout << "ShotsAway is out of range. Leaving game creator." << endl;
        return;
    }

    if (goalsHome > goalsAway) {
        winner = homeTeam;
    } else {
        winner = awayTeam;
    }

    if (abs(goalsHome - goalsAway) == 1) {
        while (isOvertime != "Y" && isOvertime != "N" && isOvertime != "y" && isOvertime != "n") {
            cout << "Was the game an overtime or shootout game? (Y or N)" << endl;
            cin >> isOvertime;
            isOvertime[0] = toupper(isOvertime[0]);
            // For error message
            if (isOvertime != "Y" && isOvertime != "N" && isOvertime != "y" && isOvertime != "n") {
                cout << "Please enter Y or N." << endl;
            }
        }
    }

    stmtMap["mg_insert"]->setInt(1, gameID);
    stmtMap["mg_insert"]->setString(2, homeTeam);
    stmtMap["mg_insert"]->setString(3, awayTeam);
    stmtMap["mg_insert"]->setString(4, this->currentSeason);
    stmtMap["mg_insert"]->setString(5, winner);
    stmtMap["mg_insert"]->setInt(6, goalsHome);
    stmtMap["mg_insert"]->setInt(7, goalsAway);
    stmtMap["mg_insert"]->setInt(8, shotsHome);
    stmtMap["mg_insert"]->setInt(9, shotsAway);
    stmtMap["mg_insert"]->setString(10, isOvertime);
    stmtMap["mg_insert"]->executeUpdate();

    stmtMap["mg_teamCheck"]->closeResultSet(hteamData);
    stmtMap["mg_teamCheck"]->closeResultSet(ateamData);
    stmtMap["mg_getID"]->closeResultSet(getID);
    cout << "Game with ID " << gameID << " added to game records." << endl;
    updateStandings(gameID);
}

/*
    @brief Updates the standings table given one singular game, passed as a gameID.

    Only use this function if rows in standings already exist, otherwise it will crash.
    @param gameID The ID of the game whose data is to be added to standings.
    @return Void.
*/
void DBObject::updateStandings(int gameID) {
    stmtMap["ust_findGame"]->setInt(1, gameID);
    ResultSet* gameData = stmtMap["ust_findGame"]->executeQuery();
    if (!gameData->next()) {
        cout << "Game with ID " << gameID << " doesn't exist. Returning." << endl;
        return;
    }
    // Variables from gameData
    string homeTeam = gameData->getString(2);
    string awayTeam = gameData->getString(3);
    string season = gameData->getString(5);
    string gameType = gameData->getString(6);
    string winner = gameData->getString(7);
    int goalsHome = gameData->getInt(8);
    int goalsAway = gameData->getInt(9);
    string isOvertime = gameData->getString(10);

    if (gameType == "Regular") {
        // Get each team's standings data.
        stmtMap["ust_homeStandings"]->setString(1, homeTeam);
        ResultSet* homeData = stmtMap["ust_homeStandings"]->executeQuery();
        stmtMap["ust_awayStandings"]->setString(1, awayTeam);
        ResultSet* awayData = stmtMap["ust_awayStandings"]->executeQuery();
        // Variables for updating
        int gamesPlayed = 0, wins = 0, losses = 0, overtimeLosses = 0, points = 0, goalsFor = 0, goalsAgainst = 0, goalDifferential = 0;    
        float pointPct = 0.0f;

        // Update home team data
        if (homeData && homeData->next()) { // Check if not nullptr
            gamesPlayed = homeData->getInt(6);
            wins = homeData->getInt(7);
            losses = homeData->getInt(8);
            overtimeLosses = homeData->getInt(9);
            points = homeData->getInt(10);
            pointPct = homeData->getFloat(11);
            goalsFor = homeData->getInt(12);
            goalsAgainst = homeData->getInt(13);
            goalDifferential = homeData->getInt(14);

            gamesPlayed += 1;
            if (winner == homeData->getString(2)) {
                wins += 1;
                points += 2;
            } else if (isOvertime == "Y") {
                overtimeLosses += 1;
                points += 1;
            } else {
                losses += 1;
            }   
            pointPct = static_cast<float>(points) / (2.0f * gamesPlayed);

            goalsFor += goalsHome;
            goalsAgainst += goalsAway;
            goalDifferential = goalsFor - goalsAgainst;

            stmtMap["ust_updateStandings"]->setInt(1, gamesPlayed);
            stmtMap["ust_updateStandings"]->setInt(2, wins);
            stmtMap["ust_updateStandings"]->setInt(3, losses);
            stmtMap["ust_updateStandings"]->setInt(4, overtimeLosses);
            stmtMap["ust_updateStandings"]->setInt(5, points);
            stmtMap["ust_updateStandings"]->setFloat(6, pointPct);
            stmtMap["ust_updateStandings"]->setInt(7, goalsFor);
            stmtMap["ust_updateStandings"]->setInt(8, goalsAgainst);
            stmtMap["ust_updateStandings"]->setInt(9, goalDifferential);
            stmtMap["ust_updateStandings"]->setString(10, homeData->getString(2));

            stmtMap["ust_updateStandings"]->executeUpdate();
        } else {
            cout << "Home team doesn't exist in standings. Aborting standings update." << endl;
            stmtMap["ust_homeStandings"]->closeResultSet(homeData);
            stmtMap["ust_awayStandings"]->closeResultSet(awayData);
            return;
        }

        // Update away team data
        if (awayData && awayData->next()) { // Check if not nullptr
            gamesPlayed = awayData->getInt(6);
            wins = awayData->getInt(7);
            losses = awayData->getInt(8);
            overtimeLosses = awayData->getInt(9);
            points = awayData->getInt(10);
            pointPct = awayData->getFloat(11);
            goalsFor = awayData->getInt(12);
            goalsAgainst = awayData->getInt(13);
            goalDifferential = awayData->getInt(14);

            gamesPlayed += 1;
            if (winner == awayData->getString(2)) {
                wins += 1;
                points += 2;
            } else if (isOvertime == "Y") {
                overtimeLosses += 1;
                points += 1;
            } else {
                losses += 1;
            }   
            pointPct = static_cast<float>(points) / (2.0f * gamesPlayed);

            goalsFor += goalsAway;
            goalsAgainst += goalsHome;
            goalDifferential = goalsFor - goalsAgainst;

            stmtMap["ust_updateStandings"]->setInt(1, gamesPlayed);
            stmtMap["ust_updateStandings"]->setInt(2, wins);
            stmtMap["ust_updateStandings"]->setInt(3, losses);
            stmtMap["ust_updateStandings"]->setInt(4, overtimeLosses);
            stmtMap["ust_updateStandings"]->setInt(5, points);
            stmtMap["ust_updateStandings"]->setFloat(6, pointPct);
            stmtMap["ust_updateStandings"]->setInt(7, goalsFor);
            stmtMap["ust_updateStandings"]->setInt(8, goalsAgainst);
            stmtMap["ust_updateStandings"]->setInt(9, goalDifferential);
            stmtMap["ust_updateStandings"]->setString(10, awayData->getString(2));

            stmtMap["ust_updateStandings"]->executeUpdate();
        } else {
            cout << "Away team doesn't exist in standings. Aborting standings update." << endl;
            stmtMap["ust_homeStandings"]->closeResultSet(homeData);
            stmtMap["ust_awayStandings"]->closeResultSet(awayData);
            return;
        }
        stmtMap["ust_homeStandings"]->closeResultSet(homeData);
        stmtMap["ust_awayStandings"]->closeResultSet(awayData);
    }
    stmtMap["ust_findGame"]->closeResultSet(gameData);
    cout << "Game with ID " << gameID << " reflected in standings." << endl;
}

/*
    @brief Updates playoff status for each team in standings based on current position.

    NHL wildcard rules apply.
    @return Void.
*/
void DBObject::genPlayoffQual() {
    ResultSet* leagueRank = stmtMap["gpq_presTrophy"]->executeQuery();
    bool westernFlag = false;
    string presTrophy, confWinner, playoffStatus;
    // Get the President's trophy winning team.
    if (leagueRank && leagueRank->next()) {
        presTrophy = leagueRank->getString(1);
        playoffStatus = "P";
        stmtMap["gpq_update"]->setString(1, playoffStatus);
        stmtMap["gpq_update"]->setString(2, presTrophy);
        stmtMap["gpq_update"]->executeUpdate();
        stmtMap["gpq_update2"]->setString(1, playoffStatus);
        stmtMap["gpq_update2"]->setString(2, presTrophy);
        stmtMap["gpq_update2"]->executeUpdate(); 
        if (leagueRank->getString(2) == "Western") { // Winner was in west conf.
            westernFlag = true;
        } else { // Winner was in east conf.
            westernFlag = false;
        }
    }
    // Get the conference winning team of non pres trophy conference.
    while (leagueRank && leagueRank->next()) {
        confWinner = leagueRank->getString(1);
        playoffStatus = "Z";
        if (westernFlag == false) {
            if (leagueRank->getString(2) == "Western") {
                stmtMap["gpq_update"]->setString(1, playoffStatus);
                stmtMap["gpq_update"]->setString(2, confWinner);
                stmtMap["gpq_update"]->executeUpdate();
                stmtMap["gpq_update2"]->setString(1, playoffStatus);
                stmtMap["gpq_update2"]->setString(2, confWinner);
                stmtMap["gpq_update2"]->executeUpdate(); 
                break;
            }
        } else {
            if (leagueRank->getString(2) == "Eastern") {
                stmtMap["gpq_update"]->setString(1, playoffStatus);
                stmtMap["gpq_update"]->setString(2, confWinner);
                stmtMap["gpq_update"]->executeUpdate();
                stmtMap["gpq_update2"]->setString(1, playoffStatus);
                stmtMap["gpq_update2"]->setString(2, confWinner);
                stmtMap["gpq_update2"]->executeUpdate(); 
                break;
            }
        }
    }

    int pacCount = 0, cenCount = 0, atlCount = 0, metCount = 0, westWCCount = 0, eastWCCount = 0;
    ResultSet* divRank = stmtMap["gpq_divisions"]->executeQuery();
    string teamCode, division, conference, currStatus;
    while (divRank && divRank->next()) {
        teamCode = divRank->getString(1);
        division = divRank->getString(2);
        conference = divRank->getString(3);
        currStatus = divRank->getString(4);

        if (division == "Pacific" && pacCount < 3) { 
            pacCount++;
            if (pacCount == 1 && currStatus != "P" && currStatus != "Z") { // Won division
                playoffStatus = "Y";
                stmtMap["gpq_update"]->setString(1, playoffStatus);
                stmtMap["gpq_update"]->setString(2, teamCode);
                stmtMap["gpq_update"]->executeUpdate(); 
                stmtMap["gpq_update2"]->setString(1, playoffStatus);
                stmtMap["gpq_update2"]->setString(2, teamCode);
                stmtMap["gpq_update2"]->executeUpdate(); 
            } else { // Top 3 in division
                if (currStatus != "P" && currStatus != "Z" && currStatus != "Y") {
                    playoffStatus = "X";
                    stmtMap["gpq_update"]->setString(1, playoffStatus);
                    stmtMap["gpq_update"]->setString(2, teamCode);
                    stmtMap["gpq_update"]->executeUpdate(); 
                    stmtMap["gpq_update2"]->setString(1, playoffStatus);
                    stmtMap["gpq_update2"]->setString(2, teamCode);
                    stmtMap["gpq_update2"]->executeUpdate(); 
                } 
            }
        } else if (division == "Central" && cenCount < 3) { 
            cenCount++;
            if (cenCount == 1 && currStatus != "P" && currStatus != "Z") { // Won division
                playoffStatus = "Y";
                stmtMap["gpq_update"]->setString(1, playoffStatus);
                stmtMap["gpq_update"]->setString(2, teamCode);
                stmtMap["gpq_update"]->executeUpdate(); 
                stmtMap["gpq_update2"]->setString(1, playoffStatus);
                stmtMap["gpq_update2"]->setString(2, teamCode);
                stmtMap["gpq_update2"]->executeUpdate(); 
            } else { // Top 3 in division
                if (currStatus != "P" && currStatus != "Z" && currStatus != "Y") {
                    playoffStatus = "X";
                    stmtMap["gpq_update"]->setString(1, playoffStatus);
                    stmtMap["gpq_update"]->setString(2, teamCode);
                    stmtMap["gpq_update"]->executeUpdate(); 
                    stmtMap["gpq_update2"]->setString(1, playoffStatus);
                    stmtMap["gpq_update2"]->setString(2, teamCode);
                    stmtMap["gpq_update2"]->executeUpdate(); 
                } 
            }
        } else if (division == "Atlantic" && atlCount < 3) { 
            atlCount++;
            if (atlCount == 1 && currStatus != "P" && currStatus != "Z") { // Won division
                playoffStatus = "Y";
                stmtMap["gpq_update"]->setString(1, playoffStatus);
                stmtMap["gpq_update"]->setString(2, teamCode);
                stmtMap["gpq_update"]->executeUpdate(); 
                stmtMap["gpq_update2"]->setString(1, playoffStatus);
                stmtMap["gpq_update2"]->setString(2, teamCode);
                stmtMap["gpq_update2"]->executeUpdate(); 
            } else { // Top 3 in division
                if (currStatus != "P" && currStatus != "Z" && currStatus != "Y") {
                    playoffStatus = "X";
                    stmtMap["gpq_update"]->setString(1, playoffStatus);
                    stmtMap["gpq_update"]->setString(2, teamCode);
                    stmtMap["gpq_update"]->executeUpdate(); 
                    stmtMap["gpq_update2"]->setString(1, playoffStatus);
                    stmtMap["gpq_update2"]->setString(2, teamCode);
                    stmtMap["gpq_update2"]->executeUpdate(); 
                } 
            }
        } else if (division == "Metropolitan" && metCount < 3) {
            metCount++;
            if (metCount == 1 && currStatus != "P" && currStatus != "Z") { // Won division
                playoffStatus = "Y";
                stmtMap["gpq_update"]->setString(1, playoffStatus);
                stmtMap["gpq_update"]->setString(2, teamCode);
                stmtMap["gpq_update"]->executeUpdate(); 
                stmtMap["gpq_update2"]->setString(1, playoffStatus);
                stmtMap["gpq_update2"]->setString(2, teamCode);
                stmtMap["gpq_update2"]->executeUpdate(); 
            } else { // Top 3 in division
                if (currStatus != "P" && currStatus != "Z" && currStatus != "Y") {
                    playoffStatus = "X";
                    stmtMap["gpq_update"]->setString(1, playoffStatus);
                    stmtMap["gpq_update"]->setString(2, teamCode);
                    stmtMap["gpq_update"]->executeUpdate(); 
                    stmtMap["gpq_update2"]->setString(1, playoffStatus);
                    stmtMap["gpq_update2"]->setString(2, teamCode);
                    stmtMap["gpq_update2"]->executeUpdate(); 
                } 
            }
        } else if (conference == "Western" && westWCCount < 2) {
            westWCCount++;
            if (currStatus != "P" && currStatus != "Z" && currStatus != "Y") {
                playoffStatus = "X";
                stmtMap["gpq_update"]->setString(1, playoffStatus);
                stmtMap["gpq_update"]->setString(2, teamCode);
                stmtMap["gpq_update"]->executeUpdate(); 
                stmtMap["gpq_update2"]->setString(1, playoffStatus);
                stmtMap["gpq_update2"]->setString(2, teamCode);
                stmtMap["gpq_update2"]->executeUpdate(); 
            }  
        } else if (conference == "Eastern" && eastWCCount < 2) {
            eastWCCount++;
            if (currStatus != "P" && currStatus != "Z" && currStatus != "Y") {
                playoffStatus = "X";
                stmtMap["gpq_update"]->setString(1, playoffStatus);
                stmtMap["gpq_update"]->setString(2, teamCode);
                stmtMap["gpq_update"]->executeUpdate(); 
                stmtMap["gpq_update2"]->setString(1, playoffStatus);
                stmtMap["gpq_update2"]->setString(2, teamCode);
                stmtMap["gpq_update2"]->executeUpdate(); 
            }  
        } else {
            playoffStatus = "E";
            stmtMap["gpq_update"]->setString(1, playoffStatus);
            stmtMap["gpq_update"]->setString(2, teamCode);
            stmtMap["gpq_update"]->executeUpdate(); 
            stmtMap["gpq_update2"]->setString(1, playoffStatus);
            stmtMap["gpq_update2"]->setString(2, teamCode);
            stmtMap["gpq_update2"]->executeUpdate(); 
        }
    }

    stmtMap["gpq_presTrophy"]->closeResultSet(leagueRank);
    stmtMap["gpq_divisions"]->closeResultSet(divRank);
    cout << "Playoff statuses updated!" << endl;
}

/*
    @brief Dump all existing game records to HistoricalGames table.
*/
void DBObject::dumpGameRecords() {
    ResultSet* games = stmtMap["dg_select"]->executeQuery();
    if (games && games->next()) { // Sanity check
        do {
            // Get all row data
            int gameID = games->getInt(1);
            string homeTeam = games->getString(2);
            string awayTeam = games->getString(3);
            Date gameDate = games->getDate(4);
            string season = games->getString(5);
            string gameType = games->getString(6);
            string winner = games->getString(7);
            int goalsHome = games->getInt(8);
            int goalsAway = games->getInt(9);
            int shotsHome = games->getInt(10);
            int shotsAway = games->getInt(11);
            string isOvertime = games->getString(12);

            // Insert all row data
            stmtMap["dg_insert"]->setInt(1, gameID);
            stmtMap["dg_insert"]->setString(2, homeTeam);
            stmtMap["dg_insert"]->setString(3, awayTeam);
            stmtMap["dg_insert"]->setDate(4, gameDate); 
            stmtMap["dg_insert"]->setString(5, season);
            stmtMap["dg_insert"]->setString(6, gameType);
            stmtMap["dg_insert"]->setString(7, winner);
            stmtMap["dg_insert"]->setInt(8, goalsHome);
            stmtMap["dg_insert"]->setInt(9, goalsAway);
            stmtMap["dg_insert"]->setInt(10, shotsHome);
            stmtMap["dg_insert"]->setInt(11, shotsAway);
            stmtMap["dg_insert"]->setString(12, isOvertime);
            stmtMap["dg_insert"]->executeUpdate();
        } while (games->next());
    } else {
        cout << "No games to dump to historical records. Aborting." << endl;
        return;
    }
    int affectedRows = stmtMap["dg_delete"]->executeUpdate();
    cout << affectedRows << " game records dumped to HistoricalGames table." << endl;
    stmtMap["dg_select"]->closeResultSet(games);
}

void DBObject::dumpStandings() {
    ResultSet* standings = stmtMap["ds_select"]->executeQuery();
    
    if (standings && standings->next()) { // Sanity check
        do {
            // Get all row data
            string season = standings->getString(1);
            string teamCode = standings->getString(2);
            string teamName = standings->getString(3);
            string conference = standings->getString(4);
            string division = standings->getString(5);
            int gamesPlayed = standings->getInt(6);
            int wins = standings->getInt(7);
            int losses = standings->getInt(8);
            int overtimeLosses = standings->getInt(9);
            int points = standings->getInt(10); 
            float pointPct = standings->getFloat(11);
            int goalsFor = standings->getInt(12);
            int goalsAgainst = standings->getInt(13);
            int goalDifferential = standings->getInt(14);
            string playoffStatus = standings->getString(15);

            // Insert all row data
            stmtMap["ds_insert"]->setString(1, season);
            stmtMap["ds_insert"]->setString(2, teamCode);
            stmtMap["ds_insert"]->setString(3, teamName);
            stmtMap["ds_insert"]->setString(4, conference);
            stmtMap["ds_insert"]->setString(5, division);
            stmtMap["ds_insert"]->setInt(6, gamesPlayed);
            stmtMap["ds_insert"]->setInt(7, wins);
            stmtMap["ds_insert"]->setInt(8, losses);
            stmtMap["ds_insert"]->setInt(9, overtimeLosses);
            stmtMap["ds_insert"]->setInt(10, points);
            stmtMap["ds_insert"]->setFloat(11, pointPct);
            stmtMap["ds_insert"]->setInt(12, goalsFor);
            stmtMap["ds_insert"]->setInt(13, goalsAgainst);
            stmtMap["ds_insert"]->setInt(14, goalDifferential);
            stmtMap["ds_insert"]->setString(15, playoffStatus);
            stmtMap["ds_insert"]->executeUpdate();
        } while (standings->next());
    } else {
        cout << "No standings to dump to historical records. Aborting." << endl;
        return;
    }
    int affectedRows = stmtMap["ds_delete"]->executeUpdate();
    cout << affectedRows << " standings records dumped to HistoricalStandings table." << endl;
    stmtMap["ds_select"]->closeResultSet(standings);  
}

/*
    @brief Given a playerID and gameID, checks if there is a SkaterGames record that exists and if so, updates Skater stats.
*/

void DBObject::skaterGameUpdate(int playerID, int gameID) {
    stmtMap["skg_search"]->setInt(1, playerID);
    stmtMap["skg_search"]->setInt(2, gameID);
    ResultSet* gameData = stmtMap["skg_search"]->executeQuery();
    stmtMap["skg_getStats"]->setInt(1, playerID);
    ResultSet* stats = stmtMap["skg_getStats"]->executeQuery();

    if (stats->next() && gameData->next()) {
        // get games played
        stmtMap["skg_getGP"]->setInt(1, playerID);
        ResultSet* gp = stmtMap["skg_getGP"] ->executeQuery();
        gp->next();
        int gamesPlayed = gp->getInt(1) + 1;
        
        // Get data and add new data.
        int goals = stats->getInt(2) + gameData->getInt(3);
        int assists = stats->getInt(3) + gameData->getInt(4);
        int points = stats->getInt(4) + gameData->getInt(5);
        int shots = stats->getInt(5) + gameData->getInt(6);     
        int hits = stats->getInt(6) + gameData->getInt(7);
        int plusMinus = stats->getInt(7) + gameData->getInt(8);

        // Do the update
        stmtMap["skg_update"]->setInt(1, goals);
        stmtMap["skg_update"]->setInt(2, assists);
        stmtMap["skg_update"]->setInt(3, points);
        stmtMap["skg_update"]->setInt(4, shots);
        stmtMap["skg_update"]->setInt(5, hits);
        stmtMap["skg_update"]->setInt(6, plusMinus);
        stmtMap["skg_update"]->setInt(7, playerID);
        stmtMap["skg_update"]->executeUpdate();

        stmtMap["skg_GPUpdate"]->setInt(1, gamesPlayed);
        stmtMap["skg_GPUpdate"]->setInt(2, playerID);
        stmtMap["skg_GPUpdate"]->executeUpdate();

        stmtMap["skg_getGP"]->closeResultSet(gp);
    } else {
        cout << "Skater or SkaterGames record doesn't exist. Aborting." << endl;
        stmtMap["skg_getStats"]->closeResultSet(stats);
        stmtMap["skg_search"]->closeResultSet(gameData);
        return;
    }

    stmtMap["skg_getStats"]->closeResultSet(stats);
    stmtMap["skg_search"]->closeResultSet(gameData);
    cout << "Skater record updated with new game data." << endl;
}

/*
    @brief Given a playerID and gameID, checks if there is a GoaltenderGames record that exists and if so, updates Goaltender stats.
*/
void DBObject::goaltenderGameUpdate(int playerID, int gameID) {
    stmtMap["gog_search"]->setInt(1, playerID);
    stmtMap["gog_search"]->setInt(2, gameID);
    ResultSet* gameData = stmtMap["gog_search"]->executeQuery();
    stmtMap["gog_getStats"]->setInt(1, playerID);
    ResultSet* stats = stmtMap["gog_getStats"]->executeQuery();

    if (stats->next() && gameData->next()) {
        // get games played
        stmtMap["skg_getGP"]->setInt(1, playerID);
        ResultSet* gp = stmtMap["skg_getGP"] ->executeQuery();
        gp->next();
        int gamesPlayed = gp->getInt(1) + 1;
        
        // Get data and add new data.
        int wins = stats->getInt(2) + gameData->getInt(3);
        int losses = stats->getInt(3) + gameData->getInt(4);
        int OTLosses = stats->getInt(4) + gameData->getInt(5);
        int saves = stats->getInt(6) + gameData->getInt(7);
        int goalsAgainst = stats->getInt(7) + gameData->getInt(8);

        // Get new averages
        float savePct = static_cast<float>(stats->getFloat(5) * (gamesPlayed - 1) + gameData->getFloat(6)) / gamesPlayed;     
        float GAA = static_cast<float>(stats->getFloat(8) * (gamesPlayed - 1) + gameData->getFloat(9)) / gamesPlayed;
        // Do the update
        stmtMap["gog_update"]->setInt(1, wins);
        stmtMap["gog_update"]->setInt(2, losses);
        stmtMap["gog_update"]->setInt(3, OTLosses);
        stmtMap["gog_update"]->setFloat(4, savePct);
        stmtMap["gog_update"]->setInt(5, saves);
        stmtMap["gog_update"]->setInt(6, goalsAgainst);
        stmtMap["gog_update"]->setFloat(7, GAA);
        stmtMap["gog_update"]->setInt(8, playerID);
        stmtMap["gog_update"]->executeUpdate();

        stmtMap["skg_GPUpdate"]->setInt(1, gamesPlayed);
        stmtMap["skg_GPUpdate"]->setInt(2, playerID);
        stmtMap["skg_GPUpdate"]->executeUpdate();

        stmtMap["skg_getGP"]->closeResultSet(gp);
    } else {
        cout << "Goaltender or GoaltenderGames record doesn't exist. Aborting." << endl;
        stmtMap["gog_getStats"]->closeResultSet(stats);
        stmtMap["gog_search"]->closeResultSet(gameData);
        return;
    }

    stmtMap["gog_getStats"]->closeResultSet(stats);
    stmtMap["gog_search"]->closeResultSet(gameData);
    cout << "Goaltender record updated with new game data." << endl;
}

void DBObject::addPlayerToTeam() {
    cout << "Welcome to the player creator. Follow instructions and input all required fields." << endl;
    string team, name, nationality, birthdate, position;
    int salary, jerseyNum;

    cout << "What is the player's name?" << endl;
    getline(cin, name);
    if (name.length() < 1 || name.length() > 50) {
        cout << "Player name is of invalid length. Leaving player creator." << endl;
        return;
    }
    cout << "What team will they play for (enter 3 letter code)?" << endl;
    cin >> team;
    stmtMap["add_teamData"]->setString(1, team);
    ResultSet* teamData = stmtMap["add_teamData"]->executeQuery();
    if (!teamData->next()) {
        cout << "Team doesn't exist in league. Leaving player creator." << endl;
        stmtMap["add_teamData"]->closeResultSet(teamData);
        return;
    }
    team = teamData->getString(1);
    int skaterCount = teamData->getInt(2);
    int goaltenderCount = teamData->getInt(3);
    int capHit = teamData->getInt(4);
    stmtMap["add_teamData"]->closeResultSet(teamData);

    cout << "What is the player's yearly salary?" << endl;
    try {
        cin >> salary;

        if (salary < 0 || salary > 20000000) {
            cout << "Salary is out of range. Leaving player creator." << endl;
            return;
        }
        if (cin.fail()) {
            throw invalid_argument("Invalid input for an integer.");
        }
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        cin.clear();
        return;
    }
    capHit += salary;
    if (capHit > salaryCap) {
        cout << "Team would be over salary cap. Can't add " << name << " to " << team << endl;
        return;
    }

    cout << "What is the player's nationality?" << endl;
    cin >> nationality;
    
    cout << "What is the player's birthdate (in yyyy-mm-dd)?" << endl;
    cin >> birthdate;  

    cout << "What is the player's position?" << endl;
    cin >> position;
    if (position != "C" && position != "LW" && position != "RW" && position != "D" && position != "G") {
        cout << "Invalid position. Leaving player creator." << endl;
        return;
    }

    if (position == "G") {
        goaltenderCount++;
        if (goaltenderCount > 3) {
            cout << "Team would exceed goaltender limit. Can't add player." << endl;
            return;
        }
    } else {
        skaterCount++;
        if (skaterCount > 21) {
            cout << "Team would exceed skater limit. Can't add player." << endl;
            return;
        } 
    }

    int playerID = 1;

    cout << "What is the player's jersey number (1-99)?" << endl;
    cin >> jerseyNum;
    if (jerseyNum < 1 || jerseyNum > 99) {
        cout << "Jersey number is out of range. Leaving player creator." << endl;
        return;
    }


    ResultSet* getID = stmtMap["add_getID"]->executeQuery();
    if (getID && getID->next() ){
        playerID = getID->getInt(1) + 1;
    }
    stmtMap["add_getID"]->closeResultSet(getID);

    stmtMap["add_player"]->setInt(1, playerID);
    stmtMap["add_player"]->setString(2, name);
    stmtMap["add_player"]->setString(3, team);
    stmtMap["add_player"]->setInt(4, salary);
    stmtMap["add_player"]->setString(5, nationality);
    stmtMap["add_player"]->setString(6, birthdate);
    stmtMap["add_player"]->setString(7, position);
    stmtMap["add_player"]->setInt(8, jerseyNum);
    stmtMap["add_player"]->executeUpdate();
    cout << "Player added to Players table." << endl;

    if (position == "G") {
        stmtMap["add_goalie"]->setInt(1, playerID);
        stmtMap["add_goalie"]->executeUpdate();
        cout << "Player added to Goaltenders table." << endl;
    } else {
        stmtMap["add_skater"]->setInt(1, playerID);
        stmtMap["add_skater"]->executeUpdate();
        cout << "Player added to Skaters table." << endl;
    }
    // Update team data.
    stmtMap["add_updateTeam"]->setInt(1, capHit);
    stmtMap["add_updateTeam"]->setInt(2, skaterCount);
    stmtMap["add_updateTeam"]->setInt(3, goaltenderCount);
    stmtMap["add_updateTeam"]->setString(4, team);
    stmtMap["add_updateTeam"]->executeUpdate();
    cout << "Team data updated." << endl;
}