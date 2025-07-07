#include "BigInteger.h"
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <cctype>
#include <cmath>
#include <sstream>
#include <future>

const long long BigInteger::BASE;
const int BigInteger::BASE_DIGITS;

BigInteger::BigInteger() : isNegative(false) {}

BigInteger::BigInteger(const std::string& number) {
    isNegative = !number.empty() && number[0] == '-';
    std::string num = isNegative ? number.substr(1) : number;

    blocks.clear();
    for (int i = (int)num.length(); i > 0; i -= BASE_DIGITS) {
        int start = std::max(0, i - BASE_DIGITS);
        int len = i - start;
        blocks.push_back(std::stoll(num.substr(start, len)));
    }

    removeLeadingZeros();
}

BigInteger::BigInteger(int number) : BigInteger(std::to_string(number)) {}

void BigInteger::removeLeadingZeros() {
    while (blocks.size() > 1 && blocks.back() == 0)
        blocks.pop_back();
    if (blocks.size() == 1 && blocks[0] == 0)
        isNegative = false;
}

// Recursive helper: factorial(n) = fact(a, b)
BigInteger BigInteger::factorialRecursive(const BigInteger& a, const BigInteger& b, bool verbose) {
    if (a > b) return BigInteger(1);
    if (a == b) {
        if (verbose && compareAbs(b, BigInteger(5000)) > 0)
            std::cout << "Multiplying: " << a << "\n";
        return a;
    }
    if (b == a + BigInteger(1)) {
        if (verbose && compareAbs(b, BigInteger(5000)) > 0)
            std::cout << "Multiplying: " << a << " * " << b << "\n";
        return a * b;
    }

    // Use multithreading for large ranges
    BigInteger threshold(128);
    if (b - a > threshold) {
        BigInteger mid = (a + b) / BigInteger(2);
        auto left_future = std::async(std::launch::async, factorialRecursive, a, mid, verbose);
        BigInteger right = factorialRecursive(mid + BigInteger(1), b, verbose);
        BigInteger left = left_future.get();
        return left * right;
    } else {
        BigInteger mid = (a + b) / BigInteger(2);
        BigInteger left = factorialRecursive(a, mid, verbose);
        BigInteger right = factorialRecursive(mid + BigInteger(1), b, verbose);
        return left * right;
    }
}

// Fast public factorial
BigInteger BigInteger::factorial() const {
    if (isNegative)
        throw std::invalid_argument("Factorial of negative number is undefined.");
    if (*this == BigInteger(0) || *this == BigInteger(1))
        return BigInteger(1);
    return factorialRecursive(BigInteger(1), *this, true);
}


// GCD
BigInteger BigInteger::gcd(const BigInteger& other) const {
    BigInteger a = this->abs();
    BigInteger b = other.abs();
    while (!(b == BigInteger(0))) {
        BigInteger temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Absolute value
BigInteger BigInteger::abs() const {
    BigInteger result = *this;
    result.isNegative = false;
    return result;
}

// Prime check (naive)
bool BigInteger::isPrime() const {
    if (*this <= BigInteger(1)) return false;
    if (*this == BigInteger(2)) return true;
    if ((*this % BigInteger(2)) == BigInteger(0)) return false;

    BigInteger i = BigInteger(3);
    BigInteger limit = *this / BigInteger(2);

    while (i <= limit) {
        if (*this % i == BigInteger(0))
            return false;
        i = i + BigInteger(2);
    }

    return true;
}

// Comparison
int BigInteger::compareAbs(const BigInteger& a, const BigInteger& b) {
    if (a.blocks.size() != b.blocks.size())
        return a.blocks.size() < b.blocks.size() ? -1 : 1;

    for (int i = (int)a.blocks.size() - 1; i >= 0; --i) {
        if (a.blocks[i] != b.blocks[i])
            return a.blocks[i] < b.blocks[i] ? -1 : 1;
    }

    return 0;
}

// Addition
BigInteger BigInteger::addAbs(const BigInteger& a, const BigInteger& b) {
    BigInteger result;
    long long carry = 0;
    size_t n = std::max(a.blocks.size(), b.blocks.size());
    for (size_t i = 0; i < n || carry; ++i) {
        long long sum = carry;
        if (i < a.blocks.size()) sum += a.blocks[i];
        if (i < b.blocks.size()) sum += b.blocks[i];
        result.blocks.push_back(sum % BASE);
        carry = sum / BASE;
    }
    return result;
}

// Subtraction (|a| >= |b|)
BigInteger BigInteger::subtractAbs(const BigInteger& a, const BigInteger& b) {
    BigInteger result;
    long long borrow = 0;
    for (size_t i = 0; i < a.blocks.size(); ++i) {
        long long sub = a.blocks[i] - borrow - (i < b.blocks.size() ? b.blocks[i] : 0);
        if (sub < 0) {
            sub += BASE;
            borrow = 1;
        } else borrow = 0;
        result.blocks.push_back(sub);
    }
    result.removeLeadingZeros();
    return result;
}

// Addition operator
BigInteger BigInteger::operator+(const BigInteger& other) const {
    if (isNegative == other.isNegative) {
        BigInteger result = addAbs(*this, other);
        result.isNegative = isNegative;
        return result;
    } else {
        if (compareAbs(*this, other) >= 0) {
            BigInteger result = subtractAbs(*this, other);
            result.isNegative = isNegative;
            return result;
        } else {
            BigInteger result = subtractAbs(other, *this);
            result.isNegative = other.isNegative;
            return result;
        }
    }
}

// Subtraction operator
BigInteger BigInteger::operator-(const BigInteger& other) const {
    return *this + (-other);
}

// Unary minus
BigInteger BigInteger::operator-() const {
    BigInteger result = *this;
    if (!(blocks.size() == 1 && blocks[0] == 0))
        result.isNegative = !isNegative;
    return result;
}

// Multiplication algorithms
BigInteger BigInteger::gradeSchoolMultiply(const BigInteger& a, const BigInteger& b) {
    BigInteger result;
    result.blocks.assign(a.blocks.size() + b.blocks.size(), 0);
    for (size_t i = 0; i < a.blocks.size(); ++i) {
        long long carry = 0;
        for (size_t j = 0; j < b.blocks.size() || carry; ++j) {
            long long cur = result.blocks[i + j] + a.blocks[i] * 1LL * (j < b.blocks.size() ? b.blocks[j] : 0) + carry;
            result.blocks[i + j] = static_cast<long long>(cur % BASE);
            carry = static_cast<long long>(cur / BASE);
        }
    }
    result.removeLeadingZeros();
    return result;
}

BigInteger BigInteger::karatsubaMultiply(const BigInteger& a, const BigInteger& b) {
    // Base case: use grade-school for small numbers
    size_t n = std::max(a.blocks.size(), b.blocks.size());
    if (n <= 32) return gradeSchoolMultiply(a, b);

    size_t m = n / 2;
    // Split a and b
    BigInteger aLow, aHigh, bLow, bHigh;
    aLow.blocks.assign(a.blocks.begin(), a.blocks.begin() + std::min(m, a.blocks.size()));
    aHigh.blocks.assign(a.blocks.begin() + std::min(m, a.blocks.size()), a.blocks.end());
    bLow.blocks.assign(b.blocks.begin(), b.blocks.begin() + std::min(m, b.blocks.size()));
    bHigh.blocks.assign(b.blocks.begin() + std::min(m, b.blocks.size()), b.blocks.end());
    aLow.removeLeadingZeros(); aHigh.removeLeadingZeros();
    bLow.removeLeadingZeros(); bHigh.removeLeadingZeros();

    BigInteger z0 = karatsubaMultiply(aLow, bLow);
    BigInteger z2 = karatsubaMultiply(aHigh, bHigh);
    BigInteger aLowHigh = aLow + aHigh;
    BigInteger bLowHigh = bLow + bHigh;
    BigInteger z1 = karatsubaMultiply(aLowHigh, bLowHigh) - z0 - z2;

    // Shift z2 by 2*m, z1 by m
    z2.blocks.insert(z2.blocks.begin(), 2 * m, 0);
    z1.blocks.insert(z1.blocks.begin(), m, 0);

    BigInteger result = z2 + z1 + z0;
    result.removeLeadingZeros();
    return result;
}

BigInteger BigInteger::multiplySwitchAlgorithm(const BigInteger& a, const BigInteger& b) {
    size_t n = std::max(a.blocks.size(), b.blocks.size());
    if (n <= 32) {
        return gradeSchoolMultiply(a, b);
    } else {
        return karatsubaMultiply(a, b);
    }
}

BigInteger BigInteger::operator*(const BigInteger& other) const {
    BigInteger result = multiplySwitchAlgorithm(*this, other);
    result.isNegative = isNegative != other.isNegative;
    return result;
}

// Division
BigInteger BigInteger::operator/(const BigInteger& other) const {
    if (other == BigInteger(0))
        throw std::runtime_error("Division by zero");

    BigInteger dividend = this->abs();
    BigInteger divisor = other.abs();
    BigInteger result, current;
    result.blocks.resize(dividend.blocks.size());

    for (int i = (int)dividend.blocks.size() - 1; i >= 0; --i) {
        current.blocks.insert(current.blocks.begin(), dividend.blocks[i]);
        current.removeLeadingZeros();

        long long x = 0, l = 0, r = BASE - 1;
        while (l <= r) {
            long long m = (l + r) / 2;
            BigInteger t = divisor * BigInteger((int)m);
            if (t <= current) {
                x = m;
                l = m + 1;
            } else r = m - 1;
        }

        result.blocks[i] = x;
        current = current - divisor * BigInteger((int)x);
    }

    result.removeLeadingZeros();
    result.isNegative = isNegative != other.isNegative;
    return result;
}

// Modulo
BigInteger BigInteger::operator%(const BigInteger& other) const {
    return *this - (*this / other) * other;
}

// Comparisons
bool BigInteger::operator==(const BigInteger& other) const {
    return isNegative == other.isNegative && blocks == other.blocks;
}

bool BigInteger::operator!=(const BigInteger& other) const {
    return !(*this == other);
}

bool BigInteger::operator<(const BigInteger& other) const {
    if (isNegative != other.isNegative)
        return isNegative;
    int cmp = compareAbs(*this, other);
    return isNegative ? cmp > 0 : cmp < 0;
}

bool BigInteger::operator>(const BigInteger& other) const {
    return other < *this;
}

bool BigInteger::operator<=(const BigInteger& other) const {
    return !(*this > other);
}

bool BigInteger::operator>=(const BigInteger& other) const {
    return !(*this < other);
}

// Output
std::ostream& operator<<(std::ostream& os, const BigInteger& num) {
    if (num.blocks.empty()) return os << "0";
    if (num.isNegative) os << '-';

    os << num.blocks.back();
    for (int i = (int)num.blocks.size() - 2; i >= 0; --i)
        os << std::setw(BigInteger::BASE_DIGITS) << std::setfill('0') << num.blocks[i];

    return os;
}

std::string BigInteger::factorialProcessString() const {
    if (isNegative)
        return "Factorial of negative number is undefined.";
    BigInteger twenty(20);
    if (*this > twenty) {
        std::ostringstream oss;
        oss << "Factorial of " << *this << ": Too large to display process.";
        return oss.str();
    }
    std::ostringstream oss;
    oss << *this << "! = ";
    BigInteger result(1);
    BigInteger counter(1);
    BigInteger one(1);
    while (counter <= *this) {
        oss << counter;
        if (counter < *this) oss << " * ";
        result = result * counter;
        counter = counter + one;
    }
    oss << " = " << result;
    return oss.str();
}
