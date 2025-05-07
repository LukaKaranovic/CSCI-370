/**
 * @file functions.cpp
 * @author Luka Karanovic, 665778833, S25N01, CSCI 370, VIU
 * @version 1.0.0
 * @date March 18, 2025
 *
 * Header file for functions.cpp, contains definitions for helper functions that handle command line input checking and error handling, and the report generation for A8.
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <string>
#include <occi.h>  // If Oracle OCCI is used

void checkArgs(int, char**);
void checkAccNum(oracle::occi::Statement*, std::string);
bool isProducer(oracle::occi::Statement*, std::string);
void genMemberReport(std::ostream&, std::string, std::string, oracle::occi::Statement*, oracle::occi::Statement*);
void genProducerReport(std::ostream&, std::string, std::string, oracle::occi::Statement*, oracle::occi::Statement*);

#endif // FUNCTIONS_H
