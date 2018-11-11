//
// Created by Daniil Bolotov on 24.05.18.
//

#include "BigInteger.hpp"
#include <string>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <vector>

const int standartBase = (int) 1e9;
const int step = 9;

using namespace std;

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(long long val) {
    sign = val < 0 ? -1 : 1;
    val *= sign;
    number.clear();
    size_of_number = 0;
    std::vector<T> buf(0);
    while (val > 0) {
        for (size_t i = 0; i < num_of_blocks && val > 0; i++) {
            buf.push_back(val % Base);
            size_of_number++;
            val /= Base;
        }
        std::reverse(buf.begin(), buf.end());
        T cur = 0;
        for (size_t i = 0; i < buf.size(); i++) {
            cur = cur << num_of_bits;
            cur = cur | buf[i];
        }
        number.push_back(cur);
        buf.clear();
    }
    clear();
}

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(const std::vector<T> &number, const int sign) {

    this->number = number;
    this->sign = sign;
    clear();
}

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(const std::string &s) {
    std::vector<int> v(0);
    int pos = 0;
    int sign = 1;
    if (s[pos] == '-') {
        sign = -1;
        pos++;
    }
    for (int i = (int) s.size() - 1; i >= pos; i -= step) {
        int digit = 0;
        for (int j = std::max(pos, i - step + 1); j <= i; j++) {
            digit = digit * 10 + (int) (s[j] - '0');
        }
        v.push_back(digit);
    }

    while (!v.empty() && v.back() == 0) {
        v.pop_back();
    }
    *this = static_cast<BigInteger <T, Base>>(BigInteger < int, standartBase > (v, sign));
}

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(const BigInteger &val) {
    number = val.number;
    sign = val.sign;
    size_of_number = val.size_of_number;
}

template<typename T, size_t Base>
BigInteger<T, Base>::BigInteger(BigInteger &&val) {
    number = std::move(val.number);
    sign = val.sign;
    size_of_number = val.size_of_number;
}

template<typename T, size_t Base>
size_t BigInteger<T, Base>::Size() const {
    return size_of_number;
}

template<typename T, size_t Base>
T BigInteger<T, Base>::operator[](size_t id) const {
    size_t pos = id / num_of_blocks;
    size_t shift = id % num_of_blocks;
    T cur = number[pos] >> (num_of_bits * shift);
    T mask = (T(1) << num_of_bits) - 1;
    T res = cur & mask;
    return res;
}

template<typename T, size_t Base>
template<typename NewT, size_t NewBase>
BigInteger<T, Base>::operator BigInteger<NewT, NewBase>() {
    BigInteger<NewT, 10> pow(1);
    BigInteger<NewT, 10> result(0);
    BigInteger<NewT, 10> base(Base);

    for (size_t i = 0; i < Size(); i++) {
        BigInteger<NewT, 10> digit((*this)[i]);
        BigInteger<NewT, 10> buffer = digit.mul(pow);
        result += buffer;
        pow = pow.mul(base);
    }

    NewT num = 0;
    std::vector<NewT> buffer;
    size_t new_num_of_bits = ceil(log2(NewBase));
    size_t new_num_of_blocks = sizeof(NewT) * 8 / new_num_of_bits;

    int index = 0;
    NewT path = 0;
    while (result.Size() > 0) {
        path = (result % NewBase);
        result /= NewBase;
        num += (path << (index % new_num_of_blocks) * new_num_of_bits);
        if (index % new_num_of_blocks == new_num_of_blocks - 1) {
            buffer.push_back(num);
            num = 0;
        }
        index++;
    }
    if (num != 0) {
        buffer.push_back(num);
    }
    BigInteger <NewT, NewBase> ans(buffer, sign);
    return ans;
}

template<typename T, size_t Base>
BigInteger <T, Base> &BigInteger<T, Base>::operator=(BigInteger <T, Base> val) {
    number = val.number;
    sign = val.sign;
    size_of_number = val.size_of_number;
    return *this;
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::operator<(const BigInteger &val) const {
    if (sign != val.sign)
        return sign < val.sign;
    if (Size() != val.Size())
        return sign >= 0 ? Size() < val.Size() : Size() > val.Size();
    for (int i = int(val.Size()) - 1; i >= 0; i--)
        if ((*this)[i] != val[i])
            return (*this)[i] * sign < val[i] * val.sign;
    return false;
}


template<typename T, size_t Base>
bool BigInteger<T, Base>::operator==(const BigInteger &val) const {
    return !(*this < val) && !(val < *this);
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::operator!=(const BigInteger &val) const {
    return !(*this == val);
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::operator>(const BigInteger &val) const {
    return !(*this == val) && !(*this < val);
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::operator<=(const BigInteger &val) const {
    return (*this < val) || (*this == val);
}

template<typename T, size_t Base>
bool BigInteger<T, Base>::operator>=(const BigInteger &val) const {
    return (*this > val) || (*this == val);
}

template<typename T, size_t Base>
BigInteger <T, Base> BigInteger<T, Base>::abs() const {
    BigInteger <T, Base> buffer = *this;
    if (buffer.sign == -1) {
        buffer.sign = 1;
    }
    return buffer;
}

template<typename T, size_t Base>
BigInteger <T, Base> &BigInteger<T, Base>::operator+=(const BigInteger &val) {
    if (sign == val.sign) {
        T carry = 0;
        size_t len_of_this = number.size();
        for (size_t i = 0; i < std::max(val.number.size(), len_of_this) || carry; i++) {
            if (i == number.size()) {
                number.push_back(0);
                size_of_number += num_of_blocks;
            }
            T num = 0;
            if (i < len_of_this) {
                if (i < val.number.size()) {
                    for (size_t j = 0; j < num_of_blocks; j++) {
                        T first = val[i * num_of_blocks + j];
                        T second = (*this)[i * num_of_blocks + j];
                        if (first >= T(Base) - T(carry) - second) {
                            num = num + ((first - T(Base) + second + carry) << j * num_of_bits);
                            carry = 1;
                        } else {
                            num += ((first + second + carry) << j * num_of_bits);
                            carry = 0;
                        }
                    }
                } else {
                    for (size_t j = 0; j < num_of_blocks; j++) {
                        T first = (*this)[i * num_of_blocks + j];
                        if (first >= T(Base) - carry) {
                            num += ((first - (T) (Base) + carry) << j * num_of_bits);
                            carry = 1;
                        } else {
                            num += ((first + carry) << j * num_of_bits);
                            carry = 0;
                        }
                    }
                }
            } else if (i < val.number.size()) {
                for (size_t j = 0; j < num_of_blocks; j++) {
                    T first = val[i * num_of_blocks + j];
                    if (first >= T(Base) - carry) {
                        num += ((first + (-(T) Base + carry)) << j * num_of_bits);
                        carry = 1;
                    } else {
                        num += ((first + carry) << j * num_of_bits);
                        carry = 0;
                    }
                }
            } else {
                num = carry;
                carry = 0;
            }
            number[i] = num;
        }
        clear();
        return *this;
    } else {
        (*this) -= (-val);
        clear();
        return *this;
    }
}

template<typename T, size_t Base>
BigInteger <T, Base> BigInteger<T, Base>::operator+(const BigInteger &val) const {
    BigInteger <T, Base> buffer = *this;
    buffer += val;
    return buffer;
}

template<typename T, size_t Base>
BigInteger <T, Base> &BigInteger<T, Base>::operator-=(const BigInteger &val) {
    if (sign == val.sign) {
        if ((*this).abs() >= val.abs()) {
            T carry = 0;
            for (size_t i = 0; i < val.number.size() || carry; i++) {
                T num = 0;
                if (i < val.number.size()) {
                    for (size_t j = 0; j < num_of_blocks; j++) {
                        T first = (*this)[i * num_of_blocks + j];
                        T second = val[i * num_of_blocks + j];
                        if (first < second + carry) {
                            num += (((T(Base) - carry - second) + first) << j * num_of_bits);
                            carry = 1;
                        } else {
                            num += ((first - second - carry) << j * num_of_bits);
                            carry = 0;
                        }
                    }
                } else {
                    for (size_t j = 0; j < num_of_blocks; j++) {
                        T first = (*this)[i * num_of_blocks + j];
                        if (first < carry) {
                            num += ((T(Base) - carry + first) << j * num_of_bits);
                            carry = 1;
                        } else {
                            num += ((first - carry) << j * num_of_bits);
                            carry = 0;
                        }
                    }
                }
                (*this).number[i] = num;
            }
            clear();
            return *this;
        } else {
            *this = (-(val - *this));
            clear();
            return *this;
        }
    } else {
        *this = *this + (-val);
        clear();
        return *this;
    }
}

template<typename T, size_t Base>
BigInteger <T, Base> BigInteger<T, Base>::operator-(const BigInteger &val) const {
    BigInteger <T, Base> buffer = *this;
    buffer -= val;
    return buffer;
}

template<typename T, size_t Base>
BigInteger <T, Base> &BigInteger<T, Base>::operator*=(const BigInteger &val) {
    BigInteger <T, Base> val1 = val;
    *this = *this * val1;
    return *this;
}

template<typename T, size_t Base>
BigInteger <T, Base> BigInteger<T, Base>::mul(const BigInteger &nsecond) {
    vector<T> res((*this).Size() + nsecond.Size());
    vector<T> first((*this).Size());
    vector<T> second(nsecond.Size());

    for (int i = 0; i < (int) first.size(); i++) {
        first[i] = (*this)[i];
    }
    for (int i = 0; i < (int) second.size(); i++) {
        second[i] = nsecond[i];
    }
    for (int i = 0; i < (int) first.size(); i++) {
        long long carry = 0;
        for (int j = 0; j < (int) second.size() || carry; j++) {
            long long cur = (long long) res[i + j] +
                            ((long long) first[i] * (long long) (j < (int) second.size() ? second[j] : 0) + carry);
            res[i + j] = (T) (cur % Base);
            carry = (T) (cur / Base);
        }
    }

    while (!res.empty() && res.back() == (T) 0) {
        res.pop_back();
    }

    vector<T> buf;
    T num = 0;
    for (size_t i = 0; i < res.size(); i++) {
        num += (res[i] << ((i % num_of_blocks) * num_of_bits));
        if (i % num_of_blocks == num_of_blocks - 1) {
            buf.push_back(num);
            num = 0;
        }
    }

    if (num != 0) {
        buf.push_back(num);
    }

    BigInteger <T, Base> ans(buf, sign == nsecond.sign ? 1 : -1);
    ans.clear();
    return ans;
}

template<typename T, size_t Base>
BigInteger <T, Base> BigInteger<T, Base>::operator*(const BigInteger &val) {
    const long long type = 1e9;
    BigInteger <T, Base> vval1 = *this;
    BigInteger <T, Base> vval2 = val;
    BigInteger<long long, type> nfirst = static_cast<BigInteger<long long, type>>(vval1);
    BigInteger<long long, type> nsecond = static_cast<BigInteger<long long, type>>(vval2);
    vector<long long> res(nfirst.Size() + nsecond.Size());
    vector<long long> first(nfirst.Size());
    vector<long long> second(nsecond.Size());

    for (int i = 0; i < (int) first.size(); i++) {
        first[i] = nfirst[i];
    }
    for (int i = 0; i < (int) second.size(); i++) {
        second[i] = nsecond[i];
    }
    for (int i = 0; i < (int) first.size(); i++) {
        long long carry = 0;
        for (int j = 0; j < (int) second.size() || carry; j++) {
            long long cur = (long long) res[i + j] +
                            ((long long) first[i] * (long long) (j < (int) second.size() ? second[j] : 0) + carry);
            res[i + j] = (long long) (cur % type);
            carry = (long long) (cur / type);
        }
    }

    while (!res.empty() && res.back() == (long long) 0) {
        res.pop_back();
    }

    vector<long long> buf;
    long long num = 0;
    size_t nnum_of_bits = ceil(log2(type));
    size_t nnum_of_blocks = sizeof(long long) * 8 / nnum_of_bits;
    for (size_t i = 0; i < res.size(); i++) {
        num += (res[i] << ((i % nnum_of_blocks) * nnum_of_bits));
        if (i % nnum_of_blocks == nnum_of_blocks - 1) {
            buf.push_back(num);
            num = 0;
        }
    }

    if (num != 0) {
        buf.push_back(num);
    }

    BigInteger<long long, type> ans(buf, sign == val.sign ? 1 : -1);
    ans.clear();
    BigInteger <T, Base> answer = static_cast<BigInteger <T, Base>>(ans);
    return answer;
}

template<typename T, size_t Base>
BigInteger <T, Base> &BigInteger<T, Base>::operator/=(long long val) {
    if (Size() == 0) {
        return *this;
    }

    if (val < 0) {
        sign *= -1;
        val *= -1;
    }
    unsigned long long path = 0;
    T num = 0;
    int index = number.size() - 1;
    for (int i = int(Size()) - 1; i >= 0; i--) {
        unsigned long long cur = path * Base + (*this)[i];
        path = cur % val;
        num += (T(cur / val) << ((i % num_of_blocks) * num_of_bits));
        if (i % num_of_blocks == 0) {
            number[index] = num;
            num = 0;
            index--;
        }
    }
    (*this).clear();
    return *this;
}

template<typename T, size_t Base>
BigInteger <T, Base> BigInteger<T, Base>::operator/(long long val) const {
    BigInteger <T, Base> buffer = *this;
    buffer /= val;
    return buffer;
}


template<typename T, size_t Base>
long long BigInteger<T, Base>::operator%(long long val) const {
    if (Size() == 0) {
        return 0;
    }
    unsigned long long path = 0;
    for (int i = int(Size()) - 1; i >= 0; i--) {
        path = (path * Base + (*this)[i]) % val;
    }
    return path;
}

template<typename T, size_t Base>
BigInteger <T, Base> BigInteger<T, Base>::operator-() const {
    BigInteger <T, Base> buffer = *this;
    buffer.sign = -sign;
    return buffer;
}

template<typename T, size_t Base>
void BigInteger<T, Base>::clear() {
    while (!number.empty() && number.back() == 0) {
        number.pop_back();
    }
    size_of_number = num_of_blocks * number.size();
    while (!number.empty() && (*this)[size_of_number - 1] == 0)
        size_of_number--;
    if (number.empty()) {
        sign = 1;
    }
}

template<typename T, size_t Base>
std::ostream &operator<<(std::ostream &os, BigInteger <T, Base> &val) {
    auto tmp = static_cast<BigInteger<int, 10>>(val);
    auto Zero = BigInteger<int, 10>(0);
    if (tmp < Zero) {
        os << '-';
    }
    if (tmp.Size() == 0) {
        os << 0;
    } else {
        for (int i = (int) tmp.Size() - 1; i >= 0; i--) {
            os << tmp[i];
        }
    }
    return os;
}

template<typename T, size_t Base>
std::istream &operator>>(std::istream &is, BigInteger <T, Base> &val) {
    std::string s;
    is >> s;
    BigInteger <T, Base> buffer = BigInteger<T, Base>(s);
    val = buffer;
    return is;
}
