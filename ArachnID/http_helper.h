#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H
#include <bits/stdc++.h>
#include <QString>
using namespace std;


class HTTP_Helper {
    public:
    static map<QString, QString> parse(QString payload);
};

#endif // HTTP_HELPER_H
