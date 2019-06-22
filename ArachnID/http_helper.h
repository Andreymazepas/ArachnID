#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H
#include <bits/stdc++.h>
#include <QString>
using namespace std;


class HTTP_Helper {
    public:
    static pair<map<QString, QString>, QString>  parse_html_header(QString payload);
    static QString build_html_header(map<QString, QString>& fields, QString first_line);

};

#endif // HTTP_HELPER_H
