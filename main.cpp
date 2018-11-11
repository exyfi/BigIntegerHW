#include "BigInteger.hpp"
#include "BigInteger.cpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>

using namespace std;

int main() {

    BigInteger<int, (int) 1e9> a;
    BigInteger<int, (int) 1e9> b;
    BigInteger<long long, (int) 1e4> c;
    cin >> a;
    cin >> b;
    cout << "a = " << a;
    cout << endl;
    cout << "b = " << b;
    cout << endl;
    cout << " < " << (a < b);
    cout << endl;
    cout << " > " << (a > b);
    cout << endl;
    cout << " = " << (a == b);
    cout << endl;
    cout << " a + b = ";
    BigInteger<int, (int) 1e9> buf = a + b;
    cout << buf;
    cout << endl;
    cout << " a - b = ";
    buf = a - b;
    cout << buf;
    cout << endl;
    cout << " a * b = ";
    buf = a * b;
    cout << buf;
    cout << endl;
    cout << "________________________________________" << endl;
    cout << " cast int to long long and system 10^18 " << endl;
    BigInteger<long long, (long long) 1e18> a1 = static_cast<BigInteger<long long, (long long) 1e18>>(a);
    BigInteger<long long, (long long) 1e18> b1 = static_cast<BigInteger<long long, (long long) 1e18>>(b);
    cout << " a1 = " << a1;
    cout << endl;
    cout << " b1 = " << b1;
    cout << endl;
    BigInteger<long long, (long long) 1e18> buf1 = a1 + b1;
    cout << " a1 + b1 = ";
    cout << buf1;
    cout << endl;
    cout << " a1 - b1 = ";
    buf1 = a1 - b1;
    cout << buf1;
    cout << endl;
    cout << " a1 * b1 = ";
    buf1 = a1 * b1;
    cout << buf1;
    cout << endl;
    cout << "________________________________________" << endl;
    cout << " cast int to char and system 10^1 " << endl;
    BigInteger<char, 10> a2 = static_cast<BigInteger<char, 10>>(a);
    BigInteger<char, 10> b2 = static_cast<BigInteger<char, 10>>(b);
    cout << " a2 = " << a2;
    cout << endl;
    cout << " b2 = " << b2;
    cout << endl;
    BigInteger<char, 10> buf2 = a2 + b2;
    cout << " a2 + b2 = ";
    cout << buf2;
    cout << endl;
    cout << " a2 - b2 = ";
    buf2 = a2 - b2;
    cout << buf2;
    cout << endl;
    cout << " a2 * b2 = ";
    buf2 = a2 * b2;
    cout << buf2;
    cout << endl;
    cout << "________________________________________" << endl;
    cout << " cast int to char and system 8 " << endl;
    BigInteger<char, 8> a3 = static_cast<BigInteger<char, 8>>(a);
    BigInteger<char, 8> b3 = static_cast<BigInteger<char, 8>>(b);
    cout << " a3 = " << a3;
    cout << endl;
    cout << " b3 = " << b3;
    cout << endl;
    BigInteger<char, 8> buf3 = a3 + b3;
    cout << " a3 + b3 = ";
    cout << buf3;
    cout << endl;
    cout << " a3 - b3 = ";
    buf3 = a3 - b3;
    cout << buf3;
    cout << endl;
    cout << " a3 * b3 = ";
    buf3 = a3 * b3;
    cout << buf3;
    cout << endl;
    cout << "________________________________________" << endl;
    cout << " cast int to char and system bin " << endl;
    BigInteger<char, 2> a4 = static_cast<BigInteger<char, 2>>(a);
    BigInteger<char, 2> b4 = static_cast<BigInteger<char, 2>>(b);
    cout << " a4 = " << a4;
    cout << endl;
    cout << " b4 = " << b4;
    cout << endl;
    BigInteger<char, 2> buf4 = a4 + b4;
    cout << " a4 + b4 = ";
    cout << buf4;
    cout << endl;
    cout << " a4 - b4 = ";
    buf4 = a4 - b4;
    cout << buf4;
    cout << endl;
    cout << " a4 * b4 = ";
    buf4 = a4 * b4;
    cout << buf4;
    cout << endl;
    cout << "________________________________________" << endl;

}

