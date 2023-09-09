/*
Project Name: 235 Lab 09 - Pokemon
Description: Collect, Store and Fight Pokemon
*/

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "Set.h"
#include "HashMap.h"
using namespace std;

//Determines the battle between two pokemon
string battle(string pokemonA, string attackA, string pokemonB, string attackB, HashMap<string,string>& pokemon, HashMap<string,string>& moves, HashMap<string,Set<string>>& effectives, HashMap<string,Set<string>>& ineffectives);

int main(int argc, const char * argv[]) {
    
    VS_MEM_CHECK; //Checks for Memory Leaks
    
    //Opens Files
    if (argc < 3) {
       cerr << "Please provide name of input and output files" << endl;
       return 1;
    }
    cout << "Input file: " << argv[1] << endl;
    ifstream in(argv[1]);
    if (!in) {
       cerr << "Unable to open " << argv[1] << " for input" << endl;
       return 2;
    }
    cout << "Output file: " << argv[2] << endl;
    
    ofstream out(argv[2]);
    if (!out) {
       in.close();
       cerr << "Unable to open " << argv[2] << " for output" << endl;
       return 3;
    }
    
    
    string line, command;
    string animal, move, type, effective, ineffective;
    string pokemonA, attackA, pokemonB, attackB;
    Set<string> set;
    HashMap<string,string> pokemon;
    HashMap<string,string> moves;
    HashMap<string,Set<string>> effectives;
    HashMap<string,Set<string>> ineffectives;
    
    //Reads and Inputs Various Commands
    while(getline(in, line)) {
        if (line == "") continue;
        istringstream iss(line);
        iss >> command;
        if (command != "Set:" && command != "Pokemon:" && command != "Move:" && command != "Effective:" && command != "Ineffective:") { out << endl; }
        out << line;
        
        //Adds an item to the set
        if (command == "Set:") {
            set.clear();
            while (iss >> type) {
                set.insert(type);
            }
            out << endl;
            out << "  [" << set << "]" << endl;
        }
        
        //Adds pokemon with corresponding move
        else if (command == "Pokemon:") {
            while (iss >> animal) {
                iss >> type;
                pokemon[animal] = type;
            }
        }
        
        //Adds move type to corresponding move
        else if (command == "Move:") {
            while (iss >> move) {
                iss >> type;
                moves[move] = type;
            }
        }
        
        //Adds move type with corresponding strengths
        else if (command == "Effective:") {
            iss >> type;
            while (iss >> effective) {
                effectives[type].insert(effective);
            }
        }
        
        //Adds move type with corresponding weaknesses
        else if (command == "Ineffective:") {
            iss >> type;
            while (iss >> ineffective) {
                ineffectives[type].insert(ineffective);
            }
        }
        
        //Outputs list of all pokemon stored
        else if (command == "Pokemon") {
            out << pokemon;
        }
        
        //Outputs list of all moves stored
        else if (command == "Moves") {
            out << moves;
        }
        
        //Outputs list of all move types with corresponding strengths
        else if (command == "Effectivities") {
            out << effectives;
        }
        
        //Output list of all move types with corresponding weaknesses
        else if (command == "Ineffectivities") {
            out << ineffectives;
        }
        
        //Output battle results between two pokemon
        else if (command == "Battle:") {
            iss >> pokemonA >> attackA >> pokemonB >> attackB;
            if (pokemon.count(pokemonA) == 0 || pokemon.count(pokemonB) == 0) continue;
            out << battle(pokemonA, attackA, pokemonB, attackB, pokemon, moves, effectives, ineffectives);
        }
        
        out << endl;
    }
    
    return 0;
}

//Determines the battle between two pokemon
string battle(string pokemonA, string attackA, string pokemonB, string attackB, HashMap<string,string>& pokemon, HashMap<string,string>& moves, HashMap<string,Set<string>>& effectives, HashMap<string,Set<string>>& ineffectives) {
    ostringstream os;
    
    int damageAToB = int(effectives[moves[attackA]].count(pokemon[pokemonB])) - int(ineffectives[moves[attackA]].count(pokemon[pokemonB]));
    int damageBToA = int(effectives[moves[attackB]].count(pokemon[pokemonA])) - int(ineffectives[moves[attackB]].count(pokemon[pokemonA]));
    
    os << endl;
    os << "  " << pokemonA << " (" << attackA << ") vs " << pokemonB << " (" << attackB << ")" << endl;
    os << "  " << pokemonA << " is a " << pokemon[pokemonA] << " type Pokemon." << endl;
    os << "  " << pokemonB << " is a " << pokemon[pokemonB] << " type Pokemon." << endl;
    os << "  " << attackA << " is a " << moves[attackA] << " type move." << endl;
    os << "  " << attackB << " is a " << moves[attackB] << " type move." << endl;
    
    if (moves[attackA] != "normal") {
        os << "  " << attackA << " is super effective against [" << effectives[moves[attackA]] << "] type Pokemon." << endl;
    }
    else {
        os << "  " << attackA << " is effective against Pokemon." << endl;
    }
    if (moves[attackA] != "normal") {
        os << "  " << attackA << " is ineffective against [" << ineffectives[moves[attackA]] << "] type Pokemon." << endl;
    }
    
    os << "  " << pokemonA << "'s " << attackA << " is ";
    if (damageAToB == -1) { os << "ineffective"; }
    else if (damageAToB == 0) { os << "effective"; }
    else if (damageAToB == 1) { os << "super effective"; }
    os << " against " << pokemonB << endl;
    
    if (moves[attackB] != "normal") {
    os << "  " << attackB << " is super effective against [" << effectives[moves[attackB]] << "] type Pokemon." << endl;
    }
    else {
        os << "  " << attackB << " is effective against Pokemon." << endl;
    }
    if (moves[attackB] != "normal") {
    os << "  " << attackB << " is ineffective against [" << ineffectives[moves[attackB]] << "] type Pokemon." << endl;
    }
    os << "  " << pokemonB << "'s " << attackB << " is ";
    if (damageBToA == -1) { os << "ineffective"; }
    else if (damageBToA == 0) { os << "effective"; }
    else if (damageBToA == 1) { os << "super effective"; }
    os << " against " << pokemonA << endl;
    
    if (damageAToB - damageBToA == 0) { os << "  The battle between " << pokemonA << " and " << pokemonB << " is a tie."; }
    else {
        os << "  In the battle between " << pokemonA << " and " << pokemonB << ", ";
        (damageAToB - damageBToA > 0) ? (os << pokemonA << " wins!") : (os << pokemonB << " wins!");
    }
    
    return os.str();
}
