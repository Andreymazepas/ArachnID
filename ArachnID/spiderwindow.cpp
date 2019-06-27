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
    this->ui->lineEdit->clear();
    this->ui->plainTextEdit->clear();
}



SpiderWindow::~SpiderWindow() {
    delete ui;
}

void SpiderWindow::on_pushButton_clicked() {
    QString host = "www.howto-pages.org";
    QString path = "/";
    auto result_future = QtConcurrent::run(&this->spider, &Spider::crawl_page, host, path);
    map<QString, vector<QString>> result = result_future.result();

}
