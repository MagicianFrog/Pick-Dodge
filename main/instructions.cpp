#include "instructions.hpp"
#include <fstream>
#include <iostream>
using namespace std;

Instructions::Instructions(string _name, string _desc, int _id){
    this->name = _name;
    this->desc = _desc;
    this->id = _id;
}

vector <vector <Instructions>> ins(2);

void loadInstructions(int tier, string path){
    ifstream file(path);
    if (!file.is_open()){
        cout << "Failed to open: " << path << endl;
        return;
    }

    int id = 0;
    string line;

    while (getline(file, line)){
        string name, desc;
        int i = 0;
        while (i < (int) line.size() && line[i] != '$'){
            name += line[i];
            i++;
        }
        i++;
        while (i < (int) line.size() && line[i] != '$'){
            desc += line[i];
            i++;
        }
        i++;
        Instructions newIns(name, desc, ++id);
        ins[tier].push_back(newIns);
    }
    file.close();
}
