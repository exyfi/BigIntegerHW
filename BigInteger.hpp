//
// Created by Daniil Bolotov on 24.05.18.
//

#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

template<typename T, size_t Base = std::numeric_limits<T>::max()>

class BigInteger {
public:
    explicit BigInteger(long long val = 0);

    explicit BigInteger(const std::string &s);

    explicit BigInteger(const std::vector<T> &number, const int sign);

    template<typename NewT, size_t NewBase>
    explicit operator BigInteger<NewT, NewBase>();

    BigInteger(const BigInteger &);

    BigInteger(BigInteger &&);

    size_t Size() const;

    T operator[](size_t id) const;

//	BigInteger& operator = (BigInteger &val);
    BigInteger &operator=(BigInteger val);

    bool operator<(const BigInteger &val) const;

    bool operator>(const BigInteger &val) const;

    bool operator<=(const BigInteger &val) const;

    bool operator>=(const BigInteger &val) const;

    bool operator==(const BigInteger &val) const;

    bool operator!=(const BigInteger &val) const;

    BigInteger abs() const;

    BigInteger mul(const BigInteger &nsecond);

    BigInteger &operator+=(const BigInteger &val);

    BigInteger operator+(const BigInteger &val) const;

    BigInteger &operator-=(const BigInteger &val);

    BigInteger operator-(const BigInteger &val) const;

    BigInteger &operator*=(const BigInteger &val);

    BigInteger operator*(const BigInteger &val);

    BigInteger &operator/=(long long val);

    BigInteger operator/(long long val) const;

    long long operator%(long long val) const;

    BigInteger operator-() const;

    void clear();

    size_t num_Of_blocks();

    size_t num_Of_bits();

    size_t size_Of_number();

private:
    //  template<typename T, size_t Base> friend std::ostream & operator <<(std::ostream &, const BigInteger &);
    // template<typename T, size_t Base> friend std::istream & operator >>(std::istream &, BigInteger &);
    int sign;
    std::vector<T> number;
    size_t size_of_number = 0;
    const size_t num_of_bits = std::ceil(log2(Base));
    const size_t num_of_blocks = sizeof(T) * 8 / num_of_bits;
};

template<typename T, size_t Base>
std::ostream &operator<<(std::ostream &, const BigInteger<T, Base> &);

template<typename T, size_t Base>
std::istream &operator>>(std::istream &, BigInteger<T, Base> &);