#include "ReadTextInput.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "factory.hpp"

using std::string;

string ReadTextInput::getDocs(){
    return string(
"System that can read a text file (input 'filename') containg timestamps and values.\n\n"

"The values for each time is stored in the output variables column_1,column2,"
"etc. They can be connected to other systems and used as input. \n It is "
"expected that the text file is well formatted with each line beginning with"
"a timestamp and then followed by the same number of columns for every line."
"The columns should be separated by whitespace."
);
}

//Sets each element of the member array "columns" as an output called "column_i".
//Also creates an input string that will represent the name of the text file.
ReadTextInput::ReadTextInput(void):
    filename("default.txt")
{
    PAR(filename, "Name of the file containing the input data")

    for (size_t i = 1; i <= NUMBER_OF_COLUMNS; i++) {
        char name[100];
        sprintf(name, "column_%lu", i);
        output(&(columns[i-1]), name, "Output value read from column in file");
        columns[i - 1] = 0;
    }
}


//In preparation for the simulation run this function reads the input file
//and stores the values of it in a map. The file is expected to have
//the time as first value on each line, followed by the values for each 
//column.
void ReadTextInput::preSim() {

    std::ifstream file(filename);
    std::string line;
    int linenumber = 0;

    while (!file.eof()){
        string line;
        std::vector<double> linevalues;
        getline(file, line);
        linenumber++;

        std::stringstream ss(line);
        double time;
        ss >> time;
        if (ss.fail()) {
            continue;
        }
        for (size_t i = 0; (i < NUMBER_OF_COLUMNS) && (!ss.fail()); i++) {
            double value;
            ss >> value;
            linevalues.push_back(value);
        }
        valuemap[time] = linevalues;
    }
}

//At each step the values for the current time are store in the member
//variable "columns". If there is no entry for the current time the 
//first existing time prior to the current time is used. 
void ReadTextInput::doStep(double time){

    //Get the first time in the map which is larger than the current
    //time, and then decrease the iterator to get the last time that
    //is lower than the value.
    auto i = valuemap.upper_bound(time);
    i--;

    //Iterate over the columns for the selected time and fill in 
    //the member variable.
    std::vector<double> vv = i->second;
    size_t currentColumn = 0;
    for (auto j = vv.begin(); j != vv.end();  j++) {
        columns[currentColumn++] = *j;
    }
}

REGISTER_SYSTEM(ReadTextInput);
