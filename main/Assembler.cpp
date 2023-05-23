#include "Assembler.h"
#include "SymbolTable.h"
#include <bitset>
#include <string>

using namespace std;

/**
 * Assembler constructor
 */
Assembler::Assembler()
{
    // Your code here
    compdict.clear();
    destdict.clear();
    jumpdict.clear();

    compEntry(CONST_0, "0101010");
    compEntry(CONST_1, "0111111");
    compEntry(CONST_NEG_1, "0111010");
    compEntry(VAL_D, "0001100");
    compEntry(VAL_A, "0110000");
    compEntry(NOT_D, "0001101");
    compEntry(NOT_A, "0110001");
    compEntry(NEG_D, "0001111");
    compEntry(NEG_A, "0110011");
    compEntry(D_ADD_1, "0011111");
    // compEntry("1+D","0011111");
    compEntry(A_ADD_1, "0110111");
    // compEntry("1+A","0110111");
    compEntry(D_SUB_1, "0001110");
    compEntry(A_SUB_1, "0110010");
    compEntry(D_ADD_A, "0000010");
    // compEntry("A+D","0000010");
    compEntry(D_SUB_A, "0010011");
    compEntry(A_SUB_D, "0000111");
    compEntry(D_AND_A, "0000000");
    // compEntry("A&D","0000000");
    compEntry(D_OR_A, "0010101");
    // compEntry("A|D","0010101");

    compEntry(VAL_M, "1110000");
    compEntry(NOT_M, "1110001");
    compEntry(NEG_M, "1110011");
    compEntry(M_ADD_1, "1110111");
    // compEntry("1+M","0110111");
    compEntry(M_SUB_1, "1110010");
    compEntry(D_ADD_M, "1000010");
    // compEntry("M+D","0000010");
    compEntry(D_SUB_M, "1010011");
    compEntry(M_SUB_D, "1000111");
    compEntry(D_AND_M, "1000000");
    // compEntry("M&D","0000000");
    compEntry(D_OR_M, "1010101");
    // compEntry("M|D","0010101");

    destEntry(NULL_DEST, "000");
    destEntry(M, "001");
    destEntry(D, "010");
    destEntry(MD, "011");
    // destEntry("DM","011");
    destEntry(A, "100");
    destEntry(AM, "101");
    // destEntry("MA","101");
    destEntry(AD, "110");
    // destEntry("DA","110");
    destEntry(AMD, "111");
    // destEntry("ADM","111");
    // destEntry("MAD","111");
    // destEntry("MDA","111");
    // destEntry("DAM","111");
    // destEntry("DMA","111");
    // destEntry("null","000");
    // destEntry("null","000");

    jumpEntry(NULL_JUMP, "000");
    jumpEntry(JGT, "001");
    jumpEntry(JEQ, "010");
    jumpEntry(JGE, "011");
    jumpEntry(JLT, "100");
    jumpEntry(JNE, "101");
    jumpEntry(JLE, "110");
    jumpEntry(JMP, "111");
}

/**
 * Assembler destructor
 */
Assembler::~Assembler()
{
    // Your code here
    compdict.clear();
    destdict.clear();
    jumpdict.clear();
}

void Assembler::compEntry(InstructionComp key, string val)
{
    compdict.insert(pair<InstructionComp, string>(key, val));
}

void Assembler::destEntry(InstructionDest key, string val)
{
    destdict.insert(pair<InstructionDest, string>(key, val));
}

void Assembler::jumpEntry(InstructionJump key, string val)
{
    jumpdict.insert(pair<InstructionJump, string>(key, val));
}

/**
 * Assembler first pass; populates symbol table with label locations.
 * @param instructions An array of the assembly language instructions.
 * @param symbolTable The symbol table to populate.
 */
void Assembler::doFirstPass(SymbolTable *symbolTable, string instructions[], int numOfInst)
{
    // Your code here
    int flag = 0;
    for (int i = 0; i < numOfInst; i++)
    {
        if (parseInstructionType(instructions[i]) == L_INSTRUCTION)
        {
            flag++;
            if (i == 0)
            {
                symbolTable->addSymbol(instructions[i].substr(1, instructions[i].size() - 2), i);
            }
            else
            {
                // flag++;
                symbolTable->addSymbol(instructions[i].substr(1, instructions[i].size() - 2), i - flag + 1);
            }
        }
    }
}

/**
 * Translates a set of instructions to machine code.
 * @param instructions An array of the assembly language instructions to be converted to machine code.
 * @param symbolTable The symbol table to reference/update.
 * @return A string containing the generated machine code as lines of 16-bit binary instructions.
 */
string Assembler::doSecondPass(SymbolTable *symbolTable, string instructions[], int numOfInst)
{
    // Your code here
    string result = "";
    for (int i = 0; i < numOfInst; i++)
    {
        if (parseInstructionType(instructions[i]) == A_INSTRUCTION)
        {
            string a = translateSymbol(parseSymbol(instructions[i]), symbolTable) + "\n";
            result += a;
        }
        if (parseInstructionType(instructions[i]) == C_INSTRUCTION)
        {
            string c = "111" + translateComp(parseInstructionComp(instructions[i])) + translateDest(parseInstructionDest(instructions[i])) +
                       translateJump(parseInstructionJump(instructions[i])) + "\n";
            result += c;
        }
        if (parseInstructionType(instructions[i]) == L_INSTRUCTION || parseInstructionType(instructions[i]) == NULL_INSTRUCTION)
        {
            continue;
        }
    }
    return result;
}

/**
 * Parses the type of the provided instruction
 * @param instruction The assembly language representation of an instruction.
 * @return The type of the instruction (A_INSTRUCTION, C_INSTRUCTION, L_INSTRUCTION, NULL)
 */
Assembler::InstructionType Assembler::parseInstructionType(string instruction)
{
    // Your code here:
    if (instruction.find('@') != string::npos)
    {
        return A_INSTRUCTION;
    }
    if (instruction.find('(') != string::npos)
    {
        return L_INSTRUCTION;
    }
    if (instruction.find('@') == string::npos || instruction.find('(') == string::npos)
    {
        return C_INSTRUCTION;
    }
    return NULL_INSTRUCTION;
}

/**
 * Parses the destination of the provided C-instruction
 * @param instruction The assembly language representation of a C-instruction.
 * @return The destination of the instruction (A, D, M, AM, AD, MD, AMD, NULL)
 */
Assembler::InstructionDest Assembler::parseInstructionDest(string instruction)
{
    // Your code here:
    int pointer = -1;
    string dest;
    pointer = instruction.find("=");
    if (pointer == -1)
    {
        return NULL_DEST;
    }
    else
    {
        dest = instruction.substr(0, pointer);
        if (dest == "A")
        {
            return A;
        }
        if (dest == "D")
        {
            return D;
        }
        if (dest == "M")
        {
            return M;
        }
        if (dest == "AM")
        {
            return AM;
        }
        if (dest == "AD")
        {
            return AD;
        }
        if (dest == "MD")
        {
            return MD;
        }
        if (dest == "AMD")
        {
            return AMD;
        }
    }
    return NULL_DEST;
}
/**
 * Parses the jump condition of the provided C-instruction
 * @param instruction The assembly language representation of a C-instruction.
 * @return The jump condition for the instruction (JLT, JGT, JEQ, JLE, JGE, JNE, JMP, NULL)
 */
Assembler::InstructionJump Assembler::parseInstructionJump(string instruction)
{
    // Your code here:
    // for example if "JLT" appear at the comp field return enum label JLT
    if (instruction.find("JLT") != string::npos)
    {
        return JLT;
    }
    if (instruction.find("JGT") != string::npos)
    {
        return JGT;
    }
    if (instruction.find("JEQ") != string::npos)
    {
        return JEQ;
    }
    if (instruction.find("JLE") != string::npos)
    {
        return JLE;
    }
    if (instruction.find("JGE") != string::npos)
    {
        return JGE;
    }
    if (instruction.find("JNE") != string::npos)
    {
        return JNE;
    }
    if (instruction.find("JMP") != string::npos)
    {
        return JMP;
    }
    return NULL_JUMP;
}

/**
 * Parses the computation/op-code of the provided C-instruction
 * @param instruction The assembly language representation of a C-instruction.
 * @return The computation/op-code of the instruction (CONST_0, ... ,D_ADD_A , ... , NULL)
 */
Assembler::InstructionComp Assembler::parseInstructionComp(string instruction)
{
    // Your code here:
    if (instruction.find("0") != string::npos)
    {
        return CONST_0;
    }
    string comp;

    int spointer = instruction.find(";");
    int epointer = instruction.find("=");
    //both ; and =
    if (spointer != -1 && epointer != -1)
    {
        comp = instruction.substr(epointer + 1, spointer - 1 - epointer);
    }
    //no ；exist =
    else if (spointer == -1 && epointer != -1)
    {
        comp = instruction.substr(epointer + 1, instruction.size() - epointer);
    }
    //exist ；no =
    else if (spointer != -1 && epointer == -1)
    {
        comp = instruction.substr(0, spointer);
    }
    else if (spointer == -1 && epointer == -1)
    {
        comp = instruction;
    }

    if (comp == "1")
    {
        return CONST_1;
    }
    if (comp == "-1")
    {
        return CONST_NEG_1;
    }
    if (comp == "A")
    {
        return VAL_A;
    }
    if (comp == "M")
    {
        return VAL_M;
    }
    if (comp == "D")
    {
        return VAL_D;
    }
    if (comp == "!A")
    {
        return NOT_A;
    }
    if (comp == "!M")
    {
        return NOT_M;
    }
    if (comp == "!D")
    {
        return NOT_D;
    }
    if (comp == "-A")
    {
        return NEG_A;
    }
    if (comp == "-M")
    {
        return NEG_M;
    }
    if (comp == "-D")
    {
        return NEG_D;
    }
    if (comp == "A+1" || comp == "1+A")
    {
        return A_ADD_1;
    }
    if (comp == "M+1" || comp == "1+M")
    {
        return M_ADD_1;
    }
    if (comp == "D+1" || comp == "1+D")
    {
        return D_ADD_1;
    }
    if (comp == "A-1")
    {
        return A_SUB_1;
    }
    if (comp == "M-1")
    {
        return M_SUB_1;
    }
    if (comp == "D-1")
    {
        return D_SUB_1;
    }
    if (comp == "D+A" || comp == "A+D")
    {
        return D_ADD_A;
    }
    if (comp == "D+M" || comp == "M+D")
    {
        return D_ADD_M;
    }
    if (comp == "D-A")
    {
        return D_SUB_A;
    }
    if (comp == "D-M")
    {
        return D_SUB_M;
    }
    if (comp == "A-D")
    {
        return A_SUB_D;
    }
    if (comp == "M-D")
    {
        return M_SUB_D;
    }
    if (comp == "D&A" || comp == "DA")
    {
        return D_AND_A;
    }
    if (comp == "D&M" || comp == "DM")
    {
        return D_AND_M;
    }
    if (comp == "D|A")
    {
        return D_OR_A;
    }
    if (comp == "D|M")
    {
        return D_OR_M;
    }
    else
        return NULL_COMP;
}

/**
 * Parses the symbol of the provided A/L-instruction
 * @param instruction The assembly language representation of a A/L-instruction.
 * @return A string containing either a label name (L-instruction),
 *         a variable name (A-instruction), or a constant integer value (A-instruction)
 */
string Assembler::parseSymbol(string instruction)
{
    if (instruction.find('(') != string::npos)
    {
        return instruction.substr(1, instruction.size() - 2);
    }
    if (instruction.find('@') != string::npos)
    {
        return instruction.substr(1, instruction.size() - 1);
    }
    return "";
}

/**
 * Generates the binary bits of the dest part of a C-instruction
 * @param dest The destination of the instruction
 * @return A string containing the 3 binary dest bits that correspond to the given dest value.
 */
string Assembler::translateDest(InstructionDest dest)
{
    // Your code here:
    if (destdict.find(dest)->second != "000")
    {
        return destdict.find(dest)->second;
    }
    return "000";
}

/**
 * Generates the binary bits of the jump part of a C-instruction
 * @param jump The jump condition for the instruction
 * @return A string containing the 3 binary jump bits that correspond to the given jump value.
 */
string Assembler::translateJump(InstructionJump jump)
{
    // Your code here:
    if (jumpdict.find(jump)->second != "000")
    {
        return jumpdict.find(jump)->second;
    }
    return "000";
}

/**
 * Generates the binary bits of the computation/op-code part of a C-instruction
 * @param comp The computation/op-code for the instruction
 * @return A string containing the 7 binary computation/op-code bits that correspond to the given comp value.
 */
string Assembler::translateComp(InstructionComp comp)
{
    // Your code here:
    if (compdict.find(comp)->second != "0000000")
    {
        return compdict.find(comp)->second;
    }
    return "0000000";
}

/**
 * Generates the binary bits for an A-instruction, parsing the value, or looking up the symbol name.
 * @param symbol A string containing either a label name, a variable name, or a constant integer value
 * @param symbolTable The symbol table for looking up label/variable names
 * @return A string containing the 15 binary bits that correspond to the given sybmol.
 */
string Assembler::translateSymbol(string symbol, SymbolTable *symbolTable)
{
    // Your code here:
    // int i = 16;

    if (symbolTable->table.find(symbol) == symbolTable->table.end())
    {
        if (isdigit(symbol[0]))
        {
            symbolTable->addSymbol(symbol, stoi(symbol));
        }
        else
        {
            symbolTable->addSymbol(symbol, symbolTable->VariableCounter);
            symbolTable->VariableCounter++;
        }
    }
    if (isdigit(symbol[0]))
    {
        return "0" + bitset<15>(stoi(symbol)).to_string();
    }
    else
    {
        uint16_t addr = symbolTable->getSymbol(symbol);
        return "0" + bitset<15>(addr).to_string();
    }
    return "0000000000000000";
}
