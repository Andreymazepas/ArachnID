
#ifndef SPIDERWINDOW_H
#define SPIDERWINDOW_H

#include <QMainWindow>
#include "spider.h"

namespace Ui {
    class SpiderWindow;
}


class SpiderWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SpiderWindow(QWidget *parent = nullptr);
    ~SpiderWindow();
signals:
    void spider_was_closed();
private slots:

    void on_crawlButton_clicked();

private:
    Spider spider;
    QWidget* parent;
    Ui::SpiderWindow *ui;
    pair<QString, QString> split_host_path(QString whole);

protected:
    void showEvent(QShowEvent* event);
};

#endif // SPIDERWINDOW_H
