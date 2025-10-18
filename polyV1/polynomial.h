#pragma once   
#include <array>
#include <cstddef>
#include <iosfwd>
#include <iostream>
#include <string>

class Polynomial {
public:
    static constexpr std::size_t kMaxExponent = 9999;

    Polynomial();
    explicit Polynomial(const std::string &repr);
    Polynomial(const Polynomial &) = default;
    Polynomial &operator=(const Polynomial &) = default;
    ~Polynomial() = default;

    [[nodiscard]] int calculate_val(int x) const;
    void output(std::ostream &os = std::cout) const;

    [[nodiscard]] int degree() const noexcept { return degree_; }
    [[nodiscard]] int term_count() const noexcept { return term_count_; }
    [[nodiscard]] int coefficient(std::size_t exponent) const;

    [[nodiscard]] const std::array<int, kMaxExponent + 1> &coefficients() const noexcept
    {
        return coefficients_;
    }

    friend Polynomial SumOfPolynomial(const Polynomial &p1, const Polynomial &p2);
    friend Polynomial SubOfPolynomial(const Polynomial &p1, const Polynomial &p2);
    friend Polynomial MulOfPolynomial(const Polynomial &p1, const Polynomial &p2);
    friend Polynomial DerOfPolynomial(const Polynomial &p);

private:
    std::array<int, kMaxExponent + 1> coefficients_{};
    int degree_ = 0;
    int term_count_ = 0;

    void recomputeMetadata();
    void setCoefficient(std::size_t exponent, int value);
};

Polynomial SumOfPolynomial(const Polynomial &p1, const Polynomial &p2);
Polynomial SubOfPolynomial(const Polynomial &p1, const Polynomial &p2);
Polynomial MulOfPolynomial(const Polynomial &p1, const Polynomial &p2);
Polynomial DerOfPolynomial(const Polynomial &p);
