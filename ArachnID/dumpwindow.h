#ifndef DUMPWINDOW_H
#define DUMPWINDOW_H

#include <QDialog>

namespace Ui {
class DumpWindow;
}

class DumpWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DumpWindow(QWidget *parent = nullptr);
    ~DumpWindow();

private slots:
    void on_changeDirButton_clicked();

    void on_dumpButton_clicked();

private:
    Ui::DumpWindow *ui;
};

#endif // DUMPWINDOW_H
