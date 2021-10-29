#ifndef BIGINT_H
#define BIGINT_H
#endif // BIGINT_H_INCLUDED
#pragma once
#include <vector>
#include <cmath>
#include <string>
using namespace std;
class BigInt {
private:
    void removeLeadingZeros();
public:
    BigInt();
    BigInt(const string &s);
    BigInt(const vector<int> &bits, const bool &sign);
    bool sign = true; // >=0
    const static int base = 10;
    vector<int> bits = vector<int>();
    // -
    friend const BigInt operator - (const BigInt &a);
    //+ - * / % pow sqrt
    friend const BigInt operator + (const BigInt &a, const BigInt &b);
    friend const BigInt operator - (const BigInt &a, const BigInt &b);
    friend const BigInt operator * (const BigInt &a, const BigInt &b);
    friend const BigInt operator * (const BigInt &a, const int b);
    friend const BigInt operator / (const BigInt &a, const BigInt &b);
    friend const BigInt operator % (const BigInt &a, const BigInt &b);
    friend const BigInt pow(const BigInt &a, const BigInt &n, const BigInt& modulo);
    friend const BigInt sqrt(const BigInt &a);
    friend const BigInt abs(const BigInt &a);
    //> < == <= >= !=
    bool operator > (const BigInt &a) const;
    bool operator < (const BigInt &a) const;
    bool operator >= (const BigInt &a) const;
    bool operator <= (const BigInt &a) const;
    bool operator == (const BigInt &a) const;
    bool operator != (const BigInt &a) const;
    // output in console
    friend ostream &operator <<(ostream &out, const BigInt &a);
    friend istream &operator >>(istream &in, BigInt &a);
};

const int findDigit(const BigInt& delta, const BigInt& num);
const BigInt gcd(const BigInt& a, const BigInt& b, BigInt& x, BigInt& y);
bool congrEquation(const BigInt& a, const BigInt& b, const BigInt& m, vector<BigInt>& result);
const BigInt systemOfEquation(const vector<int> &r, const vector<int> &m);
int powModP(int a, int n, int p);