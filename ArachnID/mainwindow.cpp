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
    this->ui->requestButton->setDisabled(true);
    this->ui->responseButton->setDisabled(true);
}

MainWindow::~MainWindow() {
    delete &(this->pServer);
    delete ui;
}

void MainWindow::on_requestButton_clicked() {
    auto aux = this->ui->requestTextEdit->toPlainText();
    this->ui->requestTextEdit->clear();
    this->ui->requestButton->setDisabled(true);
    emit send_request(aux);
}

void MainWindow::got_request(QString text)
{
   //qDebug() << "From mainWindow: " << name << " " << number;
    this->ui->responseButton->setDisabled(true);
    this->ui->requestButton->setEnabled(true);
    this->ui->requestTextEdit->setPlainText(text);
}

void MainWindow::got_response(QString text)
{
   //qDebug() << "From mainWindow: " << name << " " << number;
    this->ui->responseButton->setEnabled(true);
    this->ui->responseTextEdit->setPlainText(text);
}

void MainWindow::on_responseButton_clicked()
{
    this->ui->responseButton->setDisabled(true);
    auto aux = this->ui->responseTextEdit->toPlainText();
    this->ui->responseTextEdit->clear();
    emit send_response(aux);
}
