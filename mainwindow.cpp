#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window|Qt::MSWindowsFixedSizeDialogHint);//Para no poder maximizar la ventana y que el tamaño sea fijo

    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    inicializarCombos();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit(0);
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox::about(this, tr("About Simple Terminal"),
                       tr("The <b>Simple Terminal</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

void MainWindow::inicializarCombos()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->Port_cbox->addItem(info.portName());
    }
}

//Combo Box  Baud (75,110,300,1200,2400,4800,9600,19200,38400,57600,115200)
//Combo Box  Data (5,6,7,8,9)
//Combo Box  Parity (None, Odd, Even, Mark, Space)
//Combo Box  Stop(None, One, Two, OnePointFive)
