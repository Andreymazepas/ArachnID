    #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <proxyserver.h>
#include <QtConcurrent>
#include "spiderwindow.h"
#include "dumpwindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void send_request(QString request);
    void send_response(QString response);
private slots:
    void on_requestButton_clicked();
    void on_responseButton_clicked();
    void on_spiderButton_clicked();

    void on_dumpButton_clicked();

public slots:
    void got_request(QString text);
    void got_response(QString text);
    void treat_spider_closed();
private:
    Ui::MainWindow *ui;
    SpiderWindow* spiderUi = new SpiderWindow(this);
    DumpWindow dumpUi;
    ProxyServer pServer;
};

#endif // MAINWINDOW_H
