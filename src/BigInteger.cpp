#include "BigInteger.h"
#include <algorithm>
#include <stdexcept>
#include <cctype>

BigInteger::BigInteger() : isNegative(false) {}

BigInteger::BigInteger(const std::string& number) {
    isNegative = !number.empty() && number[0] == '-';
    std::string num = isNegative ? number.substr(1) : number;

    digits.clear();
    for (auto it = num.rbegin(); it != num.rend(); ++it) {
        if (isdigit(*it)) digits.push_back(*it - '0');
    }
    removeLeadingZeros();
}

BigInteger::BigInteger(int number) : BigInteger(std::to_string(number)) {}

void BigInteger::removeLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0)
        digits.pop_back();
    if (digits.size() == 1 && digits[0] == 0)
        isNegative = false;
}

int BigInteger::compareAbs(const BigInteger& a, const BigInteger& b) {
    if (a.digits.size() != b.digits.size())
        return a.digits.size() < b.digits.size() ? -1 : 1;
    for (int i = (int)a.digits.size() - 1; i >= 0; --i) {
        if (a.digits[i] != b.digits[i])
            return a.digits[i] < b.digits[i] ? -1 : 1;
    }
    return 0;
}

BigInteger BigInteger::addAbs(const BigInteger& a, const BigInteger& b) {
    BigInteger result;
    int carry = 0;
    size_t n = std::max(a.digits.size(), b.digits.size());
    for (size_t i = 0; i < n || carry; ++i) {
        int digit = carry;
        if (i < a.digits.size()) digit += a.digits[i];
        if (i < b.digits.size()) digit += b.digits[i];
        result.digits.push_back(digit % 10);
        carry = digit / 10;
    }
    return result;
}

BigInteger BigInteger::subtractAbs(const BigInteger& a, const BigInteger& b) {
    if (compareAbs(a, b) < 0) return subtractAbs(b, a);

    BigInteger result;
    int borrow = 0;
    for (size_t i = 0; i < a.digits.size(); ++i) {
        int digit = a.digits[i] - borrow - (i < b.digits.size() ? b.digits[i] : 0);
        if (digit < 0) {
            digit += 10;
            borrow = 1;
        } else borrow = 0;
        result.digits.push_back(digit);
    }
    result.removeLeadingZeros();
    return result;
}

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

BigInteger BigInteger::operator-(const BigInteger& other) const {
    return *this + (-other);
}

BigInteger BigInteger::operator-() const {
    BigInteger result = *this;
    if (!(digits.size() == 1 && digits[0] == 0))  // Don't make 0 negative
        result.isNegative = !isNegative;
    return result;
}

BigInteger BigInteger::operator*(const BigInteger& other) const {
    BigInteger result;
    result.digits.assign(digits.size() + other.digits.size(), 0);
    for (size_t i = 0; i < digits.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            long long cur = result.digits[i + j] +
                digits[i] * 1LL * (j < other.digits.size() ? other.digits[j] : 0) + carry;
            result.digits[i + j] = static_cast<int>(cur % 10);
            carry = static_cast<int>(cur / 10);
        }
    }
    result.removeLeadingZeros();
    result.isNegative = isNegative != other.isNegative;
    return result;
}

BigInteger BigInteger::operator/(const BigInteger& other) const {
    if (other == BigInteger(0))
        throw std::runtime_error("Division by zero");

    BigInteger dividend = *this;
    BigInteger divisor = other;
    dividend.isNegative = divisor.isNegative = false;
    BigInteger result, current;
    result.digits.resize(dividend.digits.size());

    for (int i = (int)dividend.digits.size() - 1; i >= 0; --i) {
        current.digits.insert(current.digits.begin(), dividend.digits[i]);
        current.removeLeadingZeros();

        int x = 0, l = 0, r = 9;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInteger t = divisor * BigInteger(m);
            if (t <= current) {
                x = m;
                l = m + 1;
            } else r = m - 1;
        }
        result.digits[i] = x;
        current = current - divisor * BigInteger(x);
    }

    result.removeLeadingZeros();
    result.isNegative = isNegative != other.isNegative;
    return result;
}

BigInteger BigInteger::operator%(const BigInteger& other) const {
    return *this - (*this / other) * other;
}

bool BigInteger::operator==(const BigInteger& other) const {
    return isNegative == other.isNegative && digits == other.digits;
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

std::ostream& operator<<(std::ostream& os, const BigInteger& num) {
    if (num.isNegative) os << '-';
    for (auto it = num.digits.rbegin(); it != num.digits.rend(); ++it)
        os << *it;
    return os;
}
