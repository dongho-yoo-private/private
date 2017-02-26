#include <iostream>
#include <string>
using namespace std;

string decode(const string& s)
{
    string t;
    string::const_iterator p = s.begin();
    while (p != s.end()) {
        if (*p == '%') {
            if (++p == s.end()) break;
            int d = *p;
            if (++p == s.end()) break;
            int e = *p;
            d = (d >= 'A') ? (d & 0xdf) - 'A' + 10 : d - '0';
            e = (e >= 'A') ? (e & 0xdf) - 'A' + 10 : e - '0';
            if (d >= 0 && d < 16 && e >= 0 && e < 16)
                t += char(16 * d + e);
        } else if (*p == '+') {
            t += ' ';
        } else if (*p == '&') {
            t += '\n';
        } else {
            t += *p;
        }
        p++;
    }
    return t;
}

