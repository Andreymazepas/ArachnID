#include "dumpwindow.h"
#include "ui_dumpwindow.h"
#include <QFile>
#include <QFileDialog>
#include "common.h"
#include <QDebug>

DumpWindow::DumpWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DumpWindow)
{
    ui->setupUi(this);
}

DumpWindow::~DumpWindow()
{
    delete ui;
}

void DumpWindow::on_changeDirButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    this->ui->directory->setText(dir);
}

void DumpWindow::on_dumpButton_clicked()
{
    qDebug() << "im useless\n";
}
