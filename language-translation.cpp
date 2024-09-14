/*
 * COMP 141 Assignment 3: BNF (Backus-Naur Form) Evaluator
 * Professor: Dr. Cliburn
 * Authors: Sarah Akhtar, Kieran Monks
 * Date: September 10th, 2024
 * Purpose: Characterizes BNF Statements
 */

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
using namespace std;

#pragma mark - Function Forward Declaration

/// Initializes Language Translation Process
void languageTranslationInitialization();

/// Tokenizes and Scans Client Statement for Syntactical Correctness
vector<string> languageTranslationTokenization(const string& statement);
vector<string> languageTranslationScanner(const vector<string>& words);

/// Determines Syntactical Correctness by Regular Expressions
bool isValidateRegexNumber(const string& integerStatement);
bool isValidateRegexVariable(const string& variableStatement);
bool isValidateRegexOperator(const string& operatorStatement);

/// Parses and Solves for Valid, Invalid, Solvable, and Unsolvable Statements
void languageTranslationParser(const vector<string>& words, const vector<string>& tokens);
void languageTranslationSolver(const vector<string>& words, const vector<string>& tokens, bool isReversed);

/// Calculates Solvable Variables and Switches Operations for Calculations
string calculateSolvableToken(const string& operation, const string& firstNumber, const string& secondNumber);
string getDifferentOperation(const string& operation);

#pragma mark - Program Initialization

int main() {
    languageTranslationInitialization();
    return 0;
}

#pragma mark - Language Translation Process Initialization

/**
 * @brief Creates loop for continuous language translation
 */
void languageTranslationInitialization() {
    string statement;
    cout << "Welcome! Press 'X' at any time to exit." << endl;
    cout << "Enter a Statement: ";
    getline(cin, statement);

    while (statement != "X" && statement != "x") {
        // Separates words by space, scans to tokens, and parses for semantics
        vector<string> words = languageTranslationTokenization(statement);
        vector<string> tokens = languageTranslationScanner(words);
        languageTranslationParser(words, tokens);
        cout << endl;

        cout << "Enter a Statement: ";
        getline(cin, statement);
    }
}

#pragma mark - Tokenization And Scanning Process

/**
 * @brief Prepares for tokenization by separating raw expressions from BNF
 * @param statement The client statement as a single string
 * @return A vector containing all the variable, number, and operation literals
 */
vector<string> languageTranslationTokenization(const string& statement) {
    string currentWord;
    vector<string> words;
    int whitespaceCount = 0;

    // Creates tokens for variables, numbers, and operations
    for (const char character : statement) {
        if (!isspace(character)) {
            currentWord.push_back(character);
        } else {
            whitespaceCount++;
            words.push_back(currentWord);
            currentWord.clear();
        }
    }

    // Adds last word and verifies legal expression spacing
    if (!currentWord.empty()) { words.push_back(currentWord); }
    if (whitespaceCount != 4) { words.emplace_back("invalid"); }
    return words;
}

/**
 * @brief Tokenizes numbers, variables, operators, and equal signs
 * @param words The separated expressions with original values
 * @return A vector containing tokens specified by type
 */
vector<string> languageTranslationScanner(const vector<string>& words) {
    vector<string> tokens;

    // Validates types number, variable, operator, and equal from original expressions
    for (const string& word : words) {
        if (isValidateRegexNumber(word)) { tokens.emplace_back("number");
        } else if (isValidateRegexVariable(word)) { tokens.emplace_back("variable");
        } else if (isValidateRegexOperator(word)) { tokens.emplace_back("operator");
        } else if (word == "=") { tokens.emplace_back("equal");
        } else { return {"invalid"};
        }
    }

    return tokens;
}

#pragma mark - Regular Expression Validation

/**
 * @brief Determines valid BNF-specified integer of [0-9]+
 * @param integerStatement A valid or invalid integer string
 * @return A boolean for whether the integer is valid for the regular expression
 */
bool isValidateRegexNumber(const string& integerStatement) {
    regex validNumber("^[0-9]+$");
    return regex_match(integerStatement, validNumber);
}

/**
 * @brief Determines valid BNF-specified variable of (_|[a-z]|[A-Z]) ([a-z]|[A-Z]|[0-9]|_)*
 * @param variableStatement A valid or invalid variable string
 * @return A boolean for whether the variable is valid for the regular expression
 */
bool isValidateRegexVariable(const string& variableStatement) {
    regex validVariable("^[_a-zA-Z][_a-zA-Z0-9]*$");
    return regex_match(variableStatement, validVariable);
}

/**
 * @brief Determines valid BNF-specified operator of (+|-|*|/)
 * @param operatorStatement A valid or invalid operator string
 * @return A boolean for whether the operator is valid for the regular expression
 */
bool isValidateRegexOperator(const string& operatorStatement) {
    regex validOperator("[+\\-*/]");
    return regex_match(operatorStatement, validOperator);
}

#pragma mark - Language Translation Parser And Solver

/**
 * @brief Evaluates semantics based on syntax and solves for values
 * @param words A vector representing the original string values
 * @param tokens A vector representing the token values
 */
void languageTranslationParser(const vector<string>& words, const vector<string>& tokens) {
    // Verifies legal expression size of five tokens excluding spaces
    if (tokens.size() != 5) {
        cout << "Syntax Error: Invalid regular expression" << endl;
        return;
    }

    // Outputs an error for invalid expression of two operators
    if ((tokens.at(1) == "operator" && tokens.at(3) == "operator")) {
        cout << "Syntax Error: Expected an equal sign" << endl;
        return;
    }

    // Outputs an error for invalid expression of two equal signs
    if ((tokens.at(1) == "equal" && tokens.at(3) == "equal")) {
        cout << "Syntax Error: Expected an operator" << endl;
        return;
    }

    // Determines if expression is reversed and continues to solver
    bool isReversed = (tokens.at(1) == "equal" && tokens.at(3) == "operator");
    cout << "Syntactically Valid" << endl;
    languageTranslationSolver(words, tokens, isReversed);
}

/**
 * @brief Determines one of the following:
 *        1) The value of a variable based on the expression structure,
 *        2) The validity of an expression if all values are numbers, or
 *        3) If the expression is unsolvable in the case of multiple variables
 * @param words A vector representing the original string values
 * @param tokens A vector representing the token values
 * @param isReversed A boolean representing if the expression is reversed (A = A op A)
 */
void languageTranslationSolver(const vector<string>& words, const vector<string>& tokens, bool isReversed) {
    // Separates the first, second, and third number from vector and determines operation
    string operation = ((isReversed) ? (words.at(3)) : (words.at(1)));
    const string& firstNumber = words.at(0);
    const string& secondNumber = words.at(2);
    const string& thirdNumber = words.at(4);

    // Categorizes common operation behaviors and creates solution placeholder
    bool plusOrMult = (operation == "+" || operation == "*");
    bool minusOrDiv = (operation == "-" || operation == "/");
    string solution;

    // Case #1 (Number-Number-Number): Calculates the number and validity based on position of equal sign
    if (tokens.at(0) == "number" && tokens.at(2) == "number" && tokens.at(4) == "number") {
        if (isReversed) {
            solution = calculateSolvableToken(operation, secondNumber, thirdNumber);
            cout << (stoi(solution) == stoi(firstNumber) ? "True Statement" : "Invalid Statement") << endl;
        } else {
            solution = calculateSolvableToken(operation, firstNumber, secondNumber);
            cout << (stoi(solution) == stoi(thirdNumber) ? "True Statement" : "Invalid Statement") << endl;
        }

    // Case #2 (Variable-Number-Number): Calculates the value from reversed boolean and type of operation
    } else if (tokens.at(0) == "variable" && tokens.at(2) == "number" && tokens.at(4) == "number") {
        if (!isReversed && plusOrMult) {
            solution = calculateSolvableToken(getDifferentOperation(operation), thirdNumber, secondNumber);
        } else if (!isReversed && minusOrDiv) {
            solution = calculateSolvableToken(getDifferentOperation(operation), secondNumber, thirdNumber);
        } else if (isReversed && plusOrMult) {
            solution = calculateSolvableToken(operation, thirdNumber, secondNumber);
        } else {
            solution = calculateSolvableToken(operation, secondNumber, thirdNumber);
        }
        cout << firstNumber << " is " << solution << endl;

    // Case #3 (Number-Variable-Number): Calculates the value from reversed boolean and type of operation
    } else if (tokens.at(0) == "number" && tokens.at(2) == "variable" && tokens.at(4) == "number") {
        if ((!isReversed && plusOrMult) || (isReversed && minusOrDiv)) {
            solution = calculateSolvableToken(getDifferentOperation(operation), thirdNumber, firstNumber);
        } else if (isReversed && plusOrMult) {
            solution = calculateSolvableToken(getDifferentOperation(operation), firstNumber, thirdNumber);
        } else {
            solution = calculateSolvableToken(operation, firstNumber, thirdNumber);
        }
        cout << secondNumber << " is " << solution << endl;

    // Case #4 (Number-Number-Variable): Calculates the value from reversed boolean and type of operation
    } else if (tokens.at(0) == "number" && tokens.at(2) == "number" && tokens.at(4) == "variable") {
        if (!isReversed && (plusOrMult || minusOrDiv)) {
            solution = calculateSolvableToken(operation, firstNumber, secondNumber);
        } else if (isReversed && minusOrDiv) {
            solution = calculateSolvableToken(operation, secondNumber, firstNumber);
        } else {
            solution = calculateSolvableToken(getDifferentOperation(operation), firstNumber, secondNumber);
        }
        cout << thirdNumber << " is " << solution << endl;

    // Case #5 (Unsolvable): If all other cases are not applicable, the answer is unsolvable
    } else {
        cout << "Unsolvable" << endl;
    }
}

#pragma mark - Calculating Solvable Tokens And Manipulating Operations

/**
 * @brief Calculates the string output number from the specified operation
 * @param operation A string representing the operation to be performed
 * @param firstNumber A string representing the first number in operation
 * @param secondNumber A string representing the second number in operation
 * @return A string integer as the desired operation calculated from two numbers
 */
string calculateSolvableToken(const string& operation, const string& firstNumber, const string& secondNumber) {
    // Performs the desired operation based on the string operation provided
    if (operation == "+") { return to_string(stoi(firstNumber) + stoi(secondNumber)); }
    if (operation == "-") { return to_string(stoi(firstNumber) - stoi(secondNumber)); }
    if (operation == "*") { return to_string(stoi(firstNumber) * stoi(secondNumber)); }
    if (operation == "/" && secondNumber == "0") { return "ERROR_CANNOT_DIVIDE_BY_ZERO"; }
    if (operation == "/") { return to_string(stod(firstNumber) / stod(secondNumber)); }
    return firstNumber;
}

/**
 * @brief Switches the current operation to operator of the opposite type as described below:
 *        Type #1: Addition->Subtraction, Subtraction->Addition,
 *        Type #2: Multiplication->Division, Division->Multiplication
 * @param operation A vector representing the original string values
 * @return A string operator representing the opposite of the original operator
 */
string getDifferentOperation(const string& operation) {
    // Creates a map of operators to opposite operators and returns desired switch
    static const unordered_map<string, string> opMap = {{"+", "-"}, {"-", "+"}, {"*", "/"}, {"/", "*"}};
    return opMap.count(operation) ? opMap.at(operation) : operation;
}