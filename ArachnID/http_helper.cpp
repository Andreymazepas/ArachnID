#include "http_helper.h"
#include <bits/stdc++.h>
#include <QString>
#include <QStringList>
#include <QDebug>

using namespace std;

map<QString, QString> HTTP_parser::atributes;


QString HTTP_parser::get_atribute(QString atribute) {
    return atributes[atribute];
}

void HTTP_parser::parse(QString payload) {
    atributes.clear();
    QStringList lines = payload.split(QString("\r\n"));
    for(auto line : lines) {
//        qDebug() << line << endl;
        if(line.size() == 0) continue;
        auto aux = line.trimmed().toLower();
        QStringList parts = aux.split(":");
        if(parts.size() < 2) continue;
        atributes[parts[0].trimmed()] = parts[1].trimmed();
    }

}

