#include "spiderwindow.h"
#include "ui_spiderwindow.h"
#include <QtConcurrent>
#include "spider.h"
#include <QString>
#include <QFuture>

using namespace std;

SpiderWindow::SpiderWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::SpiderWindow) {
    ui->setupUi(this);
    this->parent = parent;
}

class MainWindow : public QMainWindow{};

void SpiderWindow::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    qDebug() << "ShowEvent: Apagando campos" << endl;
    this->ui->urlEdit->clear();
    this->ui->plainTextEdit->clear();
}

SpiderWindow::~SpiderWindow() {
    delete ui;
}

pair<QString, QString> SpiderWindow::split_host_path(QString whole) {
    QString host, path;
    host = whole.left(whole.indexOf("/"));
    path = whole.right(int(whole.size()) - int(host.size()));
    return {host, path};
}

void SpiderWindow::on_crawlButton_clicked() {
    QString host, path;
    tie(host, path) = split_host_path(ui->urlEdit->text());
    qDebug() << "host antes de chamar" << endl;
    qDebug() << host << endl;
    qDebug() << "path antes de chamar" << endl;
    qDebug() << path << endl;
    auto result_future = QtConcurrent::run(&this->spider, &Spider::crawl_page, host, path);
    map<QString, vector<QString>> result = result_future.result();
    for(auto value : result) {
        qDebug() << value.first << ":" << endl;
        for(auto value2 : value.second) {
            qDebug() << value2 << '\t';
        }
        qDebug() << endl;
    }
}
