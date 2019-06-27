#include "http_helper.h"
#include <bits/stdc++.h>
#include <QString>
#include <QStringList>
#include <QDebug>

using namespace std;

map<QString, QString> HTTP_Helper::simplify_http_header(map<QString ,QString> complex_header) {
    map<QString, QString> simplified_header = complex_header;
    simplified_header["connection"] = "close";
    simplified_header["accept-encoding"] = "identity";
    simplified_header.erase("range");
    simplified_header.erase("proxy-connection");
    simplified_header.erase("if-range");
    return simplified_header;
}

QString HTTP_Helper::build_html_header(map<QString, QString>& fields, QString first_line) {
    QString result = "";
    QString CRNL = "";
    CRNL += QChar::CarriageReturn;
    CRNL += QChar::LineFeed;
    result += first_line + CRNL;
    qDebug() << "first line build\n";
    qDebug() << result << endl;
    for(auto key_val : fields) {
        result += key_val.first + ": " + key_val.second + CRNL;
    }
    result += CRNL;
    return result;
}

pair<map<QString, QString>,QString> HTTP_Helper::parse_html_header(QString payload) {
    map<QString, QString> resulting_map;
    QStringList lines = payload.split(QString("\r\n"));
    QString resulting_first_line = lines[0].trimmed();

    for(int i = 1; i < lines.size(); i++) {
        if(lines[i].size() == 0) continue;
        auto aux = lines[i].trimmed().toLower();
        QStringList parts = aux.split(":");
        if(parts.size() < 2) continue;
        resulting_map[parts[0].trimmed()] = parts[1].trimmed();
    }
    return {resulting_map, resulting_first_line};
}

