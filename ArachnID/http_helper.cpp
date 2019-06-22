#include "http_helper.h"
#include <bits/stdc++.h>
#include <QString>
#include <QStringList>
#include <QDebug>

using namespace std;

QString HTTP_Helper::build_html_header(map<QString, QString>& fields, QString first_line) {
    QString result = "";
    QString CRNL{QChar::CarriageReturn, QChar::LineFeed};
    result += first_line;
    for(auto key_val : fields) {
        result += key_val.first + ": " + key_val.second + CRNL + CRNL;
    }
    result += CRNL;
    return result;
}

map<QString, QString> HTTP_Helper::parse_html_header(QString payload) {
    map<QString, QString> result;
    QStringList lines = payload.split(QString("\r\n"));
    for(auto line : lines) {
        if(line.size() == 0) continue;
        auto aux = line.trimmed().toLower();
        QStringList parts = aux.split(":");
        if(parts.size() < 2) continue;
        result[parts[0].trimmed()] = parts[1].trimmed();
    }
    return result;
}

