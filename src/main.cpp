#include "BigInteger.h"
#include <iostream>
#include <regex>
#include <stdexcept>

bool isValidNumber(const std::string& input) {
    return std::regex_match(input, std::regex("-?[0-9]+"));
}


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

    std::regex plainNumber(R"(\s*-?\d+\s*)");
    if (std::regex_match(input, plainNumber)) {
        return std::regex_replace(input, std::regex(R"(\s+)"), "");
    }

    throw std::invalid_argument("Invalid input. Only digits, +/- and spacing like 'a - b' are allowed.");
}


int main() {
    std::string inputA, inputB;

    while (true) {
        std::cout << "Enter first large integer or expression (a): ";
        std::getline(std::cin, inputA);
        std::cout << "Enter second large integer or expression (b): ";
        std::getline(std::cin, inputB);

        try {
            std::string valueA = evaluateSimpleExpression(inputA);
            std::string valueB = evaluateSimpleExpression(inputB);

            BigInteger a(valueA);
            BigInteger b(valueB);

            std::cout << "\na + b = " << (a + b) << '\n';
            std::cout << "b - a = " << (b - a) << '\n';
            std::cout << "a * b = " << (a * b) << '\n';
            std::cout << "b / a = " << (b / a) << '\n';
            std::cout << "b % a = " << (b % a) << '\n';
        }
        catch (const std::exception& e) {
            std::cerr << "[ERROR] " << e.what() << "\n";
            std::cout << "Invalid input. Try again? (y/n): ";
            char ch;
            std::cin >> ch;
            std::cin.ignore();
            if (ch == 'y' || ch == 'Y') continue;
            else break;
        }

        std::cout << "\nTry again with new numbers? (y/n): ";
        char again;
        std::cin >> again;
        std::cin.ignore();
        if (again != 'y' && again != 'Y') break;
    }

    return 0;
}
