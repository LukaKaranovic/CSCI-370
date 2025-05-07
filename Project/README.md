# Introduction / Use of the product

This is a hockey database system that can be used by either league managers/updaters or fans of the game. 

The domain I will be designing my database system project around is the National Hockey League (NHL). My plan is to make a player and team statistics database for my own hockey league (using NHL rules). Statisticians can use this system to create narratives or find trends (for certain players, time periods, or teams) in hockey. This database will also be used by league managers, who can put in game data that will automatically update other tables.

This is a project for CSCI 370.

# Directory and File Structure

There are four main directories in this repository.
- `filldata` - Contains the sample data used to populate my database when initialized. These scripts contain hundreds of records.
    - `createtables.sql` - Drops and creates all tables in the database.
    - `teamplayerdata.sql` -  Inserts 32 teams and hundreds of player, skater, and goaltender tuples.
    - `gamedata.sql` - Inserts hundreds of game records.
- `testdata` - Contains additional sample data used to test certain functions.
    - `playergametest.sql` - Inserts some records into the SkaterGames and GoaltenderGames tables for testing.
    - `updatestandingstest.sql` - Inserts a few extra game records to test my `updateStandings()` function.
- `build` - Contains the object files when compiling the application.
- `src` - Contains the source code files for my database application.
    - `main.cpp` - The main routine for league managers.
    - `viewermain.cpp` - The main routine for viewers.
    - `DBObject.h` - Header file for `DBObject` class.
    - `DBObject.cpp` - Implements the constructor and destructor of `DBObject` class.
    - `managerfunc.cpp` - Implements every function associated with a league manager command. These functions are part of the `DBObject` class.
    - `viewerfunc.cpp` - Implements every function associated with a viewer command. These functions are part of the `DBObject` class.

There are a few extra files within this repository:
- `Makefile` - Holds commands for compiling the application.
- `README.md` - Describes the use of the product, file structure, how to run it, and the license.
- `Report.md` - The report and documentation portion of the project.
    - `testing.pdf` - Appendid for testing documentation of the project.

# Set up and run the application

Follow these steps:
1. Be in the `Project` directory.
2. Type `make` to compile the files.
3. Go into sqlplus and execute the following commands:
    - @filldata/createtables.sql 
    - @filldata/teamplayerdata.sql 
    - @filldata/gamedata.sql 
    - Optionally:
        - @testdata/updatestandingstest.sql
        - @testdata/playergametest.sql
4. To execute the league manager version of the application. Type `./main`.
5. To execute the viewer version of the application. Type `./viewermain`.
6. Once you are in the application, you will be greeted. Follow the instructions on the terminal and enjoy!

# License

MIT License

Copyright (c) 2025 Luka Karanovic

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.