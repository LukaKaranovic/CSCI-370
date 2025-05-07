/**
 * @file a8.cpp
 * @author Luka Karanovic, 665778833, S25N01, CSCI 370, VIU
 * @version 1.0.0
 * @date March 18, 2025
 *
 * Contains the main routine for my implementation of A8.
*/

#include "functions.h"
#include <iostream>
#include <cstdlib>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <stdexcept>
#include <fstream>

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

int main(int argc, char** argv) {
    string username;
    string password;

    const string connectString = "database.csci.viu.ca";

    cout << "Your username: ";
    getline(cin, username);

    cout << "Your password: ";
    password = readPassword();
    cout << endl;
    try {
        // Check for garbage inputs before initiating a connection to the database.
        try {
            checkArgs(argc, argv);
        } catch (invalid_argument& e) {
            cout << e.what() << endl;
            exit(1);
        }
        // Establish database connection
        Environment* env = Environment::createEnvironment();
        Connection* conn = env->createConnection(username, password, connectString);
        string queryStr = "";
        string year = argv[1];
        string accnum = argv[2];

        queryStr = "select * from MemberAccounts where accnum = :1";
        Statement* accountCheck = conn->createStatement(queryStr);
        try {
            checkAccNum(accountCheck, accnum);
        } catch (invalid_argument& e) {
            cout << e.what() << endl;
            exit(1);
        }
        // Create output file
        ofstream output;
        output.open("Report.txt");
        if (!output.is_open()) {
            cout << "Unable to open output file. Check if it exists or if it has read permissions." << endl;
            cout << "File should be located in the same directory as the executable and should have the name 'Report.txt'." << endl;
            exit(1);
        } 
        queryStr = "select packageType, count(*) from Orders" 
        " where placeTime >= to_date(:1, 'dd-mon-yyyy')" 
        " and placeTime <= to_date(:2, 'dd-mon-yyyy') and placedBy = :3"
        " group by packageType";

        Statement* orderStats = conn->createStatement(queryStr);
        genMemberReport(output, accnum, year, accountCheck, orderStats);

        queryStr = "select * from Producers where accnum = :1";
        Statement* producerCheck = conn->createStatement(queryStr);

        queryStr = "select productCode, F.name, F.unit, sum(quantity)"
        " from SupplyRecords S natural join FoodItems F"
        " where S.accnum = :1 and S.supplyDate >= to_date(:2, 'dd-mon-yyyy')" 
        " and S.supplyDate <= to_date(:3, 'dd-mon-yyyy')"
        " group by productCode, F.name, F.unit";
        Statement* contributions = conn->createStatement(queryStr);

        
        if (isProducer(producerCheck, accnum)) {
            genProducerReport(output, accnum, year, producerCheck, contributions);
        }

        output << endl << "====== END OF REPORT ======";
        output.close();
        // Close statements and terminate database connection.
        conn->terminateStatement(accountCheck);
        conn->terminateStatement(orderStats);
        conn->terminateStatement(producerCheck);
        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    } catch (SQLException & e) {
        cout << e.what();
        return 0;
    }

    return 0;
}