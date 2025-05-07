/**
 * @file a7.cpp
 * @author Luka Karanovic, 665778833, S25N01, CSCI 370, VIU
 * @version 1.0.0
 * @date March 10, 2025
 *
 */

#include <iostream>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <stdexcept>
using namespace std;
using namespace oracle::occi;

static int orderNum = 60000;

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

/** @brief Prompts the user for a command returns it.
 *
 *  Converts the command input to lowercase so it works for the conditional statements in main. This adds user input friendliness.
 *
 *  @param userCommand The string used to store the user's command.
 *  @return The inputted user command in lowercase.
 */

string& getCommand(string& command) {
    cout << "Enter a command: ";
    getline(cin, command);
    int length = command.length();
    for (int i = 0; i < length; i++) {
        command[i] = tolower(command[i]);
    }
    return command;
}

bool isActive(string accNum, Statement* stmt) {
    stmt->setString(1, accNum);
    ResultSet* rs = stmt->executeQuery();

    // Since it should retrieve only 1 row.
    if (rs->next()) {
        bool status = (rs->getString(1) == "Active");
        stmt->closeResultSet(rs);
        return status;
    }
    stmt->closeResultSet(rs);
    throw invalid_argument("Not a valid account number.");
}

bool orderedOnDate(string accNum, string expectDate, Statement* stmt) {
    stmt->setString(1, accNum);
    stmt->setString(2, expectDate);
    stmt->executeQuery();
    ResultSet* rs = stmt->executeQuery();
    // Since it should retrieve only 1 row
    if (rs->next()) {
        if(rs->getInt(1) == 0) {
            stmt->closeResultSet(rs);
            return false;
        }
    }
    stmt->closeResultSet(rs);
    return true;
}

void placeOrder(Statement* stmt, Statement* stmtActive, Statement* stmtOrder) {
    string expectDate = "31-Mar-2025";
    string accNum;
    cout << "Enter your account number: ";
    getline(cin, accNum);
    // Check if account number exists and that the account is active.
    if (!isActive(accNum, stmtActive)) {
        throw invalid_argument("The account with that number isn't active, cannot place order!");
    }

    string packageSize;
    do {
        cout << "What size package do you want (Enter 'S', 'M', or 'L')? ";
        getline(cin, packageSize);
        // To allow lowercase inputs, such as 's', 'm', or 'l' to be valid.
        for (int i = 0; i < packageSize.length(); i++) {
            packageSize[i] = toupper(packageSize[i]);
        }    
    } while (packageSize != "S" && packageSize != "M" && packageSize != "L");

    // Check if the given member has already placed an order for that expected delivery day before.
    if (orderedOnDate(accNum, expectDate, stmtOrder)) {
        throw invalid_argument("Account " + accNum + " already has an order with that expected delivery date.");
    }

    orderNum++;

    stmt->setInt(1, orderNum);
    stmt->setString(2, accNum);
    stmt->setString(3, packageSize);
    stmt->setString(4, expectDate);

    int status = stmt->executeUpdate();
    cout << status << " rows changed." << endl;

    cout << endl;
    cout << "Order " << orderNum << " has been placed." << endl;
    cout << "Account number: " << accNum << endl;
    cout << "Package Size: " << packageSize << endl;
    cout << "Expected delivery date: " << expectDate << endl;
}

void unsubscribe(Statement* stmt, Statement* stmtActive) {
    string accNum;
    cout << "Enter your account number: ";
    getline(cin, accNum);
    // Check if account number exists and that the account is active.
    if (!isActive(accNum, stmtActive)) {
        throw invalid_argument("Account already is inactive, can't unsubscribe!");
    }
    stmt->setString(1, accNum);
    int status = stmt->executeUpdate();
    cout << status << " rows changed." << endl;
    cout << endl;
    cout << "Member account " << accNum << " unsubscribed successfully!" << endl;
}


int main() {
    string username;
    string password;

    const string connectString = "database.csci.viu.ca";

    cout << "Your username: ";
    getline(cin, username);

    cout << "Your password: ";
    password = readPassword();
    cout << endl;
    try {
        // Establish database connection
        Environment* env = Environment::createEnvironment();
        Connection* conn = env->createConnection(username, password, connectString);
        string queryStr = "";

        // Creating prepared statement for placeOrder()
        queryStr = "insert into Orders (orderNum, placedBy, placeTime, packageType, expectDate) values (:1, :2, current_date, :3, to_date(:4, 'dd-mon-yyyy'))";
        Statement* stmtPlace = conn->createStatement(queryStr);

        // Creating prepared statement for isActive()
        queryStr = "select status from MemberAccounts where accnum = :1";
        Statement* stmtActive = conn->createStatement(queryStr);

        // Creating prepared statement for orderedOnDate()
        queryStr = "select count(*) from Orders where placedBy = :1 and expectDate = to_date(:2, 'dd-mon-yyyy')";
        Statement* stmtOrder = conn->createStatement(queryStr);

        // Creating prepared statement for unsubscribe()
        queryStr = "update MemberAccounts set status = 'Inactive' where accNum = :1";
        Statement* stmtUnsub = conn->createStatement(queryStr);

        string command;

        while (command != "terminate") {
            cout << "Commands:" << endl;
            cout << "place_order: Place an order for a particular package size and expected delivery date (active members only)." << endl;
            cout << "unsubscribe: Unsubscribe a particular member account (make status 'inactive')" << endl;
            cout << "terminate: Terminates the program." << endl;
            getCommand(command);
            if (command == "place_order") {
                try {
                    placeOrder(stmtPlace, stmtActive, stmtOrder);
                    conn->commit();
                } catch (invalid_argument& error) {
                    cout << error.what() << endl;
                }
            } else if (command == "unsubscribe") {
                try {
                    unsubscribe(stmtUnsub, stmtActive);
                    conn->commit();
                } catch (invalid_argument& error) {
                    cout << error.what() << endl;
                }
            } else {
                // Command being terminate would run this statement so I put an additional check.
                if (command != "terminate") {
                    cout << "Invalid command. Try again." << endl << endl;
                }
            }
        }

        // Close all statements and terminate connection.
        cout << "Terminating program and closing connection." << endl;
        conn->terminateStatement(stmtPlace);
        conn->terminateStatement(stmtActive);
        conn->terminateStatement(stmtOrder);
        conn->terminateStatement(stmtUnsub);
        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    } catch (SQLException & e) {
        cout << e.what();
        return 0;
    }

    return 0;
}