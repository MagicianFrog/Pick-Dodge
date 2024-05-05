#ifndef INSTRCUTIONS_HPP
#define INSTRCUTIONS_HPP

#include <vector>
#include <string>
#include "timer.hpp"
using namespace std;

struct Instructions {
    string name;
    string desc;
    int id;

    Instructions(){};
    Instructions(string _name, string _desc, int _id);
};

extern vector <vector <Instructions>> ins;

void loadInstructions(int tier, string path);

#endif // INSTRCUTIONS_HPP_INCLUDED
