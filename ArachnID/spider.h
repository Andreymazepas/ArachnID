#ifndef SPIDER_H
#define SPIDER_H

#include <bits/stdc++.h>
#include <QString>
using namespace std;

class Spider {
private:
    QString build_request_for_path(QString host, QString path);
    QStringList extract_links(QString content);
public:
    map<QString, vector<QString>> crawl_page(QString host, QString start_path);

};


#endif // SPIDER_H
