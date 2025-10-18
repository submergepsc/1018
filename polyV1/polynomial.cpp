#include "polynomial.h"

#include <cctype>
#include <limits>
#include <stdexcept>
#include <string>

namespace {
int checkedAdd(int lhs, int rhs)
{
    long long sum = static_cast<long long>(lhs) + rhs;
    if (sum > std::numeric_limits<int>::max() || sum < std::numeric_limits<int>::min()) {
        throw std::overflow_error("integer overflow");
    }
    return static_cast<int>(sum);
}

int checkedSub(int lhs, int rhs)
{
    long long diff = static_cast<long long>(lhs) - rhs;
    if (diff > std::numeric_limits<int>::max() || diff < std::numeric_limits<int>::min()) {
        throw std::overflow_error("integer overflow");
    }
    return static_cast<int>(diff);
}

int checkedMul(int lhs, int rhs)
{
    long long product = static_cast<long long>(lhs) * rhs;
    if (product > std::numeric_limits<int>::max() || product < std::numeric_limits<int>::min()) {
        throw std::overflow_error("integer overflow");
    }
    return static_cast<int>(product);
}

int fastPow(int base, int exponent)
{
    if (exponent < 0) {
        throw std::invalid_argument("exponent must be non-negative");
    }

    int result = 1;
    int factor = base;
    int power = exponent;
    while (power > 0) {
        if (power & 1) {
            result = checkedMul(result, factor);
        }
        power >>= 1;
        if (power > 0) {
            factor = checkedMul(factor, factor);
        }
    }
    return result;
}

std::string removeSpaces(const std::string &input)
{
    std::string normalized;
    normalized.reserve(input.size());
    for (char c : input) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            normalized.push_back(c);
        }
    }
    return normalized;
}
} // namespace

Polynomial::Polynomial() = default;

Polynomial::Polynomial(const std::string &repr)
    : Polynomial()
{
    std::string normalized = removeSpaces(repr);
    if (normalized.empty()) {
        recomputeMetadata();
        return;
    }

    std::size_t i = 0;
    int currentSign = 1;

    auto parseInteger = [&](int &out) {
        if (i >= normalized.size() || !std::isdigit(static_cast<unsigned char>(normalized[i]))) {
            throw std::invalid_argument("expected digit");
        }
        long long value = 0;
        while (i < normalized.size() && std::isdigit(static_cast<unsigned char>(normalized[i]))) {
            value = value * 10 + (normalized[i] - '0');
            if (value > std::numeric_limits<int>::max()) {
                throw std::out_of_range("value too large");
            }
            ++i;
        }
        out = static_cast<int>(value);
    };

    while (i < normalized.size()) {
        if (normalized[i] == '+') {
            currentSign = 1;
            ++i;
            continue;
        }
        if (normalized[i] == '-') {
            currentSign = -1;
            ++i;
            continue;
        }

        int coefficientValue = 0;
        parseInteger(coefficientValue);
        coefficientValue *= currentSign;
        currentSign = 1;

        if (i >= normalized.size() || normalized[i] != '^') {
            throw std::invalid_argument("missing exponent separator '^'");
        }
        ++i;

        int exponentValue = 0;
        parseInteger(exponentValue);
        if (exponentValue < 0) {
            throw std::invalid_argument("negative exponent not supported");
        }
        if (static_cast<std::size_t>(exponentValue) > kMaxExponent) {
            throw std::out_of_range("exponent out of supported range");
        }

        int updated = checkedAdd(coefficients_[static_cast<std::size_t>(exponentValue)],
                                 coefficientValue);
        setCoefficient(static_cast<std::size_t>(exponentValue), updated);
    }

    recomputeMetadata();
}

void Polynomial::setCoefficient(std::size_t exponent, int value)
{
    coefficients_[exponent] = value;
}

void Polynomial::recomputeMetadata()
{
    term_count_ = 0;
    degree_ = 0;
    for (std::size_t i = coefficients_.size(); i-- > 0;) {
        if (coefficients_[i] != 0) {
            degree_ = static_cast<int>(i);
            break;
        }
    }

    for (int i = 0; i <= degree_; ++i) {
        if (coefficients_[static_cast<std::size_t>(i)] != 0) {
            ++term_count_;
        }
    }
}

int Polynomial::coefficient(std::size_t exponent) const
{
    if (exponent >= coefficients_.size()) {
        return 0;
    }
    return coefficients_[exponent];
}

int Polynomial::calculate_val(int x) const
{
    if (degree_ == 0) {
        return coefficients_[0];
    }

    long long result = 0;
    for (int exponent = degree_; exponent >= 0; --exponent) {
        int coef = coefficients_[static_cast<std::size_t>(exponent)];
        if (coef == 0) {
            continue;
        }
        long long term = static_cast<long long>(coef) * fastPow(x, exponent);
        if (term > 0 && result > std::numeric_limits<int>::max() - term) {
            throw std::overflow_error("integer overflow");
        }
        if (term < 0 && result < std::numeric_limits<int>::min() - term) {
            throw std::overflow_error("integer overflow");
        }
        result += term;
    }
    if (result > std::numeric_limits<int>::max() || result < std::numeric_limits<int>::min()) {
        throw std::overflow_error("polynomial evaluation overflow");
    }
    return static_cast<int>(result);
}

void Polynomial::output(std::ostream &os) const
{
    if (term_count_ == 0) {
        os << "0,0," << std::endl;
        return;
    }

    os << term_count_ << ',';
    for (int i = degree_; i >= 0; --i) {
        int coef = coefficients_[static_cast<std::size_t>(i)];
        if (coef == 0) {
            continue;
        }
        os << coef << ',' << i << ',';
    }
    os << std::endl;
}

Polynomial SumOfPolynomial(const Polynomial &p1, const Polynomial &p2)
{
    Polynomial result;
    for (std::size_t i = 0; i <= Polynomial::kMaxExponent; ++i) {
        int value = checkedAdd(p1.coefficients_[i], p2.coefficients_[i]);
        if (value != 0) {
            result.coefficients_[i] = value;
        }
    }
    result.recomputeMetadata();
    return result;
}

Polynomial SubOfPolynomial(const Polynomial &p1, const Polynomial &p2)
{
    Polynomial result;
    for (std::size_t i = 0; i <= Polynomial::kMaxExponent; ++i) {
        int value = checkedSub(p1.coefficients_[i], p2.coefficients_[i]);
        if (value != 0) {
            result.coefficients_[i] = value;
        }
    }
    result.recomputeMetadata();
    return result;
}

Polynomial MulOfPolynomial(const Polynomial &p1, const Polynomial &p2)
{
    Polynomial result;
    if (p1.term_count_ == 0 || p2.term_count_ == 0) {
        return result;
    }

    for (int i = 0; i <= p1.degree_; ++i) {
        int coef1 = p1.coefficients_[static_cast<std::size_t>(i)];
        if (coef1 == 0) {
            continue;
        }
        for (int j = 0; j <= p2.degree_; ++j) {
            int coef2 = p2.coefficients_[static_cast<std::size_t>(j)];
            if (coef2 == 0) {
                continue;
            }
            std::size_t exponent = static_cast<std::size_t>(i + j);
            if (exponent > Polynomial::kMaxExponent) {
                throw std::out_of_range("result exponent exceeds supported range");
            }
            int product = checkedMul(coef1, coef2);
            result.coefficients_[exponent] = checkedAdd(result.coefficients_[exponent], product);
        }
    }

    result.recomputeMetadata();
    return result;
}

Polynomial DerOfPolynomial(const Polynomial &p)
{
    Polynomial result;
    if (p.degree_ == 0 || p.term_count_ == 0) {
        return result;
    }

    for (int i = 1; i <= p.degree_; ++i) {
        int coef = p.coefficients_[static_cast<std::size_t>(i)];
        if (coef == 0) {
            continue;
        }
        std::size_t exponent = static_cast<std::size_t>(i - 1);
        result.coefficients_[exponent] = checkedMul(coef, i);
    }
    result.recomputeMetadata();
    return result;
}
