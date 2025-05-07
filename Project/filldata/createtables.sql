DROP TABLE SkaterGames;
DROP TABLE GoaltenderGames;
DROP TABLE HistoricalGames;
DROP TABLE HistoricalStandings;
DROP TABLE Standings;
DROP TABLE Skaters;
DROP TABLE Goaltenders;
DROP TABLE Players;
DROP TABLE Games;
DROP TABLE Teams;


CREATE TABLE Teams (
    teamCode CHAR(3) PRIMARY KEY,
    location VARCHAR(20),
    name VARCHAR(20),
    salaryCapHit NUMBER,
    skaterCount NUMBER CHECK(skaterCount <= 21),
    goaltenderCount NUMBER CHECK(goaltenderCount <= 3),
    conference VARCHAR(10) CHECK(conference IN ('Western', 'Eastern')),
    division VARCHAR(15) CHECK(division IN ('Pacific', 'Central', 'Metropolitan', 'Atlantic')),
    playoffStatus CHAR(1)
);

CREATE TABLE Players (
    playerID NUMBER PRIMARY KEY,
    name VARCHAR(50),
    team CHAR(3),
    salary NUMBER,
    nationality VARCHAR(30) NOT NULL,
    birthdate DATE NOT NULL,
    position VARCHAR(2) CHECK(position IN ('C', 'RW', 'LW', 'D', 'G')),
    gamesPlayed NUMBER,
    status VARCHAR(10) DEFAULT 'Active' CHECK(status IN ('Active', 'Inactive', 'Retired')),
    jerseyNumber NUMBER CHECK(jerseyNumber >= 1 AND jerseyNumber <= 99),
    FOREIGN KEY (team) REFERENCES Teams(teamCode)
);

CREATE TABLE Skaters (
    playerID NUMBER PRIMARY KEY REFERENCES Players(playerID),
    goals NUMBER,
    assists NUMBER,
    points NUMBER,
    shots NUMBER,
    hits NUMBER,
    plusMinus NUMBER,
    TOI VARCHAR(6) DEFAULT '0:00'
);

CREATE TABLE Goaltenders (
    playerID NUMBER PRIMARY KEY REFERENCES Players(playerID),
    wins NUMBER,
    losses NUMBER,
    OTLosses NUMBER,
    savePct NUMBER(4, 3) DEFAULT 0.000 CHECK(savePct >= 0 and savePct <= 1),
    saves NUMBER,
    goalsAgainst NUMBER,
    GAA NUMBER(4, 2) DEFAULT 0.00
);

CREATE TABLE Games (
    gameID NUMBER PRIMARY KEY,
    homeTeam CHAR(3), 
    awayTeam CHAR(3),
    gameDate DATE,
    season VARCHAR(20),
    gameType VARCHAR(10) DEFAULT 'Regular' CHECK(gameType IN ('Regular', 'Playoff')),
    winner CHAR(3),
    goalsHome NUMBER,
    goalsAway NUMBER,
    shotsHome NUMBER,
    shotsAway NUMBER,
    isOvertime CHAR(1) DEFAULT 'N' CHECK(isOvertime IN ('Y', 'N')),
    FOREIGN KEY (homeTeam) REFERENCES Teams(teamCode),
    FOREIGN KEY (awayTeam) REFERENCES Teams(teamCode),
    FOREIGN KEY (winner) REFERENCES Teams(teamCode)
);

CREATE TABLE SkaterGames (
    playerID NUMBER,
    gameID NUMBER,
    goals NUMBER,
    assists NUMBER,
    points NUMBER,
    shots NUMBER,
    hits NUMBER,
    plusMinus NUMBER,
    TOI VARCHAR(6) DEFAULT '0:00',
    FOREIGN KEY (playerID) REFERENCES Skaters(playerID),
    PRIMARY KEY (playerID, gameID)
);

CREATE TABLE GoaltenderGames (
    playerID NUMBER,
    gameID NUMBER,
    win NUMBER,
    loss NUMBER,
    OTLoss NUMBER,
    savePct NUMBER(4, 3) DEFAULT 0.000 CHECK(savePct >= 0 and savePct <= 1),
    saves NUMBER,
    goalsAgainst NUMBER,
    GAA NUMBER(4, 2) DEFAULT 0.00,
    FOREIGN KEY (playerID) REFERENCES Goaltenders(playerID),
    PRIMARY KEY (playerID, gameID)
);

CREATE TABLE Standings (
    season VARCHAR(20),
    teamCode CHAR(3),
    teamName VARCHAR(40),
    conference VARCHAR(10) CHECK(conference IN ('Western', 'Eastern')),
    division VARCHAR(15) CHECK(division IN ('Pacific', 'Central', 'Metropolitan', 'Atlantic')),
    gamesPlayed NUMBER,
    wins NUMBER,
    losses NUMBER,
    overtimeLosses NUMBER,
    points NUMBER,
    pointPct NUMBER(4, 3),
    goalsFor NUMBER,
    goalsAgainst NUMBER,
    goalDifferential NUMBER,
    playoffStatus CHAR(1) DEFAULT '-',
    PRIMARY KEY (season, teamCode),
    FOREIGN KEY (teamCode) REFERENCES Teams(teamCode)
);


CREATE TABLE HistoricalStandings (
    season VARCHAR(20),
    teamCode CHAR(3),
    teamName VARCHAR(40),
    conference VARCHAR(10) CHECK(conference IN ('Western', 'Eastern')),
    division VARCHAR(15) CHECK(division IN ('Pacific', 'Central', 'Metropolitan', 'Atlantic')),
    gamesPlayed NUMBER,
    wins NUMBER,
    losses NUMBER,
    overtimeLosses NUMBER,
    points NUMBER,
    pointPct NUMBER(4, 3),
    goalsFor NUMBER,
    goalsAgainst NUMBER,
    goalDifferential NUMBER,
    playoffStatus CHAR(1) DEFAULT '-',
    archiveDate DATE DEFAULT SYSDATE,
    PRIMARY KEY (season, teamCode),
    FOREIGN KEY (teamCode) REFERENCES Teams(teamCode)
);

CREATE TABLE HistoricalGames (
    gameID NUMBER PRIMARY KEY,
    homeTeam CHAR(3), 
    awayTeam CHAR(3), 
    gameDate DATE,
    season VARCHAR(20),
    gameType VARCHAR(10) DEFAULT 'Regular' CHECK(gameType IN ('Regular', 'Playoff')),
    winner CHAR(3),
    goalsHome NUMBER,
    goalsAway NUMBER,
    shotsHome NUMBER,
    shotsAway NUMBER,
    isOvertime CHAR(1) DEFAULT 'N' CHECK(isOvertime IN ('Y', 'N')),
    archiveDate DATE DEFAULT SYSDATE,
    FOREIGN KEY (homeTeam) REFERENCES Teams(teamCode),
    FOREIGN KEY (awayTeam) REFERENCES Teams(teamCode),
    FOREIGN KEY (winner) REFERENCES Teams(teamCode)
);

-- CREATE MATERIALIZED VIEW Standings
-- BUILD IMMEDIATE
-- REFRESH ON COMMIT
-- AS
-- SELECT
--     base.teamCode,
--     base.teamName,
--     base.conference,
--     base.division,
--     base.gamesPlayed,
--     base.wins,
--     base.losses,
--     base.overtimeLosses,
--     2 * base.wins + base.overtimeLosses AS points,
--     2 * base.wins + base.overtimeLosses / (base.gamesPlayed * 2) AS pointPct,
--     base.goalsForHome + base.goalsForAway AS goalsFor,
--     base.goalsAgainstHome + base.goalsAgainstAway AS goalsAgainst,
--     (base.goalsForHome + base.goalsForAway) - (base.goalsAgainstHome + base.goalsAgainstAway) AS goalDifferential,
--     base.playoffStatus
--     *
-- FROM (
--     SELECT
--         t.teamCode,
--         t.location || ' ' || t.name AS teamName,
--         t.conference,
--         t.division,
--         -- Games Played
--         (SELECT COUNT(*) 
--         FROM Games g 
--         WHERE (g.homeTeam = t.teamCode OR g.awayTeam = t.teamCode)) AS gamesPlayed,
--         -- Wins
--         (SELECT COUNT(*) 
--         FROM Games g 
--         WHERE g.winner = t.teamCode) AS wins,
--         -- Losses
--         (SELECT COUNT(*) 
--         FROM Games g 
--         WHERE (g.homeTeam = t.teamCode OR g.awayTeam = t.teamCode)
--         AND g.winner != t.teamCode
--         AND g.winner IS NOT NULL
--         AND g.isOvertime = 'N') AS losses,
--         -- Overtime Losses
--         (SELECT COUNT(*) 
--         FROM Games g 
--         WHERE (g.homeTeam = t.teamCode OR g.awayTeam = t.teamCode)
--         AND g.winner != t.teamCode
--         AND g.winner IS NOT NULL
--         AND g.isOvertime = 'Y') AS overtimeLosses,
--         -- Goals For
--         (SELECT SUM(goalsHome)
--         FROM Games g 
--         WHERE (g.homeTeam = t.teamCode)) as goalsForHome,
--         (SELECT SUM(goalsAway)
--         FROM Games g 
--         WHERE (g.awayTeam = t.teamCode)) as goalsForAway,
--         -- Goals Against
--         (SELECT SUM(goalsAway)
--         FROM Games g 
--         WHERE (g.homeTeam = t.teamCode)) AS goalsAgainstHome,
--         (SELECT SUM(goalsHome)
--         FROM Games g 
--         WHERE (g.awayTeam = t.teamCode)) AS goalsAgainstAway,
--         t.playoffStatus    
--     FROM Teams t
--     LEFT JOIN Games g 
--         ON (t.teamCode = g.homeTeam OR t.teamCode = g.awayTeam)
--         AND g.season = '2024-25'
--         AND g.gameType = 'Regular'
--     GROUP BY t.teamCode, t.location || ' ' || t.name, t.conference, t.division, t.playoffStatus
-- ) base;