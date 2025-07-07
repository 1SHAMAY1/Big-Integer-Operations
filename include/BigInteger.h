#pragma once

#include <string>
#include <vector>
#include <iostream>

class BigInteger {
public:
    BigInteger();
    BigInteger(const std::string& number);
    BigInteger(int number);

    // Math operations
    BigInteger factorial() const;
    std::string factorialProcessString() const;
    bool isPrime() const;
    BigInteger abs() const;
    BigInteger gcd(const BigInteger& other) const;

    // Arithmetic operators
    BigInteger operator+(const BigInteger& other) const;
    BigInteger operator-(const BigInteger& other) const;
    BigInteger operator*(const BigInteger& other) const;
    BigInteger operator/(const BigInteger& other) const;
    BigInteger operator%(const BigInteger& other) const;
    BigInteger operator-() const;

    // Comparison operators
    bool operator==(const BigInteger& other) const;
    bool operator!=(const BigInteger& other) const;
    bool operator<(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;

    // Output
    friend std::ostream& operator<<(std::ostream& os, const BigInteger& num);

private:
    static const long long BASE = 1000000000LL;
    static const int BASE_DIGITS = 9;

    std::vector<long long> blocks;
    bool isNegative;

    void removeLeadingZeros();
    static int compareAbs(const BigInteger& a, const BigInteger& b);
    static BigInteger addAbs(const BigInteger& a, const BigInteger& b);
    static BigInteger subtractAbs(const BigInteger& a, const BigInteger& b);

    static BigInteger gradeSchoolMultiply(const BigInteger& a, const BigInteger& b);
    static BigInteger karatsubaMultiply(const BigInteger& a, const BigInteger& b);
    static BigInteger multiplySwitchAlgorithm(const BigInteger& a, const BigInteger& b);

    BigInteger factorialRecursive(const BigInteger& a, const BigInteger& b, bool verbose) const;
};
