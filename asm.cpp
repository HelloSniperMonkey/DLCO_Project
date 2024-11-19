//Soumyajyoti Mohnata
//Roll No. : 2301AI23
// 2 pass assembler
#include <bits/stdc++.h>
using namespace std;

unordered_map<string, pair<int, int>> instructions = {
    {"ldc", {0, 1}},
    {"adc", {1, 1}},
    {"ldl", {2, 1}},
    {"stl", {3, 1}},
    {"ldnl", {4, 1}},
    {"stnl", {5, 1}},
    {"add", {6, 0}},
    {"sub", {7, 0}},
    {"shl", {8, 0}},
    {"shr", {9, 0}},
    {"adj", {10, 1}},
    {"a2sp", {11, 0}},
    {"sp2a", {12, 0}},
    {"call", {13, 1}},
    {"return", {14, 0}},
    {"brz", {15, 1}},
    {"brlz", {16, 1}},
    {"br", {17, 1}},
    {"HALT", {18, 0}},
    {"data", {20, 1}},
    {"SET", {19, 1}}};

set<pair<int, string>> lablewithline;
set<string> lable;
vector<string> error;

string trimline(string code)
{
    // Check for empty string first
    if (code.empty())
    {
        return "";
    }

    // Find first non-whitespace character
    size_t start = code.find_first_not_of(" \t\n");
    if (start == string::npos)
    {
        return "";
    }

    // Find last non-whitespace character
    size_t end = code.find_last_not_of(" \t\n");
    if (end == string::npos)
    {
        return "";
    }

    return code.substr(start, end - start + 1);
}
string trim(string code)
{
    // Check for empty string first
    if (code.empty())
    {
        return "";
    }

    // Handle comments
    size_t pos = code.find(";");
    if (pos != string::npos)
    {
        code = code.substr(0, pos);
    }

    // Remove leading/trailing whitespace
    return trimline(code);
}

bool correctIdentifier(string lable)
{
    if (lable.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") != string::npos || (lable[0] >= '0' && lable[0] <= '9'))
    {
        error.push_back("Invalid Identifier: " + lable + "\n");
        return false;
    }
    return true;
}

bool checkLable(vector<string> code)
{
    int currentLine = 0;
    for (auto line : code)
    {
        line = line.substr(line.find_first_not_of(" \t\n"));       // trimming the line
        line = line.substr(0, line.find_last_not_of(" \t\n") + 1); // trimming the line

        size_t pos = line.find(":");
        // cout<<"\""<<line<<"\""<<" "<<currentLine<<endl;
        if (pos != string::npos)
        {
            string currentLable = line.substr(0, pos);
            if ((correctIdentifier(currentLable)))
            {
                if (lable.find(currentLable) != lable.end())
                {
                    error.push_back("Lable already defined: " + currentLable + "\n");
                    // cout << "Lable already defined: " << currentLable << endl;
                    // return false;
                }
                else
                {
                    lable.insert(currentLable);

                    lablewithline.insert({currentLine, currentLable});

                    if (line.find(" ") == string::npos)
                        currentLine--;
                }
                // cout << "Lable: " << currentLable << " at line : " << currentLine << endl;
            }
        }
        currentLine++;
    }
    if (error.size() > 0)
    {
        return false;
    }
    return true;
}

// Helper functions
bool isNumber(const string &str)
{
    if (str.empty())
        return false;

    size_t start = 0;
    if (str[0] == '+' || str[0] == '-')
        start = 1;

    // Hex number
    if (str.substr(start, 2) == "0x")
    {
        return str.substr(start + 2).find_first_not_of("0123456789abcdefABCDEF") == string::npos;
    }
    // Octal number
    if (str[start] == '0' && str.length() > start + 1)
    {
        return str.substr(start + 1).find_first_not_of("01234567") == string::npos;
    }
    // Decimal number
    return str.substr(start).find_first_not_of("0123456789") == string::npos;
}

bool isValidOperand(const string &instr, const string &operand, const set<string> &labels)
{
    // Handle instructions that require label operands
    if (instr == "br" || instr == "brz" || instr == "brlz" || instr == "call")
    {
        if (labels.find(operand) == labels.end())
        {
            return false; // Label not found
        }
        return true;
    }
    // Handle ldc which can take both numbers and labels
    else if (instr == "ldc")
    {
        return isNumber(operand) || labels.find(operand) != labels.end();
    }
    // All other instructions take only numeric operands
    return isNumber(operand);
}

bool checkoperand(vector<string> code)
{
    int currentLine = 0;
    for (auto line : code)
    {
        currentLine++;

        // Trim whitespace
        line = line.substr(line.find_first_not_of(" \t\n"));
        line = line.substr(0, line.find_last_not_of(" \t\n") + 1);

        // Handle labels
        size_t colonPos = line.find(":");
        if (colonPos != string::npos)
        {
            line = line.substr(colonPos + 1);
        }

        // Skip empty lines
        if (line.find_first_not_of(" \n\t") == string::npos)
        {
            continue;
        }

        // Get instruction and operand
        line = line.substr(line.find_first_not_of(" \n\t"));
        line = line.substr(0, line.find_last_not_of(" \n\t") + 1);

        string currentInstruction = line.substr(0, line.find(" "));
        string operand = "";
        if (line.find(" ") != string::npos)
        {
            operand = line.substr(line.find_first_not_of(" ", line.find(" ")));
        }

        // Check if instruction exists
        if (instructions.find(currentInstruction) == instructions.end())
        {
            error.push_back("Invalid Instruction: " + currentInstruction + " at line : " + to_string(currentLine) + "\n");
            continue;
        }

        // Check operand count and validity
        pair<int, int> instrInfo = instructions[currentInstruction];
        if (instrInfo.second == 0)
        {
            if (!operand.empty())
            {
                error.push_back("Instruction " + currentInstruction + " should not have operands at line : " + to_string(currentLine) + "\n");
            }
        }
        else
        {
            if (operand.empty())
            {
                error.push_back("Missing operand for instruction " + currentInstruction + " at line : " + to_string(currentLine) + "\n");
            }
            else if (!isValidOperand(currentInstruction, operand, lable))
            {
                if (currentInstruction == "br" || currentInstruction == "brz" ||
                    currentInstruction == "brlz" || currentInstruction == "call")
                {
                    error.push_back("Label " + operand + " not found for instruction " +
                                    currentInstruction + " at line : " + to_string(currentLine) + "\n");
                }
                else
                {
                    error.push_back("Invalid operand " + operand + " for instruction " +
                                    currentInstruction + " at line : " + to_string(currentLine) + "\n");
                }
            }
        }
    }
    return error.empty();
}

string makebinary(string code, int currentLine)
{
    //remove trailing and leading whitespaces
    code = code.substr(0, code.find_last_not_of("\n") + 1);
    code = code.substr(code.find_first_not_of(" \t"));
    code = code.substr(0, code.find_last_not_of(" \t") + 1);

    //check for empty lines 
    if (code.empty())
        return "";

    string instruction = code;
    string operand = "";
    size_t space_pos = code.find(" ");
    if (space_pos != string::npos)
    {
        instruction = code.substr(0, space_pos);
        operand = code.substr(space_pos + 1);
        operand = operand.substr(operand.find_first_not_of(" \t"));
        operand = operand.substr(0, operand.find_last_not_of(" \t") + 1);
    }

    //check for invalid instruction
    if (instructions.find(instruction) == instructions.end())
    {
        return "        ";
    }

    pair<int, int> instr_details = instructions[instruction];
    int opcode = instr_details.first;
    int needs_operand = instr_details.second;

    uint32_t binary_instruction = 0;
    binary_instruction = (opcode & 0xFF);

    if (opcode == 17 || opcode == 15 || opcode == 16)
    { // br, brz, brlz
        if (operand.empty())
            return "        ";

        try
        {
            int destinationLine;
            if (lable.find(operand) != lable.end())
            {
                for (const auto &label_pair : lablewithline)
                {
                    if (label_pair.second == operand)
                    {
                        destinationLine = label_pair.first;
                        break;
                    }
                }
            }
            else
            {
                destinationLine = stoi(operand);
            }

            int offset = destinationLine - 1 - currentLine;
            binary_instruction |= (offset & 0xFFFFFF) << 8;

            ostringstream oss;
            oss << setw(8) << setfill('0') << hex << binary_instruction;
            return oss.str();
        }
        catch (...)
        {
            return "        ";
        }
    }

    if (needs_operand)
    {
        if (operand.empty())
            return "        ";

        try//try catch block to parse machine code
        {
            int value;
            if (operand.substr(0, 2) == "0x")
            {
                value = stoi(operand.substr(2), nullptr, 16);//hexa not decimal
            }
            else if (lable.find(operand) != lable.end())
            {
                for (const auto &label_pair : lablewithline) //find the line number of the label and using & becasue set instr pushes extra line so we need to find the exact line number
                {
                    if (label_pair.second == operand)
                    {
                        value = label_pair.first;
                        break;
                    }
                }
            }
            else
            {
                value = stoi(operand);
            }

            binary_instruction |= (value & 0xFFFFFF) << 8;

            ostringstream oss;
            oss << setw(8) << setfill('0') << hex << binary_instruction;
            return oss.str();
        }
        catch (...)
        {
            return "        ";
        }
    }

    ostringstream oss;
    oss << setw(8) << setfill('0') << hex << binary_instruction;
    return oss.str();
}

void pushSETinstructions(vector<string> *code, string label, string instruction, int colonPos)
{
    // Following mnemonic are used in implementing SET mnemonic in assembler
    code->push_back("adj 10000");                                         // stack pointer at random memory
    code->push_back("stl -1");                                            // Here to load A
    code->push_back("stl 0");                                             // Here B
    code->push_back("ldc " + trimline(instruction.substr(colonPos + 6))); // Load value after SET
    code->push_back("ldc " + label);                                      // Load label
    code->push_back("stnl 0");                                            // Load A, B
    code->push_back("ldl 0");
    code->push_back("ldl -1");
    code->push_back("adj -10000"); // Adjust stack ptr
}

void implementSET(string line, vector<string> *code)
{
    // Remove leading/trailing whitespace
    line = trimline(line);

    // If no SET instruction, return original line
    if (line.find("SET") == string::npos)
    {
        code->push_back(line);
        return;
    }

    size_t colonPos = line.find(":");
    if (colonPos == string::npos)
    {
        code->push_back(line); 
        return;
    }

    string label = line.substr(0, colonPos);

    // Verify SET instruction follows the label
    string afterLabel = trimline(line.substr(colonPos + 1));
    if (afterLabel.substr(0, 3) != "SET")
    {
        code->push_back(line); // Not a SET instruction
        return;
    }

    // Push equivalent instructions for SET
    pushSETinstructions(code, label, line, colonPos);
}

void collectSETLabels(const vector<string> &cleancode)
{
    int current_line = 0;
    for (const auto &line : cleancode)
    {
        string trimmed = trimline(line);
        size_t colonPos = trimmed.find(":");
        if (colonPos != string::npos)
        {
            string label = trimmed.substr(0, colonPos);
            string rest = trimline(trimmed.substr(colonPos + 1));

            // If this is a SET instruction
            if (rest.substr(0, 3) == "SET")
            {
                if (correctIdentifier(label))
                {
                    if (lable.find(label) != lable.end())
                    {
                        error.push_back("Label already defined: " + label + "\n");
                    }
                    else
                    {
                        lable.insert(label);
                        // Store the actual address where the value will be stored
                        lablewithline.insert({current_line, label});
                    }
                }
            }
        }
        current_line++;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cerr << "Provide correct number of arguments " << endl;
        cerr << "Sample: " << argv[0] << " <source_file_name> <listing_file_name> <binary_file_name> <logfile_name>" << endl;
        return 1;
    }

    FILE *source = fopen(argv[1], "r");
    if (!source)
    {
        perror("Cannot open source file");
        return 1;
    }

    char line[1000]; // Fixed-size buffer for reading lines

    vector<string> cleancode;

    /* First pass */
    int current_address = 0;
    while (fgets(line, sizeof(line), source))
    {
        if (trim(line).empty())
            continue;
        if (trimline(line).empty())
            continue;
        // cout << trim(line) << endl;
        cleancode.push_back(trim(line));
    }
    collectSETLabels(cleancode);
    vector<string> code;
    for (auto line : cleancode)
    {
        if (line.find("SET") != string::npos)
        {
            implementSET(line, &code);
        }
        else
        {
            code.push_back(line);
        }
    }

    bool errorb = checkLable(code);
    bool operanderror = checkoperand(code);

    current_address = 0;

    if (errorb && operanderror)
    {
        /* Open output files */
        FILE *listing = fopen(argv[2], "w");
        FILE *binary = fopen(argv[3], "w");
        if (!listing || !binary)
        {
            perror("Cannot open output files");
            fclose(source);
            return 1;
        }
        FILE *log = fopen(argv[4], "a");
        if (log)
            fprintf(log, "%s file compiled \nlisting file saved in %s \nmachine code saved in %s \n",argv[1], argv[2], argv[3]);
            fprintf(log, "-------------------------------------------------------------\n");

        /* Second pass */
        vector<string> binarycode;
        int i = -1;
        for (auto line : code)
        {
            i++;
            ostringstream oss;
            oss << std::setw(8) << std::setfill('0') << hex<< i;
            string linenumber = oss.str();

            if (line.find(":") != string::npos)
            {
                string lab = line.substr(0, line.find(":"));
                string cod = line.substr(line.find(":") + 1);
                if (lable.find(lab) != lable.end())
                {
                    string whitespace = "        ";
                    lab += ":";
                    if (cod.find_first_not_of(" \n\t") == string::npos)
                    {
                        fprintf(listing, "%s %s %s\n", linenumber.c_str(), whitespace.c_str(), lab.c_str());
                        i--;
                        // cout<<'f'<<endl;
                    }
                    else
                    {
                        fprintf(listing, "%s %s %s\n", linenumber.c_str(), (makebinary(cod, i)).c_str(), line.c_str());
                        binarycode.push_back(makebinary(cod, i));
                    }
                }
            }
            else
            {
                fprintf(listing, "%s %s %s\n", linenumber.c_str(), (makebinary(line, i)).c_str(), line.c_str());
                binarycode.push_back(makebinary(line, i));
            }
        }
        for (auto line : binarycode)
        {
            if (!line.empty())
            {
                uint32_t binary_value = 0;
                string bin_str = line;

                if (!bin_str.empty())
                {
                    stringstream ss;
                    ss << hex << bin_str;
                    ss >> binary_value;

                    // Write the actual binary value, not the string representation
                    fwrite(&binary_value, sizeof(uint32_t), 1, binary);
                }
            }
        }

        fclose(source);
        fclose(listing);
        fclose(binary);
    }
    else
    {
        FILE *log = fopen(argv[4], "a");
        fprintf(log, "%s file coudn't be compiled because of errors\n",argv[1]);
        for (auto err : error)
        {
            fprintf(log, "%s", err.c_str());
            cerr << err;
        }
        fprintf(log, "-------------------------------------------------------------\n");
    }

    return 0;
}
