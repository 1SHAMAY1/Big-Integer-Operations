#include "BigInteger.h"
#include <iostream>
#include <regex>
#include <stdexcept>
#include <algorithm>
#include <cctype>

// Trim whitespace
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// Valid integer input (with optional `+` or `-`)
bool isValidNumber(const std::string& input) {
    return std::regex_match(trim(input), std::regex(R"(-?\d+)"));
}

// Evaluates expressions like "123 + 456" or "789 - 100"
std::string evaluateSimpleExpression(const std::string& input) {
    std::smatch match;
    std::regex expr(R"(\s*(-?\d+)\s*([\+\-])\s*(-?\d+)\s*)");

    if (std::regex_match(input, match, expr)) {
        long long a = std::stoll(match[1]);
        std::string op = match[2];
        long long b = std::stoll(match[3]);

        long long result = (op == "+") ? (a + b) : (a - b);
        return std::to_string(result);
    }

    std::regex plain(R"(\s*-?\d+\s*)");
    if (std::regex_match(input, plain)) {
        return trim(input);
    }

    throw std::invalid_argument("Invalid input. Use plain numbers or expressions like '123 + 456'");
}

// Perform and print all operations
void printOperations(const BigInteger& a, const BigInteger& b) {
    std::cout << "\n===== Big Integer Arithmetic =====\n";
    std::cout << "a + b = " << (a + b) << "\n";
    std::cout << "b - a = " << (b - a) << "\n";
    std::cout << "a * b = " << (a * b) << "\n";

    try {
        std::cout << "b / a = " << (b / a) << "\n";
        std::cout << "b % a = " << (b % a) << "\n";
    } catch (const std::exception& e) {
        std::cout << "[ERROR] Division or Modulo: " << e.what() << "\n";
    }

    std::cout << "\n===== Extra Operations =====\n";

    try {
        std::cout << "Is a prime? " << (a.isPrime() ? "Yes" : "No") << "\n";
    } catch (const std::exception& e) {
        std::cout << "[ERROR] a.isPrime(): " << e.what() << "\n";
    }

    try {
        std::cout << "Is b prime? " << (b.isPrime() ? "Yes" : "No") << "\n";
    } catch (const std::exception& e) {
        std::cout << "[ERROR] b.isPrime(): " << e.what() << "\n";
    }

    try {
        std::cout << "GCD(a, b): " << a.gcd(b) << "\n";
    } catch (const std::exception& e) {
        std::cout << "[ERROR] GCD: " << e.what() << "\n";
    }

    try {
        std::cout << "Factorial of a: " << a.factorial() << "\n";
    } catch (const std::exception& e) {
        std::cout << "[ERROR] a.factorial(): " << e.what() << "\n";
    }

    try {
        std::cout << "Factorial of b: " << b.factorial() << "\n";
    } catch (const std::exception& e) {
        std::cout << "[ERROR] b.factorial(): " << e.what() << "\n";
    }
}

int main() {
    std::string inputA, inputB;

    std::cout << "======= Big Integer Calculator =======\n";

    while (true) {
        std::cout << "\nEnter first large integer or expression (a): ";
        std::getline(std::cin, inputA);

        std::cout << "Enter second large integer or expression (b): ";
        std::getline(std::cin, inputB);

        try {
            std::string valueA = evaluateSimpleExpression(inputA);
            std::string valueB = evaluateSimpleExpression(inputB);

            BigInteger a(valueA);
            BigInteger b(valueB);

            printOperations(a, b);
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] " << e.what() << "\n";
            std::cout << "Try again? (y/n): ";
            char retry;
            std::cin >> retry;
            std::cin.ignore();
            if (retry == 'y' || retry == 'Y') continue;
            else break;
        }

        std::cout << "\nTry again with new numbers? (y/n): ";
        char again;
        std::cin >> again;
        std::cin.ignore();
        if (again != 'y' && again != 'Y') break;
    }

    std::cout << "Goodbye!\n";
    return 0;
}
