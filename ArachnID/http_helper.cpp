#include "http_helper.h"
#include <bits/stdc++.h>
#include <QString>
#include <QStringList>
#include <QDebug>

using namespace std;


map<QString, QString> HTTP_Helper::parse(QString payload) {
    map<QString, QString> result;
    QStringList lines = payload.split(QString("\r\n"));
    for(auto line : lines) {
//        qDebug() << line << endl;
        if(line.size() == 0) continue;
        auto aux = line.trimmed().toLower();
        QStringList parts = aux.split(":");
        if(parts.size() < 2) continue;
        result[parts[0].trimmed()] = parts[1].trimmed();
    }
    return result;
}

