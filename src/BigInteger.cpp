#include "BigInteger.h"
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <cctype>
#include <cmath>

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

// Factorial
BigInteger BigInteger::factorial() const {
    if (isNegative)
        throw std::invalid_argument("Factorial of negative number is undefined.");
    /**/if (*this > BigInteger("10000"))
        throw std::overflow_error("Factorial too large to compute.");

    BigInteger result(1);
    BigInteger counter(1);
    BigInteger one(1);

    while (counter <= *this) {
        result = result * counter;
        counter = counter + one;
    }

    return result;
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

// Multiplication
BigInteger BigInteger::operator*(const BigInteger& other) const {
    BigInteger result;
    result.blocks.assign(blocks.size() + other.blocks.size(), 0);

    for (size_t i = 0; i < blocks.size(); ++i) {
        long long carry = 0;
        for (size_t j = 0; j < other.blocks.size() || carry; ++j) {
            long long cur = result.blocks[i + j] +
                           blocks[i] * 1LL * (j < other.blocks.size() ? other.blocks[j] : 0) +
                           carry;
            result.blocks[i + j] = static_cast<long long>(cur % BASE);
            carry = static_cast<long long>(cur / BASE);
        }
    }

    result.removeLeadingZeros();
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
