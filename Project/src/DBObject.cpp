// File for constructor and destructor of DBObject class, used to initialize query map.

#include "DBObject.h"

using namespace std;
using namespace oracle::occi;

string readPassword() {
    struct termios settings;
    tcgetattr( STDIN_FILENO, &settings );
    settings.c_lflag =  (settings.c_lflag & ~(ECHO));
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );

    string password = "";
    getline(cin, password);

    settings.c_lflag = (settings.c_lflag |   ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );
    return password;
}

DBObject::DBObject(): env(), conn(), stmtMap(), salaryCap(88000000), currentSeason("2024-25") {
    string username;
    string password;

    const string connectString = "database.csci.viu.ca";

    cout << "Your username: ";
    getline(cin, username);

    cout << "Your password: ";
    password = readPassword();
    cout << endl;

    env = Environment::createEnvironment();
    conn = env->createConnection(username, password, connectString);

    // LEAGUE MANAGER QUERIES

    // generateStandings Function Queries - all games
    string queryStr = "";

    queryStr = "SELECT * from Standings";
    Statement* st_sanCheck = conn->createStatement(queryStr);
    stmtMap["st_sanCheck"] = st_sanCheck;

    queryStr = "SELECT teamCode, location || ' ' || name AS teamName, conference, division"
    " from Teams";
    Statement* st_getTeamData = conn->createStatement(queryStr);
    stmtMap["st_getTeamData"] = st_getTeamData;

    queryStr = "SELECT COUNT(*)"
    " FROM Games g left join Teams t"
        " ON (t.teamCode = g.homeTeam OR t.teamCode = g.awayTeam)"
        " AND g.gameType = 'Regular'"
    " WHERE (g.homeTeam = t.teamCode OR g.awayTeam = t.teamCode)"
        " AND t.teamCode = :1";
    Statement* st_getGamesPlayed = conn->createStatement(queryStr);
    stmtMap["st_getGamesPlayed"] = st_getGamesPlayed;

    queryStr = "SELECT COUNT(*)"
    " FROM Games g left join Teams t"
        " ON (t.teamCode = g.homeTeam OR t.teamCode = g.awayTeam)"
        " AND g.gameType = 'Regular'"
    " WHERE g.winner = t.teamCode"
        " AND t.teamCode = :1";
    Statement* st_getWins = conn->createStatement(queryStr);
    stmtMap["st_getWins"] = st_getWins;

    queryStr = "SELECT COUNT(*)" 
    " FROM Games g left join Teams t"
        " ON (t.teamCode = g.homeTeam OR t.teamCode = g.awayTeam)"
        " AND g.gameType = 'Regular'"
    " WHERE (g.homeTeam = t.teamCode OR g.awayTeam = t.teamCode)"
        " AND g.winner != t.teamCode"
        " AND g.winner IS NOT NULL"
        " AND g.isOvertime = 'N'"
        " AND t.teamCode = :1";
    Statement* st_getLosses = conn->createStatement(queryStr);
    stmtMap["st_getLosses"] = st_getLosses;

    queryStr = "SELECT COUNT(*)"
    " FROM Games g left join Teams t"
        " ON (t.teamCode = g.homeTeam OR t.teamCode = g.awayTeam)"
        " AND g.gameType = 'Regular'"
    " WHERE (g.homeTeam = t.teamCode OR g.awayTeam = t.teamCode)"
        " AND g.winner != t.teamCode"
        " AND g.winner IS NOT NULL"
        " AND g.isOvertime = 'Y'"
        " AND t.teamCode = :1";
    Statement* st_getOTLosses = conn->createStatement(queryStr);
    stmtMap["st_getOTLosses"] = st_getOTLosses;

    queryStr = "SELECT SUM(goalsHome)"
    " FROM Games g left join Teams t"
        " ON (t.teamCode = g.homeTeam OR t.teamCode = g.awayTeam)"
        " AND g.gameType = 'Regular'"
    " WHERE (g.homeTeam = t.teamCode)"
        " AND t.teamCode = :1";
    Statement* st_getGFHome = conn->createStatement(queryStr);
    stmtMap["st_getGFHome"] = st_getGFHome;

    queryStr = "SELECT SUM(goalsAway)"
    " FROM Games g left join Teams t"
        " ON (t.teamCode = g.homeTeam OR t.teamCode = g.awayTeam)"
        " AND g.gameType = 'Regular'"
    " WHERE (g.awayTeam = t.teamCode)"
        " AND t.teamCode = :1";
    Statement* st_getGFAway = conn->createStatement(queryStr);
    stmtMap["st_getGFAway"] = st_getGFAway;

    queryStr = "SELECT SUM(goalsAway)"
    " FROM Games g left join Teams t"
        " ON (t.teamCode = g.homeTeam OR t.teamCode = g.awayTeam)"
        " AND g.gameType = 'Regular'"
    " WHERE (g.homeTeam = t.teamCode)"
        " AND t.teamCode = :1";
    Statement* st_getGAHome = conn->createStatement(queryStr);
    stmtMap["st_getGAHome"] = st_getGAHome;


    queryStr = "SELECT SUM(goalsHome)"
    " FROM Games g left join Teams t"
        " ON (t.teamCode = g.homeTeam OR t.teamCode = g.awayTeam)"
        " AND g.gameType = 'Regular'"
    " WHERE (g.awayTeam = t.teamCode)"
        " AND t.teamCode = :1";
    Statement* st_getGAAway = conn->createStatement(queryStr);
    stmtMap["st_getGAAway"] = st_getGAAway;

    queryStr = "INSERT INTO Standings ("
        " season, teamCode, teamName, conference, division, "
        " gamesPlayed, wins, losses, overtimeLosses,"
        " points, pointPct, goalsFor, goalsAgainst,"
        " goalDifferential"
        ") VALUES (:1, :2, :3, :4, :5,"
            " :6, :7, :8, :9, :10,"
            " :11, :12, :13, :14)";
    Statement* st_insertRecord = conn->createStatement(queryStr);
    stmtMap["st_insertRecord"] = st_insertRecord;

    // makeGame Function Queries

    queryStr = "INSERT INTO Games ("
        " gameID, homeTeam, awayTeam, gameDate, season,"
        " gameType, winner, goalsHome, goalsAway,"
        " shotsHome, shotsAway, isOvertime"
        ") VALUES ("
        " :1, :2, :3, SYSDATE, :4,"
        " 'Regular', :5, :6, :7, :8, :9, :10)";
    Statement* mg_insert = conn->createStatement(queryStr);
    stmtMap["mg_insert"] = mg_insert;

    queryStr = "SELECT MAX(COALESCE(G.gameID, H.gameID))"
    " FROM Games G FULL OUTER JOIN HistoricalGames H ON G.gameID = H.gameID";
    Statement* mg_getID = conn->createStatement(queryStr);
    stmtMap["mg_getID"] = mg_getID;

    queryStr = "select teamCode from Teams where teamCode = :1";
    Statement* mg_teamCheck = conn->createStatement(queryStr);
    stmtMap["mg_teamCheck"] = mg_teamCheck;

    // updateStandings Function Queries - one game

    queryStr = "select * from Games where gameID = :1";
    Statement* ust_findGame = conn->createStatement(queryStr);
    stmtMap["ust_findGame"] = ust_findGame;

    queryStr = "select * from Standings where teamCode = :1";
    Statement* ust_homeStandings = conn->createStatement(queryStr);
    stmtMap["ust_homeStandings"] = ust_homeStandings;

    queryStr = "select * from Standings where teamCode = :1";
    Statement* ust_awayStandings = conn->createStatement(queryStr);
    stmtMap["ust_awayStandings"] = ust_awayStandings;

    queryStr = "UPDATE Standings"
        " SET"
            " gamesPlayed = :1,"
            " wins = :2,"
            " losses = :3,"
            " overtimeLosses = :4,"
            " points = :5,"
            " pointPct = :6,"
            " goalsFor = :7,"
            " goalsAgainst = :8,"
            " goalDifferential = :9"
        " WHERE teamCode = :10";
    Statement* ust_updateStandings = conn->createStatement(queryStr);
    stmtMap["ust_updateStandings"] = ust_updateStandings;

    // genPlayoffQual Function Queries
    queryStr = "SELECT teamCode, conference"
    " FROM Standings"
    " ORDER BY points DESC";   
    Statement* gpq_presTrophy = conn->createStatement(queryStr);
    stmtMap["gpq_presTrophy"] = gpq_presTrophy;

    queryStr = "SELECT teamCode, division, conference, playoffStatus"
        " FROM Standings"
        " ORDER BY conference, points DESC";
    Statement* gpq_divisions = conn->createStatement(queryStr);
    stmtMap["gpq_divisions"] =  gpq_divisions;

    queryStr = "UPDATE Standings"
        " SET playoffStatus = :1"
        " where teamCode = :2";
    Statement* gpq_update = conn->createStatement(queryStr);
    stmtMap["gpq_update"] = gpq_update;

    queryStr = "UPDATE Teams"
        " SET playoffStatus = :1"
        " where teamCode = :2";
    Statement* gpq_update2 = conn->createStatement(queryStr);
    stmtMap["gpq_update2"] = gpq_update2;


    // dumpGameRecords Function Queries
    queryStr = "SELECT * FROM Games";
    Statement* dg_select = conn->createStatement(queryStr);
    stmtMap["dg_select"] = dg_select;

    queryStr = "INSERT INTO HistoricalGames ("
        " gameID, homeTeam, awayTeam, gameDate, season,"
        " gameType, winner, goalsHome, goalsAway,"
        " shotsHome, shotsAway, isOvertime, archiveDate"
    ") VALUES ("
        " :1, :2, :3, :4, :5,"
        " :6, :7, :8, :9,"
        " :10, :11, :12, SYSDATE)";
    Statement* dg_insert = conn->createStatement(queryStr);
    stmtMap["dg_insert"] = dg_insert;

    queryStr = "DELETE FROM Games";
    Statement* dg_delete = conn->createStatement(queryStr);
    stmtMap["dg_delete"] = dg_delete;

    // dumpStandings Function Queries

    queryStr = "SELECT * FROM Standings";
    Statement* ds_select = conn->createStatement(queryStr);
    stmtMap["ds_select"] = ds_select;

    queryStr = "INSERT INTO HistoricalStandings ("
        " season, teamCode, teamName, conference, division,"
        " gamesPlayed, wins, losses, overtimeLosses, points,"
        " pointPct, goalsFor, goalsAgainst, goalDifferential, playoffStatus, archiveDate"
    ") VALUES ("
        " :1, :2, :3, :4, :5,"
        " :6, :7, :8, :9, :10,"
        " :11, :12, :13, :14, :15, SYSDATE)";
    Statement* ds_insert = conn->createStatement(queryStr);
    stmtMap["ds_insert"] = ds_insert;

    queryStr = "DELETE FROM Standings";
    Statement* ds_delete = conn->createStatement(queryStr);
    stmtMap["ds_delete"] = ds_delete;

    // skaterGameUpdate Function Queries
    queryStr = "SELECT *"
        " FROM SkaterGames"
        " WHERE playerID = :1 and gameID = :2";
    Statement* skg_search = conn->createStatement(queryStr);
    stmtMap["skg_search"] = skg_search;

    queryStr = "SELECT *"
        " FROM Skaters"
        " WHERE playerID = :1";
    Statement* skg_getStats = conn->createStatement(queryStr);
    stmtMap["skg_getStats"] = skg_getStats;

    queryStr = "UPDATE Skaters SET "
        " goals = :1, assists = :2, points = :3, shots = :4,"
        " hits = :5, plusMinus = :6"
        " WHERE playerID = :7";
    Statement* skg_update = conn->createStatement(queryStr);
    stmtMap["skg_update"] = skg_update;

    queryStr = "SELECT gamesPlayed "
    " FROM Players"
    " WHERE playerID = :2";
    Statement* skg_getGP = conn->createStatement(queryStr);
    stmtMap["skg_getGP"] = skg_getGP;

    queryStr = "UPDATE Players SET "
        " gamesPlayed = :1"
        " WHERE playerID = :2";
    Statement* skg_GPUpdate = conn->createStatement(queryStr);
    stmtMap["skg_GPUpdate"] = skg_GPUpdate;

    // goaltenderGameUpdate Function Queries
    queryStr = "SELECT *"
        " FROM GoaltenderGames"
        " WHERE playerID = :1 and gameID = :2";
    Statement* gog_search = conn->createStatement(queryStr);
    stmtMap["gog_search"] = gog_search;

    queryStr = "SELECT *"
        " FROM Goaltenders"
        " WHERE playerID = :1";
    Statement* gog_getStats = conn->createStatement(queryStr);
    stmtMap["gog_getStats"] = gog_getStats;

    queryStr = "UPDATE Goaltenders SET "
    " wins = :1, losses = :2, OTLosses = :3, savePct = :4,"
    " saves = :5, goalsAgainst = :6, GAA = :7"
    " WHERE playerID = :8";
    Statement* gog_update = conn->createStatement(queryStr);
    stmtMap["gog_update"] = gog_update;

    // addPlayer Function Queries
    queryStr = "SELECT teamCode, skaterCount, goaltenderCount, salaryCapHit"
        " FROM Teams"
        " WHERE teamCode = :1";
    Statement* add_teamData = conn->createStatement(queryStr);
    stmtMap["add_teamData"] = add_teamData;
    
    queryStr = "INSERT INTO Players ("
        "playerID, name, team, salary, nationality, birthdate, position, gamesPlayed, status, jerseyNumber"
        ") VALUES (:1, :2, :3, :4, :5, TO_DATE(:6, 'YYYY-MM-DD'), :7, 0, 'Active', :8)";
    Statement* add_player = conn->createStatement(queryStr);
    stmtMap["add_player"] = add_player;

    queryStr = "INSERT INTO Skaters ("
        "playerID, goals, assists, points, shots, hits, plusMinus"
        ") VALUES (:1, 0, 0, 0, 0, 0, 0)";
    Statement* add_skater = conn->createStatement(queryStr);
    stmtMap["add_skater"] = add_skater;

    queryStr = "INSERT INTO Goaltenders ("
        "playerID, wins, losses, OTLosses, savePct, saves, goalsAgainst, GAA"
        ") VALUES (:1, 0, 0, 0, 0.000, 0, 0, 0.00)";
    Statement* add_goalie = conn->createStatement(queryStr);
    stmtMap["add_goalie"] = add_goalie;

    queryStr = "SELECT max(playerID)"
        " FROM Players";
    Statement* add_getID = conn->createStatement(queryStr);
    stmtMap["add_getID"] = add_getID;

    queryStr = "UPDATE Teams SET"
        " salaryCapHit = :1, skaterCount = :2, goaltenderCount = :3"
        " WHERE teamCode = :4";
    Statement* add_updateTeam = conn->createStatement(queryStr);
    stmtMap["add_updateTeam"] = add_updateTeam;


    // VIEWER QUERIES

    // skaterStats() Function Queries
    queryStr = "SELECT P.name, S.points"
        " FROM Players P NATURAL JOIN Skaters S"
        " ORDER BY S.points DESC";
    Statement* sks_points = conn->createStatement(queryStr);
    stmtMap["sks_points"] = sks_points;


    queryStr = "SELECT P.name, S.goals"
        " FROM Players P NATURAL JOIN Skaters S"
        " ORDER BY S.goals DESC";
    Statement* sks_goals = conn->createStatement(queryStr);
    stmtMap["sks_goals"] = sks_goals;

    queryStr = "SELECT P.name, S.assists"
        " FROM Players P NATURAL JOIN Skaters S"
        " ORDER BY S.assists DESC";
    Statement* sks_assists = conn->createStatement(queryStr);
    stmtMap["sks_assists"] = sks_assists;

    queryStr = "SELECT P.name, P.gamesPlayed"
        " FROM Players P NATURAL JOIN Skaters S"
        " ORDER BY P.gamesPlayed DESC";
    Statement* sks_gp = conn->createStatement(queryStr);
    stmtMap["sks_gp"] = sks_gp;

    // goaltenderStats() Function Queries

    queryStr = "SELECT P.name, G.wins"
        " FROM Players P NATURAL JOIN Goaltenders G"
        " ORDER BY G.wins DESC";
    Statement* gos_wins = conn->createStatement(queryStr);
    stmtMap["gos_wins"] = gos_wins;

    queryStr = "SELECT P.name, G.savePct"
        " FROM Players P NATURAL JOIN Goaltenders G"
        " ORDER BY G.savePct DESC";
    Statement* gos_savePct = conn->createStatement(queryStr);
    stmtMap["gos_savePct"] = gos_savePct;

    queryStr = "SELECT P.name, G.saves"
        " FROM Players P NATURAL JOIN Goaltenders G"
        " ORDER BY G.saves DESC";
    Statement* gos_saves = conn->createStatement(queryStr);
    stmtMap["gos_saves"] = gos_saves;

    queryStr = "SELECT P.name, G.GAA"
        " FROM Players P NATURAL JOIN Goaltenders G"
        " ORDER BY G.GAA ASC";
    Statement* gos_GAA = conn->createStatement(queryStr);
    stmtMap["gos_GAA"] = gos_GAA;

    queryStr = "SELECT P.name, G.goalsAgainst"
        " FROM Players P NATURAL JOIN Goaltenders G"
        " ORDER BY G.goalsAgainst ASC";
    Statement* gos_GA = conn->createStatement(queryStr);
    stmtMap["gos_GA"] = gos_GA;

    queryStr = "SELECT P.name, P.gamesPlayed"
        " FROM Players P NATURAL JOIN Goaltenders G"
        " ORDER BY P.gamesPlayed DESC";
    Statement* gos_gp = conn->createStatement(queryStr);
    stmtMap["gos_gp"] = gos_gp;

    // viewStandings() Function Queries
    queryStr = "SELECT teamCode, gamesPlayed, wins, losses, overtimeLosses, points, pointPct, playoffStatus"
        " FROM Standings"
        " ORDER BY points DESC";
    Statement* vst_league = conn->createStatement(queryStr);
    stmtMap["vst_league"] = vst_league;   

    queryStr = "SELECT division, teamCode, gamesPlayed, wins, losses, overtimeLosses, points, pointPct, playoffStatus"
        " FROM Standings"
        " ORDER BY conference, division, points DESC";
    Statement* vst_division = conn->createStatement(queryStr);
    stmtMap["vst_division"] = vst_division;   

    queryStr = "SELECT teamCode, goalsFor, goalsAgainst, goalDifferential"
        " FROM Standings"
        " ORDER BY goalDifferential DESC";
    Statement* vst_goaldiff = conn->createStatement(queryStr);
    stmtMap["vst_goaldiff"] = vst_goaldiff;   
};

DBObject::~DBObject() {
    cout << "Terminating statements and closing connection." << endl;
    for (map<string, Statement*>::iterator it = stmtMap.begin(); it != stmtMap.end(); ++it) {
        conn->terminateStatement(it->second);
    }
    stmtMap.clear();
    env->terminateConnection(conn);
    Environment::terminateEnvironment(env);
}