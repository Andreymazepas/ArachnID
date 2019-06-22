#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(&pServer, &ProxyServer::got_request, this, &MainWindow::got_request);
    connect(this, &MainWindow::send_request, &pServer, &ProxyServer::send_request_to_the_web);

    connect(&pServer, &ProxyServer::got_response, this, &MainWindow::got_response);
    connect(this, &MainWindow::send_response, &pServer, &ProxyServer::send_response_to_the_browser);

    QFuture<void> _ = QtConcurrent::run(&this->pServer, &ProxyServer::setup);
}

MainWindow::~MainWindow() {
    delete &(this->pServer);
    delete ui;
}

void MainWindow::on_pushButton_3_clicked() {
    auto aux = this->ui->plainTextEdit->toPlainText();
    this->ui->plainTextEdit->clear();
    emit send_request(aux);
}

void MainWindow::got_request(QString text)
{
   //qDebug() << "From mainWindow: " << name << " " << number;
    this->ui->plainTextEdit->setPlainText(text);
}

void MainWindow::got_response(QString text)
{
   //qDebug() << "From mainWindow: " << name << " " << number;
    this->ui->plainTextEdit_2->setPlainText(text);
}

void MainWindow::on_pushButton_4_clicked()
{
    auto aux = this->ui->plainTextEdit_2->toPlainText();
    this->ui->plainTextEdit_2->clear();
    emit send_response(aux);
}
