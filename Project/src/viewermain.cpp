// File to make functions for league managers.

#include "DBObject.h"
#include <ctime>
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
    cout << "skaterstats: View the stats leaderboards for skaters." << endl;
    cout << "goaliestats: View the stats leaderboards for goaltenders." << endl;
    cout << "standings: View the current standings." << endl;
    cout << "terminate: Terminates the program." << endl;
}


int main() {
    try {
        DBObject hockeyDB;
        cout << "Welcome to the league manager view of the hockey database!" << endl << endl;
        displayCommands();
        string command;

        while (command != "terminate") {
            cout << endl << "Type 'help' to display all commands." << endl;
            getCommand(command);
            if (command == "help") {
                displayCommands();
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