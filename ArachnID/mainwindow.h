#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <proxyserver.h>
#include <QtConcurrent>

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

public slots:
    void got_request(QString text);
    void got_response(QString text);

private:
    Ui::MainWindow *ui;
    ProxyServer pServer;
};

#endif // MAINWINDOW_H
