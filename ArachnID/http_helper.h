#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H
#include <bits/stdc++.h>
#include <QString>
using namespace std;


class HTTP_parser {
    private:
        static map<QString, QString> atributes;
    public:
    static void parse(QString payload);
    static QString get_atribute(QString field);
};

#endif // HTTP_HELPER_H
