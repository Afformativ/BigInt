#include "BigInt.h"
#include <vector>
#include <cmath>
#include<iostream>
#include<algorithm>
using namespace std;
BigInt::BigInt() {
    this->sign = true;
    this->bits.push_back(0);
}
BigInt::BigInt(const string & s) {
    this->sign = s[0] == '-' ? false : true;
    for (int i = 0; i < s.size(); i++) {
        int digit = (int)s[s.size()-i-1] - '0';
        if (digit <0 || digit > 9) {
            digit = 0;
        }
        this->bits.push_back(digit);
    }
    this->removeLeadingZeros();
}
BigInt::BigInt(const vector<int> &bits, const bool &sign) {
    this->sign = sign;
    this->bits = bits;
    this->removeLeadingZeros();
}
// -
const BigInt operator - (const BigInt &a) {
    return BigInt(a.bits, !(a.sign));
}
//+ - * / % pow sqrt
const BigInt operator + (const BigInt& a, const BigInt &b) {
    if (a.sign != b.sign) {
        return a.sign ? a - abs(b) : b - abs(a);
    }
    long long n = max(a.bits.size(), b.bits.size());
    int carry = 0;
    vector<int> ans;
    for (int i = 0; i < n; i++) {
        int val = carry
            + (i < a.bits.size() ? a.bits[i] : 0)
            + (i < b.bits.size() ? b.bits[i] : 0);
        ans.push_back(val % BigInt::base);
        carry = val / BigInt::base;
    }
    ans.push_back(carry);
    return BigInt(ans, a.sign);
}
const BigInt operator - (const BigInt &a, const BigInt &b) {
    if (a.sign && b.sign) {
        bool sign = b <= a;
        if (!sign) {
            BigInt ans = abs(b) - abs(a);
            ans.sign = false;
            return ans;
        }
        int carry = 0;
        int n = max(a.bits.size(), b.bits.size());
        vector<int> ans;
        for (int i = 0; i < n; i++) {
            int val = carry
            + (i < a.bits.size() ? a.bits[i] : 0)
            - (i < b.bits.size() ? b.bits[i] : 0);
            carry = val < 0 ? -1 : 0;
            val = val < 0 ? val + BigInt::base : val;
            ans.push_back(val);
        }
        ans[n-1] += carry;
        ans[n-1] %= BigInt::base;
        return BigInt(ans, sign);
    }
    if (a.sign) {
        return abs(a) + abs(b);
    }
    if (b.sign) {
        return -(abs(a) + abs(b));
    }
    return abs(b) - abs(a);
}
const BigInt operator * (const BigInt &a, const BigInt &b) {
    int n = a.bits.size() + b.bits.size();
    vector<int> ans(n);
    int carry = 0;
    for (int i = 0; i < a.bits.size(); i++) {
        for (int j = 0; j < b.bits.size(); j++) {
            int val = a.bits[i] * b.bits[j] + carry;
            ans[i + j] += val % BigInt::base;
            carry = val / BigInt::base + ans[i+j] / BigInt::base;
            ans[i + j] %= BigInt::base;
        }
        ans[b.bits.size() + i] += carry;
        carry = 0;
    }
    ans[n-1] += carry;
    return BigInt(ans, !(a.sign ^ b.sign));
}
const BigInt operator * (const BigInt &a, const int b) {
    vector<int> ans(a.bits.size() + 1);
    int carry = 0;
    for (int i = 0; i < a.bits.size(); i++) {
        int val = a.bits[i] * b + carry;
        ans[i] += val % BigInt::base;
        carry = val / BigInt::base + ans[i] / BigInt::base;
        ans[i] %= BigInt::base;
    }
    ans[a.bits.size()] += carry;
    return BigInt(ans, a.sign);
}
const BigInt operator / (const BigInt &a, const BigInt &b) {
    if (b == BigInt("0")) {
        cout << "No division by 0" << endl;
        return a;
    }
    if (abs(a) < abs(b)){
        return BigInt("0");
    }
    BigInt delta(vector<int>(b.bits.size()), true);
    vector<int> ans = vector<int>();
    long long index = a.bits.size();
    while (index > 0) {
        index--;
        delta.bits.insert(delta.bits.begin(), a.bits[index]);
        delta.removeLeadingZeros();
        int digit = findDigit(abs(b), delta);
        delta = delta - abs(b) * digit;
        ans.push_back(digit);
    }
    reverse(ans.begin(), ans.end());
    return BigInt(ans, a.sign == b.sign ? true : false);
}
const BigInt operator % (const BigInt &a, const BigInt &b) {
    if (b <= BigInt("0")) {
        return a;
    }
    BigInt ans = a - b * (a / b);
    return ans >= BigInt("0") ? ans : ans + b;
}
const BigInt pow(const BigInt &a, const BigInt &b, const BigInt& modulo) {
    BigInt ans = BigInt("1");
    BigInt n = b;
    BigInt mult = a;
    while (n > BigInt("0")) {
        BigInt p = n % BigInt("2");
        n = n / BigInt("2");
        if (p == BigInt("1")) {
            ans = ans * mult;
            ans = ans % modulo;
        }
        mult = mult * mult;
        mult = mult % modulo;
    }
    return ans % modulo;
}
const BigInt sqrt(const BigInt &a) {
    if (!a.sign) {
        cout << "Value must be non-negative"<<endl;
        return BigInt();
    }
    BigInt l = BigInt(), r = a;
    BigInt m;
    while (l + BigInt("1") < r) {
        m = (l + r) / BigInt("2");
        if (m * m == a) {
            return m;
        }
        else if (m * m < a){
            l = m;
        }
        else {
            r = m;
        }
    }
    if (r * r <= a)
        return r;
    return l;
}
const BigInt abs(const BigInt &a) {
    return BigInt(a.bits, true);
}
    //> < == <= >= !=
bool BigInt::operator > (const BigInt &a) const {
    return !(*this == a || *this < a);
}
bool BigInt::operator < (const BigInt &a) const {
    if (!a.sign && !this->sign) {
        return abs(a) < abs(*this);
    }
    if (a.sign ^ this->sign) {
        return a.sign ? true : false;
    }
    bool sign = a.sign;
    long long n = max(a.bits.size(), this->bits.size());
    if (a.bits.size() != this->bits.size()) {
        return (this->bits.size() < n) ?  true : false;
    }
    int delta = 0;
    for (int i = n-1; i >= 0; i--) {
        delta = this->bits[i] - a.bits[i];
        if (delta == 0)
            continue;
        return delta < 0 ? true : false;
    }
    return delta < 0 ? true : false;
}
bool BigInt::operator >= (const BigInt &a) const {
    return !(*this<a);
}
bool BigInt::operator <= (const BigInt &a) const {
    return !(*this>a);
}
bool BigInt::operator == (const BigInt &a) const {
    return !(a != *this);
}
bool BigInt::operator != (const BigInt &a) const {
    if (this->sign != a.sign || this->bits.size() != a.bits.size()) {
        return true;
    }
    int n = a.bits.size();
    for (int i = 0; i < n; i++) {
        if (a.bits[i] != this->bits[i])
            return true;
    }
    return false;
}
ostream &operator <<(ostream &out, const BigInt &a) {
    out << (a.sign ? "" : "-");
    int n = a.bits.size();
    for (int i = 0; i < n; i++)
        out<<a.bits[n-i-1];
    return out;
}
istream &operator >>(istream &in, BigInt &a) {
    string s;
    in >> s;
    a = BigInt(s);
    return in;
}
void BigInt::removeLeadingZeros() {
    int i = this->bits.size();
    if (i == 0){
        this->bits.push_back(0);
        return;
    }
    while (this->bits[--i] == 0) {
        this->bits.pop_back();
    }
    if (this->bits.size() == 0) {
        this->bits.push_back(0);
    }
}
const int findDigit(const BigInt& delta, const BigInt& num) {
    int l = 0, r = BigInt::base - 1;
    while (l + 1 < r) {
        int m = (l + r) / 2;
        if (delta * m < num) {
            l = m;
        } else {
            r = m;
        }
    }
    if (delta * r <= num) {
        return r;
    }
    return l;
}
const BigInt gcd(const BigInt& a, const BigInt& b, BigInt& x, BigInt& y) {
    if (a == BigInt("0")) {
		x = BigInt("0");
		y = BigInt("1");
		return b;
	}
	BigInt x1, y1;
	BigInt d = gcd (b % a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}
bool congrEquation(const BigInt& a, const BigInt& b, const BigInt& m, vector<BigInt>& result) {
    BigInt x, y;
    BigInt d = gcd(a, m, x, y);
    if (a < BigInt("0"))
        x = -x;
    x = x * (b / d);
    x = x % m;
    if (m % d != BigInt("0"))
        return false;
    result.push_back(x);
    for (BigInt i = BigInt("1"); i < d; i = i + BigInt("1")) {
        x = x + m / d;
        x = x % m;
        result.push_back(x);
    }
    return true;
}

const BigInt systemOfEquation(const vector<int> &remainders, const vector<int> &modulars) {
    int n = remainders.size();
	vector<vector<int>> inverse = vector<vector<int>>();
	inverse.resize(n);
	BigInt result("0"), multiplier("1");
	for (int i = 0; i < n; i++){
		inverse[i].resize(n);
		for(int j = i + 1; j < n; j++){
			inverse[i][j] = powModP(modulars[i], modulars[j] - 2, modulars[j]);
		}
	}

	vector<int> x = vector<int>(n, 0);
	for (int i = 0; i < n; i++){
		x[i] = remainders[i];
		for (int j = 0; j < i; j++){
			x[i] = (x[i] - x[j]) * inverse[j][i];
			x[i] = (x[i] + modulars[i]) % modulars[i];
		}
		result = result + multiplier * x[i];
		multiplier = multiplier * modulars[i];
	}
	return result;
}
int powModP(int a, int n, int p){
	if (n == 1) return a % p;
	if (n%2 == 1)
		return (a * powModP(a, n-1, p)) % p;
	int tmp = powModP(a, n/2, p) % p;
	return (tmp * tmp) % p;

}