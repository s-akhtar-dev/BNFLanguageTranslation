/*
* COMP 141 Assignment 3 Submission
 * Professor: Dr. Cliburn
 * Teammates: Sarah Akhtar, Kieran Monks
 * Date: September 10th, 2024
 * Description:
 */

#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <regex>
using namespace std;

void languageTranslationInitialization();
vector<string> languageTranslationSeparator(const string& statement);
vector<string> languageTranslationScanner(const vector<string>& words);

void languageTranslationParser(const vector<string>& words, const vector<string>& tokens);
void languageTranslationSolvableParser(const vector<string>& words, const vector<string>& tokens, bool isReversed);

string calculateVariableAnswer(string operation, vector<int>& numbers);
string calculateSolvableAnswer(string operation, vector<int>& numbers);
string calculateUniqueInstance(string operation, vector<int>& numbers, bool isReversed);

bool isValidateRegexNumber(const string& integerStatement);
bool isValidateRegexVariable(const string& variableStatement);
bool isValidateRegexOperator(const string& operatorStatement);

string getOppositeOperation(string operation);

void languageTranslationInitialization() {
    string statement;
    cout << "Welcome! Press 'X' at any time to exit." << endl;
    cout << "Enter a Statement: ";
    getline(cin, statement);

    while (statement != "X" && statement != "x") {
        vector<string> words = languageTranslationSeparator(statement);
        vector<string> tokens = languageTranslationScanner(words);

        languageTranslationParser(words, tokens);
        cout << endl;

        cout << "Enter a Statement: ";
        getline(cin, statement);
    }
}

vector<string> languageTranslationSeparator(const string& statement) {
    string word;
    int spaceCount = 0;
    vector<string> words;

    for (char character : statement) {
        if (!isspace(character)) {
            word.push_back(character);
        } else {
            spaceCount++;
            words.push_back(word);
            word.clear();
        }
    }

    if (!word.empty()) { words.push_back(word); }
    if (spaceCount != 4) { words.push_back("invalid"); }
    return words;
}

vector<string> languageTranslationScanner(const vector<string>& words) {
    vector<string> tokens;

    for (const string& word : words) {
        if (isValidateRegexNumber(word)) {
            tokens.emplace_back("number");
        } else if (isValidateRegexVariable(word)) {
            tokens.emplace_back("variable");
        } else if (isValidateRegexOperator(word)) {
            tokens.emplace_back("operator");
        } else if (word == "=") {
            tokens.emplace_back("equal");
        } else {
            return {"invalid"};
        }
    }

    return tokens;
}

bool isValidateRegexNumber(const string& integerStatement) {
    regex validNumber("^[0-9]+$");
    return regex_match(integerStatement, validNumber);
}

bool isValidateRegexVariable(const string& variableStatement) {
    regex validVariable("^[_a-zA-Z][_a-zA-Z0-9]*$");
    return regex_match(variableStatement, validVariable);
}

bool isValidateRegexOperator(const string& operatorStatement) {
    regex validOperator("[+\\-*/]");
    return regex_match(operatorStatement, validOperator);
}

void languageTranslationParser(const vector<string>& words, const vector<string>& tokens) {
    if (tokens.size() != 5) {
        cout << "Syntax Error: Invalid regular expression" << endl;
        return;
    }

    if ((tokens[1] == "operator" && tokens[3] == "operator")) {
        cout << "Syntax Error: Expected an equal sign" << endl;
        return;
    }

    if ((tokens[1] == "equal" && tokens[3] == "equal")) {
        cout << "Syntax Error: Expected an operator" << endl;
        return;
    }

    bool isReversed = (tokens[1] == "equal" && tokens[3] == "operator");
    cout << "Syntactically Valid" << endl;
    languageTranslationSolvableParser(words, tokens, isReversed);
}

void languageTranslationSolvableParser(const vector<string>& words, const vector<string>& tokens, bool isReversed) {
    string operation = isReversed ? words[3] : words[1];

    //DONE
    if (tokens[0] == "number" && tokens[2] == "number" && tokens[4] == "number") {
        vector<int> numbers = {stoi(words[0]), stoi(words[2])};
        string result = calculateSolvableAnswer(operation, numbers);
        cout << (result == words[4] ? "True Statement" : "Invalid Statement") << endl;

    } else if (tokens[0] == "variable" && tokens[2] == "number" && tokens[4] == "number") {
        //Fox = 8 + 37 -> 45 //Fox + 8 = 37 -> 29
        //Fox = 37 + 8 -> 45 //Fox + 37 = 8 -> -29
        vector<int> numbers = {stoi(words[isReversed ? 2 : 4]), stoi(words[isReversed ? 4 : 2])};
        string alteredOperation = isReversed ? getOppositeOperation(operation) : operation;
        string result = calculateVariableAnswer(alteredOperation, numbers);
        cout << words[0] << " is " << result << endl;

    } else if (tokens[0] == "number" && tokens[2] == "variable" && tokens[4] == "number") {
        //10 = Fox + 60 -> -50 //10 + Fox = 60 -> 50
        //60 = Fox + 10 -> 50 //60 + Fox = 10 -> -50
        //15 - y = 7 -> 8     //100 / b = 4 -> 25
        vector<int> numbers = {stoi(words[0]), stoi(words[4])};
        string result = calculateUniqueInstance(operation, numbers, isReversed);
        cout << words[2] << " is " << result << endl;

    } else if (tokens[0] == "number" && tokens[2] == "number" && tokens[4] == "variable") {
        //10 = 60 + Fox -> -50 //10 + 60 = Fox -> 70
        //60 = 10 + Fox -> 50 //60 + 10 = Fox -> 70
        vector<int> numbers = {stoi(words[0]), stoi(words[2])};
        string alteredOperation = isReversed ? operation : getOppositeOperation(operation);
        string result = calculateVariableAnswer(alteredOperation, numbers);
        cout << words[4] << " is " << result << endl;

    } else {
        cout << "Unsolvable" << endl;
    }
}

string calculateVariableAnswer(string operation, vector<int>& numbers) {
    if (operation == "+") {
        return to_string(numbers[0] - numbers[1]);
    } else if (operation == "-") {
        return to_string(numbers[0] + numbers[1]);
    } else if (operation == "*") {
        if (numbers[1] == 0) return "Error: Division by zero";
        return to_string(static_cast<double>(numbers[0]) / numbers[1]);
    } else if (operation == "/") {
        if (numbers[0] == 0) return "Error: Division by zero";
        return to_string(numbers[0] * numbers[1]);
    }
    return "Error: Calculation Error";
}

string calculateUniqueInstance(string operation, vector<int>& numbers, bool isReversed) {
    if (operation == "+") {
        return to_string(!isReversed ? numbers[1] - numbers[0] : numbers[0] - numbers[1]);
    } else if (operation == "-") {
        return to_string(!isReversed ? numbers[0] - numbers[1] : numbers[0] + numbers[1]);
    } else if (operation == "*") {
        if (numbers[1] == 0) return "Error: Division by zero";
        return to_string(!isReversed ? static_cast<double>(numbers[1]) / numbers[0] : static_cast<double>(numbers[0]) / numbers[1]);
    } else if (operation == "/") {
        if (numbers[0] == 0) return "Error: Division by zero";
        return to_string(!isReversed ? static_cast<double>(numbers[0]) / numbers[1] : static_cast<double>(numbers[0]) * numbers[1]);
    }
    return "Error: Calculation Error";
}

string calculateSolvableAnswer(string operation, vector<int>& numbers) {
    if (operation == "+") {
        return to_string(numbers[0] + numbers[1]);
    } else if (operation == "-") {
        return to_string(numbers[0] - numbers[1]);
    } else if (operation == "*") {
        return to_string(numbers[0] * numbers[1]);
    } else if (operation == "/") {
        if (numbers[1] == 0) return "Error: Division by zero";
        return to_string(static_cast<double>(numbers[0]) / numbers[1]);
    }
    return "Error: Calculation Error";
}

string getOppositeOperation(string operation) {
    if (operation == "+") { return "-"; }
    else if (operation == "-") { return "+"; }
    else if (operation == "*") { return "/"; }
    else if (operation == "/") { return "*"; }
    else { return operation; }
}

int main() {
    languageTranslationInitialization();
    return 0;
}
