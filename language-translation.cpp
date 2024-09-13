/*
 * COMP 141 Assignment 3 Submission: BNF (Backus-Naur Form) Evaluator
 * Professor: Dr. Cliburn
 * Authors: Sarah Akhtar, Kieran Monks
 * Date: September 10th, 2024
 * Purpose: Characterizes BNF Statements
 */

#include <iostream>
#include <string>
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
void languageTranslationExampleCases();

int main() {
    languageTranslationInitialization();
    return 0;
}

void languageTranslationInitialization() {
    //Can use this to test example cases
    languageTranslationExampleCases();

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

    if (tokens[0] == "number" && tokens[2] == "number" && tokens[4] == "number") {
        vector<int> numbers = {stoi(words[isReversed ? 2 : 0]), stoi(words[isReversed ? 4 : 2])};
        string result = calculateSolvableAnswer(operation, numbers);
        cout << (result == words[isReversed ? 0 : 4] ? "True Statement" : "Invalid Statement") << endl;

    } else if (tokens[0] == "variable" && tokens[2] == "number" && tokens[4] == "number") {
        vector<int> numbers = {stoi(words[isReversed ? 2 : 4]), stoi(words[isReversed ? 4 : 2])};
        string alteredOperation = isReversed ? getOppositeOperation(operation) : operation;
        string result = calculateVariableAnswer(alteredOperation, numbers);
        cout << words[0] << " is " << result << endl;

    } else if (tokens[0] == "number" && tokens[2] == "variable" && tokens[4] == "number") {
        vector<int> numbers = {stoi(words[0]), stoi(words[4])};
        string result = calculateUniqueInstance(operation, numbers, isReversed);
        cout << words[2] << " is " << result << endl;

    } else if (tokens[0] == "number" && tokens[2] == "number" && tokens[4] == "variable") {
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
        return to_string(numbers[0] / numbers[1]);
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

void languageTranslationExampleCases() {
    vector<string> testCases = {
        // Example Case #1: Num op Var eq Num OR Num eq Var op Num
        "60 = Fox + 10", "10 = Fox + 60", "60 = Fox - 10", "10 = Fox - 60",
        "60 = Fox * 10", "10 = Fox * 60", "60 = Fox / 10", "10 = Fox / 60",
        "60 + Fox = 10", "10 + Fox = 60", "60 - Fox = 10", "10 - Fox = 60",
        "60 * Fox = 10", "10 * Fox = 60", "60 / Fox = 10", "10 / Fox = 60",
        "-------------",

        // Example Case #2: Var op Num eq Num OR Var eq Num op Num
        "Fox = 60 + 10", "Fox = 10 + 60", "Fox = 60 - 10", "Fox = 10 - 60",
        "Fox = 60 * 10", "Fox = 10 * 60", "Fox = 60 / 10", "Fox = 10 / 60",
        "Fox + 60 = 10", "Fox + 10 = 60", "Fox - 60 = 10", "Fox - 10 = 60",
        "Fox * 60 = 10", "Fox * 10 = 60", "Fox / 60 = 10", "Fox / 10 = 60",
        "-------------",

        // Example Case #3: Num op Num eq Var OR Num eq Num op Var
        "10 = 60 + Fox", "60 = 10 + Fox", "10 = 60 - Fox", "60 = 10 - Fox",
        "10 = 60 * Fox", "60 = 10 * Fox", "10 = 60 / Fox", "60 = 10 / Fox",
        "10 + 60 = Fox", "60 + 10 = Fox", "10 - 60 = Fox", "60 - 10 = Fox",
        "10 * 60 = Fox", "60 * 10 = Fox", "10 / 60 = Fox", "60 / 10 = Fox",
        "-------------",

        // Example Case #4: Num op Num eq Num OR Num eq Num op Num
        "90 = 10 + 10", "90 = 80 + 10", "90 = 10 - 2", "90 = 100 - 10",
        "90 * 10 = 10", "90 * 10 = 900", "90 / 10 = 20", "90 / 10 = 9"

        // Example Case #5: Invalid Cases (Operators, Equal, Spaces)
        "10 = 60 = Fox", "Fox = 10 = 60", "Fox = 60 = Fox", "Fox = 10 = Fox",
        "10 + 60 + Fox", "Fox - 10 - 60", "Fox * 60 * Fox", "Fox / 10 / Fox",
        "10   60   Fox", "Fox   10   60", "Fox   60   Fox", "Fox   10   Fox",
        "10 * 6  = Fox", "60 * 1  =  Fox", "10 /  0 = Fox", " 0  / 10 = Fox",
        "-------------",

        // Example Case #6: Syntactically Valid, Unsolvable Cases
        "Fox = Fox + 10", "Fox = Fox - 60", "Fox = Fox * 10", "Fox = Fox / 60",
        "10 + Fox = Fox", "60 - Fox = Fox", "10 * Fox = Fox", "60 / Fox = Fox",
        "Fox = Fox + Fox", "Fox = Fox - Fox", "Fox = Fox * Fox", "Fox = Fox / Fox",
        "Fox + Fox = Fox", "Fox + Fox = Fox", "Fox - Fox = Fox", "Fox - Fox = Fox",
        "-------------",

        // Example Case #7: Testing Variable Regular Expressions
        "_af_ = 60 + 10", "40 = kd90s - 10", "100 = k930 * 10", "10 = 9k / 1",
        "0af + 10 = 40", "90 - 9gj = 10", "10 * _afe = 20", "__ / 90 = 9",
    };

    for (string testCase : testCases) {
        cout << testCase << endl;
        vector<string> words = languageTranslationSeparator(testCase);
        vector<string> tokens = languageTranslationScanner(words);
        languageTranslationParser(words, tokens);
        cout << endl;
    }
}
