#include "SymbolTable.h"

#include <string>

/**
 * Symbol Table constructor
 */
SymbolTable::SymbolTable() {
    // Your code here
    table.clear();
    table.insert(pair<string,uint16_t>("SP",0));
    table.insert(pair<string,uint16_t>("LCL",1));
    table.insert(pair<string,uint16_t>("ARG",2));
    table.insert(pair<string,uint16_t>("THIS",3));
    table.insert(pair<string,uint16_t>("THAT",4));
    table.insert(pair<string,uint16_t>("R0",0));
    table.insert(pair<string,uint16_t>("R1",1));
    table.insert(pair<string,uint16_t>("R2",2));
    table.insert(pair<string,uint16_t>("R3",3));
    table.insert(pair<string,uint16_t>("R4",4));
    table.insert(pair<string,uint16_t>("R5",5));
    table.insert(pair<string,uint16_t>("R6",6));
    table.insert(pair<string,uint16_t>("R7",7));
    table.insert(pair<string,uint16_t>("R8",8));
    table.insert(pair<string,uint16_t>("R9",9));
    table.insert(pair<string,uint16_t>("R10",10));
    table.insert(pair<string,uint16_t>("R11",11));
    table.insert(pair<string,uint16_t>("R12",12));
    table.insert(pair<string,uint16_t>("R13",13));
    table.insert(pair<string,uint16_t>("R14",14));
    table.insert(pair<string,uint16_t>("R15",15));
    table.insert(pair<string,uint16_t>("SCREEN",16384));
    table.insert(pair<string,uint16_t>("KBD",24576));

    VariableCounter = 16;
}

/**
 * Symbol Table destructor
 */
SymbolTable::~SymbolTable() {
    // Your code here
    table.clear();
}

/**
 * Adds a symbol to the symbol table
 * @param symbol The name of the symbol
 * @param value The address for the symbol
 */
void SymbolTable::addSymbol(string symbol, uint16_t value) {
    // Your code here
    table.insert(pair<string,uint16_t>(symbol,value));
}

/**
 * Gets a symbol from the symbol table
 * @param symbol The name of the symbol
 * @return The address for the symbol or -1 if the symbol isn't in the table
 */
int SymbolTable::getSymbol(string symbol) {
    // Your code here
    if(table.find(symbol)!=table.end()){
        return table.find(symbol)->second;
    }
    return -1;
}