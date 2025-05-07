// Main for league managers.

#include "DBObject.h"

using namespace std;
using namespace oracle::occi;

string& getCommand(string& command) {
    cout << "Enter a command: ";
    getline(cin, command);
    int length = command.length();
    for (int i = 0; i < length; i++) {
        command[i] = tolower(command[i]);
    }
    return command;
}

void displayCommands() {
    cout << "Available commands:" << endl << endl;
    cout << "help: Displays all commands." << endl;
    cout << "gen_standings: Generate the standings for the current season based on all existing game records." << endl;
    cout << "create_game: Create a singular game record to ADD to the database and have it automatically update the standings." << endl;
    cout << "add_game: Add a singular EXISTING game record from the database and have it automatically update the standings." << endl;
    cout << "playoff: Generate a playoff status for each team based on the current standings." << endl;
    cout << "add_player: Add a player to an existing team in the league." << endl;
    cout << "add_skatergame: Use an EXISTING skater-game record to update skater's stats in skaters table." << endl;
    cout << "add_goaliegame: Use an EXISTING goaltender-game record to update goaltender's stats in goaltenders table." << endl;
    cout << "update_cap: Update the league-wide salary cap." << endl;
    cout << "new_season: Create a new season and update the league-wide current season. WARNING: THIS FUNCTION DUMPS GAMES AND STANDINGS TO HISTORICAL RECORDS." << endl;
    cout << "skaterstats: View the stats leaderboards for skaters." << endl;
    cout << "goaliestats: View the stats leaderboards for goaltenders." << endl;
    cout << "standings: View the current standings." << endl;
    cout << "terminate: Terminates the program." << endl;
}

int main() {
    try {
        DBObject hockeyDB;
        // Variables for inside commands
        int playerID, gameID, cap;
        string season;
        cout << "Welcome to the league manager view of the hockey database!" << endl << endl;
        displayCommands();
        string command;

        while (command != "terminate") {
            cout << endl << "Type 'help' to display all commands." << endl;
            getCommand(command);
            if (command == "help") {
                displayCommands();
            } else if (command == "gen_standings") {
                cout << "Generating standings..." << endl;
                hockeyDB.generateStandings();
            } else if (command == "create_game") {
                hockeyDB.makeGame();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (command == "add_game") {
                cout << "Enter the game ID whose results you want to add to standings:" << endl;
                cin >> gameID;
                if (cin.fail()) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid integer next time." << endl;
                } else {
                    hockeyDB.updateStandings(gameID);
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (command == "playoff") {
                cout << "Calculating playoff statuses..." << endl;
                hockeyDB.genPlayoffQual();
            } else if (command == "add_player") {
                hockeyDB.addPlayerToTeam();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (command == "add_skatergame") {
                cout << "Enter the player ID of the skater whose stats you want to update:" << endl;
                cin >> playerID;
                if (cin.fail()) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid integer next time." << endl;
                } else {
                    cout << "Enter the game ID of the game they played:" << endl;
                    cin >> gameID;
                    if (cin.fail()) {
                        cin.clear(); 
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a valid integer next time." << endl;
                    } else {
                        hockeyDB.skaterGameUpdate(playerID, gameID);
                    }
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

            } else if (command == "add_goaliegame") {
                cout << "Enter the player ID of the skater whose stats you want to update:" << endl;
                cin >> playerID;
                if (cin.fail()) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid integer next time." << endl;
                } else {
                    cout << "Enter the game ID of the game they played:" << endl;
                    cin >> gameID;
                    if (cin.fail()) {
                        cin.clear(); 
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a valid integer next time." << endl;
                    } else {
                        hockeyDB.goaltenderGameUpdate(playerID, gameID);
                    }
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (command == "update_cap") {
                cout << "Current salary cap is " << hockeyDB.getSalaryCap() << endl;
                cout << "What do you want the new salary cap to be?" << endl;
                cin >> cap;
                if (cin.fail()) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid integer next time." << endl;
                } else {
                    hockeyDB.setSalaryCap(cap);
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (command == "new_season") {
                cout << "Current season is " << hockeyDB.getCurrSeason() << endl;
                cout << "What do you want the new season name to be?" << endl;
                cin >> season;
                hockeyDB.setCurrSeason(season);
                cout << "New season started. Dumping all existing game and standings records to historical records." << endl;
                hockeyDB.dumpGameRecords();
                hockeyDB.dumpStandings();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (command == "skaterstats") {
                hockeyDB.skaterStats();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (command == "goaliestats") {
                hockeyDB.goaltenderStats();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (command == "standings") {
                hockeyDB.viewStandings();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (command == "terminate") {
                cout << "Thanks for using the application!" << endl;
            } else {
                cout << "Invalid command, please try again." << endl;
            }
        }
    } catch (SQLException & e) {
        cout << e.what();
        cout << "Terminating program." << endl;
        return 0;
    }
    cout << "Terminating program." << endl;  
    return 0;
}