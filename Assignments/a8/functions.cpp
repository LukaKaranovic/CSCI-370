/**
 * @file functions.cpp
 * @author Luka Karanovic, 665778833, S25N01, CSCI 370, VIU
 * @version 1.0.0
 * @date March 18, 2025
 *
 * Contains helper functions that handle command line input checking and error handling, and the report generation for A8.
 */

#include "functions.h"
#include <iostream>
#include <cstdlib>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <stdexcept>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace oracle::occi;

void checkArgs(int argc, char** argv) {
    int year;
    string accnum;
    if (argc != 3) {
        throw invalid_argument("Expected use is './a8 year account_number', where year is the year you want data from and account_number is the account number of the member you want the data for: \n E.g. './a8 2021 000001' wlll report all data from year 2021 for user 000001.");
    }
    try {
        year = atoi(argv[1]);
        accnum = argv[2];
    } catch (invalid_argument& e) {
        throw invalid_argument("Inputs are not of correct data type, make sure that argument 1, year, is a valid integer, and that argument 2, account_number, is a 6-digit number.");
    }
    if (year < 2020 || year > 2024) {
        throw invalid_argument("Year number is invalid, year must be between 2020-2024 (inclusive), as those are the full years the Food Sharing Program has been operational.");
    }
    if (accnum.length() != 6) { 
        throw invalid_argument("Account number is invalid, account numbers must be six digits long: E.g. 000001");
    }
}


void checkAccNum(Statement* accountCheck, string accnum) {
    accountCheck->setString(1, accnum);
    ResultSet* rs = accountCheck->executeQuery();

    if (!rs->next()) {
        accountCheck->closeResultSet(rs);
        throw invalid_argument("Account number does not exist in Food Sharing Program.");
    }
    accountCheck->closeResultSet(rs);
}

bool isProducer(Statement* producerCheck, string accnum) {
    producerCheck->setString(1, accnum);
    ResultSet* rs = producerCheck->executeQuery();

    if (!rs->next()) {
        producerCheck->closeResultSet(rs);
        return false;
    }
    producerCheck->closeResultSet(rs);
    return true;
}

void genMemberReport(ostream& output, string accnum, string year, Statement* accountCheck, Statement* orderStats) {
    string contactName;
    string deliveryAddress;
    string status;
    
    accountCheck->setString(1, accnum);
    ResultSet* rs = accountCheck->executeQuery();
    if (rs->next()) {
        contactName = rs->getString(2);
        deliveryAddress = rs->getString(4);
        status = rs->getString(7);
    }

    // Output member data
    output << "Summary report of year " << year << endl << endl;

    output << "Account Number:   " << accnum << endl;
    output << "Contact Name:     " << contactName << endl;
    output << "Delivery Address: " << deliveryAddress << endl;
    output << "Current Status:   " << status << endl << endl;

    // Output order data of member for year

    output << "Order history of year " << year << ": " << endl;
    output << "Package Size     Amount Ordered" << endl;
    output << "------------     --------------" << endl;
    
    string startDate = "01-Jan-" + year;
    string endDate = "31-Dec-" + year;

    orderStats->setString(1, startDate);
    orderStats->setString(2, endDate);
    orderStats->setString(3, accnum);

    ResultSet* orders = orderStats->executeQuery();

    while (orders->next()) {
        string packageType = orders->getString(1);
        int amount = orders->getInt(2);
        output << left << setw(17) << packageType << amount << endl;
    }

    accountCheck->closeResultSet(rs);
    orderStats->closeResultSet(orders);
}

void genProducerReport(ostream& output, string accnum, string year, Statement* producerCheck, Statement* contributions) {
    producerCheck->setString(1, accnum);
    ResultSet* rs = producerCheck->executeQuery();
    string farmAddress;
    if (rs->next()) {
        farmAddress = rs->getString(2);
    }
    output << endl << "This member is also a producer." << endl;
    output << "Farm Address:   " << farmAddress << endl << endl;
    output << "Contribution history of year " << year << ":" << endl;
    output << "Product Code       Food Item Name       Unit       Total Amount" << endl;
    output << "------------       --------------       ----       -----------" << endl;

    string startDate = "01-Jan-" + year;
    string endDate = "31-Dec-" + year;

    contributions->setString(1, accnum);
    contributions->setString(2, startDate);
    contributions->setString(3, endDate);

    ResultSet* contrib = contributions->executeQuery();


    while (contrib->next()) {
        string pc = contrib->getString(1);
        string name = contrib->getString(2);
        string unit = contrib->getString(3);
        int amount = contrib->getInt(4);
        output << left << setw(19) << pc << setw(21) << name << setw(11) << unit << setw(10) << amount << endl;
    }

    producerCheck->closeResultSet(rs);
    contributions->closeResultSet(contrib);
}