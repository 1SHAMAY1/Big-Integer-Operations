#pragma once

#include <string>
#include <vector>
#include <iostream>

class BigInteger {
public:
    BigInteger();                      // Default constructor
    BigInteger(const std::string& number);  // From string
    BigInteger(int number);           // From int

    // Arithmetic operators
    BigInteger operator+(const BigInteger& other) const;
    BigInteger operator-(const BigInteger& other) const;
    BigInteger operator*(const BigInteger& other) const;
    BigInteger operator/(const BigInteger& other) const;
    BigInteger operator%(const BigInteger& other) const;
    BigInteger operator-() const;     // Unary minus

    // Comparison operators
    bool operator==(const BigInteger& other) const;
    bool operator!=(const BigInteger& other) const;
    bool operator<(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const BigInteger& num);

private:
    std::vector<int> digits; // Least significant digit first
    bool isNegative;

    void removeLeadingZeros();
    static int compareAbs(const BigInteger& a, const BigInteger& b);
    static BigInteger addAbs(const BigInteger& a, const BigInteger& b);
    static BigInteger subtractAbs(const BigInteger& a, const BigInteger& b);
};
