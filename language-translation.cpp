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

bool isValidateRegexNumber(const string& integerStatement);
bool isValidateRegexVariable(const string& variableStatement);
bool isValidateRegexOperator(const string& operatorStatement);

string getOppositeOperation(string operation);

void languageTranslationInitialization() {
    vector<string> testCases = {
        "60 + Fox = 10",
        // "x = 10 + 5",
        // "6 = Variable - 9",
        // "52 + 8 = 60",
        // "54 + 90 = 12",
        // "Fox + 8 = 37",
        // "x = 10 + 5",
        // "15 - y = 7",
        // "a * 3 = 21",
        // "100 / b = 4",
        // "2 + 2 = 5",
        // "10 - 5 = 5",
        // "3 * 4 = 12",
        // "20 / 5 = 4",
        // "variable1 + 10 = 30",
        // "50 - variable2 = 25",
        "6 * variable3 = 42",
        "100 / variable4 = 20",
        "7 + 3 = answer",
        "difference = 20 - 8",
        "product = 5 * 6",
        "72 / quotient = 9",
        "1 + 1 = one",
        "ten - 5 = five",
        "dozen * 2 = twenty_four",
        "hundred / century = 1",
        "x + y = z",
        "10 = 5 + 5",
        "15 = variable - 5",
        "8 = 2 * four",
        "25 = hundred / 4",
        "result = 17 - 9",
        "6 * multiplier = 36",
        "81 / divisor = 9",
        "2 + 2 = four",
        "twelve - x = 7",
        "3 * y = fifteen",
        "sixty / z = 6",
        "a + b = c + d",
        "10 - x = x",
        "2 * y = y + y",
        "100 / z = z",
        "1 + 1 = 3 - 1",
        "5 * 5 = 100 / 4",
        "18 / 3 = 3 * 2",
        "x + 1 = x - 1",
        "2 ^ 3 = 8"
    };

    string statement;
    cout << "Welcome! Press 'X' at any time to exit." << endl;
    cout << "Enter a Statement: ";
    getline(cin, statement);

    for (string testCase : testCases) {
        cout << testCase << endl;
        vector<string> words = languageTranslationSeparator(testCase);
        vector<string> tokens = languageTranslationScanner(words);
        languageTranslationParser(words, tokens);
        cout << endl;
    }

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
        } else if (!word.empty()) {
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
        string alteredOperation = (!isReversed && numbers[0] > numbers[1]) ? getOppositeOperation(operation) : operation;
        string result = calculateVariableAnswer(alteredOperation, numbers);
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
        return to_string(numbers[0] / numbers[1]);
    } else if (operation == "/") {
        if (numbers[0] == 0) return "Error: Division by zero";
        return to_string(numbers[0] * numbers[1]);
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

int main() {
    languageTranslationInitialization();
    return 0;
}
