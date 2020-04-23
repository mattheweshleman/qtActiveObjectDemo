#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "ActiveObjectDemo.hpp"
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mActiveObject(new ActiveObjectDemo())
{
    ui->setupUi(this);

    auto primaryThread = QThread::currentThread();
    primaryThread->setObjectName("primaryThread");

    auto activeObjectThread = new QThread();
    activeObjectThread->setObjectName("activeObjectThread");
    mActiveObject->moveToThread(activeObjectThread);
    activeObjectThread->start();

    connect(ui->slotButton, &QAbstractButton::clicked, mActiveObject, &ActiveObjectDemo::DemoPublicSlot,
            Qt::QueuedConnection); //Qt should ensure queued connection, but I prefer to be explicit

    connect(mActiveObject, &ActiveObjectDemo::DemoOutput, ui->textBrowser, &QTextBrowser::append,
            Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_apiButton_clicked()
{
    mActiveObject->DemoApiAsync(ui->lineEdit->text());
}
