# Language Translation: Backus-Naur Form (BNF) Evaluator
This project is an implementation of a BNF (Backus-Naur Form) expression evaluator designed to parse and evaluate mathematical expressions based on user input. The evaluator checks for syntactical correctness and performs calculations on valid expressions. Made by Sarah Akhtar with Kieran Monks.

## Purpose
The primary purpose of this program is to characterize BNF statements and evaluate mathematical expressions input by the user. It utilizes regular expressions to validate the syntax of numbers, variables, and operators.

## Features
- Continuous input loop for user interaction.
- Validation of mathematical expressions using regular expressions.
- Parsing of valid expressions into tokens.
- Evaluation of expressions with support for basic arithmetic operations: addition, subtraction, multiplication, and division.
- Error handling for invalid syntax and unsolvable expressions.

## Getting Started

### Prerequisites
To compile and run this program, ensure you have a C++ compiler installed (e.g., `g++`, `clang++`).

### Compilation Instructions
1. Download the source code file `language-translation.cpp`.
2. Compile the code using the following command:
   ```bash
   g++ -o bnf_evaluator language-translation.cpp
   ```
3. Run the compiled program:
   ```
   ./bnf_evaluator
   ```

## Usage Instructions
Upon running the program, you will be prompted to enter a statement. Enter a mathematical expression in the format: `variable = expression` or `expression = variable`. Press 'X' at any time to exit the program. The program will analyze the input and provide feedback on its validity or compute the result.

## Code Structure
The main components of the code include:
- **Initialization:** Sets up the language translation process.
- **Tokenization:** Separates input strings into tokens for processing.
- **Validation:** Uses regular expressions to validate numbers, variables, and operators.
- **Parsing and Solving:** Analyzes the tokens to determine if they form a valid expression and computes results where applicable.

## Key Functions
- `languageTranslationInitialization()`: Initializes the translation process.
- `languageTranslationTokenization()`: Tokenizes input statements into words.
- `languageTranslationScanner()`: Validates tokens against expected types (number, variable, operator).
- `languageTranslationParser()`: Parses tokens to check for syntax errors.
- `languageTranslationSolver()`: Evaluates valid expressions based on parsed tokens.

## Error Handling
The program includes error handling for:
- Invalid syntax (e.g., incorrect number of tokens).
- Division by zero errors.
- Unsolvable expressions due to multiple variables.

## Conclusion
This BNF evaluator serves as an educational tool for understanding parsing and evaluation of mathematical expressions using BNF notation. It demonstrates practical applications of regular expressions in syntax validation and expression evaluation in programming.

For any questions or contributions, please contact the authors via their respective university email addresses.
