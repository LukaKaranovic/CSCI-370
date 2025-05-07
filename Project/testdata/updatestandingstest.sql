-- Game 1: Tampa Bay Lightning vs. Calgary Flames

-- Insert Game Record
INSERT INTO Games (gameID, homeTeam, awayTeam, gameDate, season, gameType, winner, goalsHome, goalsAway, shotsHome, shotsAway, isOvertime)
VALUES (260, 'TBL', 'CGY', TO_DATE('2025-04-15', 'YYYY-MM-DD'), '2025 Season', 'Regular', 'TBL', 4, 3, 35, 30, 'N');

-- Insert Skater Stats for Tampa Bay Lightning
INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (431, 260, 1, 2, 3, 5, 2, 1, '20:30'); -- Player 1, Example

INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (432, 260, 2, 1, 3, 4, 3, 2, '18:40'); -- Player 2, Example

INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (433, 260, 0, 1, 1, 2, 1, -1, '16:00'); -- Player 3, Example

-- Add other skaters for TBL, following the same format

-- Insert Skater Stats for Calgary Flames
INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (501, 260, 1, 0, 1, 4, 2, 1, '19:00'); -- Player 1, Example

INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (502, 260, 0, 1, 1, 3, 1, -1, '17:30'); -- Player 2, Example

INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (503, 260, 1, 2, 3, 6, 4, 2, '21:00'); -- Player 3, Example

-- Add other skaters for CGY, following the same format

-- Insert Goaltender Stats for Tampa Bay Lightning
INSERT INTO GoaltenderGames (playerID, gameID, wins, losses, OTLosses, savePct, saves, goalsAgainst, GAA)
VALUES (601, 260, 1, 0, 0, 0.920, 27, 3, 2.40); -- Goalie 1 for TBL

-- Insert Goaltender Stats for Calgary Flames
INSERT INTO GoaltenderGames (playerID, gameID, wins, losses, OTLosses, savePct, saves, goalsAgainst, GAA)
VALUES (602, 260, 0, 1, 0, 0.875, 26, 4, 3.20); -- Goalie 1 for CGY


-- Game 2: New York Rangers vs. Montreal Canadiens

-- Insert Game Record
INSERT INTO Games (gameID, homeTeam, awayTeam, gameDate, season, gameType, winner, goalsHome, goalsAway, shotsHome, shotsAway, isOvertime)
VALUES (261, 'NYR', 'MTL', TO_DATE('2025-04-16', 'YYYY-MM-DD'), '2025 Season', 'Regular', 'NYR', 5, 2, 40, 28, 'N');

-- Insert Skater Stats for New York Rangers
INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (601, 261, 1, 1, 2, 6, 3, 1, '21:10'); -- Player 1, Example

INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (602, 261, 2, 1, 3, 5, 4, 2, '19:20'); -- Player 2, Example

INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (603, 261, 1, 1, 2, 4, 3, -1, '17:40'); -- Player 3, Example

-- Add other skaters for NYR, following the same format

-- Insert Skater Stats for Montreal Canadiens
INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (701, 261, 1, 0, 1, 4, 2, -2, '18:30'); -- Player 1, Example

INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (702, 261, 0, 1, 1, 3, 3, -1, '19:50'); -- Player 2, Example

INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (703, 261, 1, 1, 2, 5, 1, 0, '20:00'); -- Player 3, Example

-- Add other skaters for MTL, following the same format

-- Insert Goaltender Stats for New York Rangers
INSERT INTO GoaltenderGames (playerID, gameID, wins, losses, OTLosses, savePct, saves, goalsAgainst, GAA)
VALUES (801, 261, 1, 0, 0, 0.920, 26, 2, 2.20); -- Goalie 1 for NYR

-- Insert Goaltender Stats for Montreal Canadiens
INSERT INTO GoaltenderGames (playerID, gameID, wins, losses, OTLosses, savePct, saves, goalsAgainst, GAA)
VALUES (802, 261, 0, 1, 0, 0.880, 23, 5, 3.40); -- Goalie 1 for MTL

-- Game 3: Boston Bruins vs. Vegas Golden Knights

-- Insert Game Record
INSERT INTO Games (gameID, homeTeam, awayTeam, gameDate, season, gameType, winner, goalsHome, goalsAway, shotsHome, shotsAway, isOvertime)
VALUES (262, 'BOS', 'VGK', TO_DATE('2025-04-17', 'YYYY-MM-DD'), '2025 Season', 'Regular', 'BOS', 3, 2, 38, 29, 'N');

-- Insert Skater Stats for Boston Bruins
INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (901, 262, 1, 0, 1, 4, 5, 1, '20:10'); -- Player 1, Example

INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (902, 262, 0, 2, 2, 3, 2, 2, '19:00'); -- Player 2, Example

INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (903, 262, 1, 0, 1, 6, 3, 1, '21:20'); -- Player 3, Example

-- Add other skaters for BOS, following the same format

-- Insert Skater Stats for Vegas Golden Knights
INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (1001, 262, 0, 1, 1, 4, 3, -1, '19:30'); -- Player 1, Example

INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (1002, 262, 1, 0, 1, 5, 4, 0, '20:00'); -- Player 2, Example

INSERT INTO SkaterGames (playerID, gameID, goals, assists, points, shots, hits, plusMinus, TOI)
VALUES (1003, 262, 1, 1, 2, 7, 2, 1, '22:00'); -- Player 3, Example

-- Add other skaters for VGK, following the same format

-- Insert Goaltender Stats for Boston Bruins
INSERT INTO GoaltenderGames (playerID, gameID, wins, losses, OTLosses, savePct, saves, goalsAgainst, GAA)
VALUES (1101, 262, 1, 0, 0, 0.930, 27, 2, 2.00); -- Goalie 1 for BOS

-- Insert Goaltender Stats for Vegas Golden Knights
INSERT INTO GoaltenderGames (playerID, gameID, wins, losses, OTLosses, savePct, saves, goalsAgainst, GAA)
VALUES (1102, 262, 0, 1, 0, 0.875, 30, 3, 3.50); -- Goalie 1 for VGK


