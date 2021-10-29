#include <vector>
#include <cmath>
#include<iostream>
#include<algorithm>
#include "BigInt.h"
using namespace std;

void info();
void error();
void process();
bool processLine(BigInt &, BigInt &, BigInt &);
int main()
{
    info();
    while (true)
        process();
    return 0;
}
void info() {
    cout << "------"<< endl;
    cout << "Choose some option. [] means that parameter is optional.\n";
    cout << "a, b - arguments, p - modulo. Input numbers in format 9, -1766, 0, 2345634545411983." << endl;
    cout << "add a b [p]"<< endl;
    cout << "sub a b [p]" << endl;
    cout << "mult a b [p]" << endl;
    cout << "div a b [p]" << endl;
    cout << "mod a b" << endl;
    cout << "sqrt a" << endl;
    cout << "pow a b [p]" << endl;
    cout << "abs a" << endl;
    cout << "** to solve system of equations equation x = ai (mod mi) **" << endl;
    cout << "equation" << endl;
    cout << "info" << endl;
    cout << "exit" << endl;
    cout << "------"<< endl;
}
void error() {
    cout << "Wrong input!" << endl;
}
bool processLine(BigInt &a, BigInt &b, BigInt &p) {
    string s;
    getline(cin, s, '\n');
    int counter = -1;
    string nums[] = {"", "", ""};
    int i = 0;
    int len = s.size();
    while (counter < 3 && i < len) {
        while (s[i] != ' ' && i < len){
            nums[counter] += s[i];
            i++;
        }
        if (s[i] == ' '){
            counter++;
            if (counter > 2)
                continue;
            while (s[i] == ' ' && i < len)
                i++;
        }
    }
    if (counter < 1)
        return false;
    a = BigInt(nums[0]);
    b = BigInt(nums[1]);
    p = BigInt("0");
    if (counter == 2)
        p = BigInt(nums[2]);
    return true;
}
void process() {
    string command;
    BigInt a, b, p;
    if (cin>>command) {
        BigInt ans;
        if (command == "equation") {
            int c, d, n;
            cout << "Input number of equations." << endl;
            vector<int> r(0);
            vector<int> m(0);
            cin >> n;
            cout << "Input coefs in format < ai mi > on each line. Print done to stop input" << endl;
            while (n > 0) {
                cin >> c >> d;
                r.push_back(c);
                m.push_back(d);
                n--;
            }
            ans = systemOfEquation(r,m);
                cout << ans;
        }
        else {
            if (command == "exit")
                exit(0);
            if (command == "info") {
                info();
                return;
            }
            if (command == "add") {
                if (processLine(a, b, p))
                    ans = (a + b) % p;
                else
                    error();
            }
            if (command == "sub") {
                if (processLine(a, b, p))
                    ans = (a - b) % p;
                else
                    error();
            }
            if (command == "mult") {
                if (processLine(a, b, p))
                    ans = (a * b) % p;
                else
                    error();
            }
            if (command == "div") {
                if (processLine(a, b, p))
                    ans = (a / b) % p;
                else
                    error();
            }
            if (command == "mod") {
                if (cin >> a && cin >> b)
                    ans = a % b;
                else
                    error();
            }
            if (command == "sqrt") {
                if (cin >> a)
                    ans = sqrt(a);
                else
                    error();
            }
            if (command == "pow") {
                if (processLine(a, b, p))
                    ans = pow(a, b, p);
                else
                    error();
            }
            if (command == "abs") {
                if (cin >> a)
                    ans = abs(a);
                else
                    error();
            }
            cout << ans << endl;
        }
    }
    else{
        error();
        return;
    }

}