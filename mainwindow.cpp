#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QList>

#include <QThread>

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};

void millisleep(int ms)
{
   if (ms>0)
   {
      Sleeper::msleep(ms);
   }
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->Port_cbox->addItem(info.portName());
    }

    this->setWindowFlags(Qt::Window|Qt::MSWindowsFixedSizeDialogHint);//Para no poder maximizar la ventana y que el tamaño sea fijo


    serial = new QSerialPort();
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    p = new Settings();
    inicializarCombos();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{

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

    ui->Baud_cbox->addItem(QStringLiteral("9600"), QSerialPort::Baud1200);
    ui->Baud_cbox->addItem(QStringLiteral("19200"), QSerialPort::Baud2400);
    ui->Baud_cbox->addItem(QStringLiteral("38400"), QSerialPort::Baud4800);
    ui->Baud_cbox->addItem(QStringLiteral("115200"), QSerialPort::Baud9600);
    ui->Baud_cbox->addItem(QStringLiteral("9600"), QSerialPort::Baud19200);
    ui->Baud_cbox->addItem(QStringLiteral("19200"), QSerialPort::Baud38400);
    ui->Baud_cbox->addItem(QStringLiteral("38400"), QSerialPort::Baud57600);
    ui->Baud_cbox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->Baud_cbox->setCurrentIndex(7);

    ui->Data_cbox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->Data_cbox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->Data_cbox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->Data_cbox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->Data_cbox->setCurrentIndex(3);

    ui->Parity_cbox->addItem(tr("None"), QSerialPort::NoParity);
    ui->Parity_cbox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->Parity_cbox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->Parity_cbox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->Parity_cbox->addItem(tr("Space"), QSerialPort::SpaceParity);
    ui->Parity_cbox->setCurrentIndex(0);

    ui->Stop_cbox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->Stop_cbox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
    ui->Stop_cbox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
    ui->Stop_cbox->setCurrentIndex(0);

}


void MainWindow::on_openPort_btn_clicked()
{


    p->dataBits = static_cast<QSerialPort::DataBits>(
                    ui->Data_cbox->itemData(ui->Data_cbox->currentIndex()).toInt());
    p->parity = static_cast<QSerialPort::Parity>(
                ui->Parity_cbox->itemData(ui->Parity_cbox->currentIndex()).toInt());
    p->stopBits = static_cast<QSerialPort::StopBits>(
                ui->Stop_cbox->itemData(ui->Stop_cbox->currentIndex()).toInt());


    serial->setPortName(ui->Port_cbox->currentText());
    serial->setBaudRate(qint32 (ui->Baud_cbox->currentText().toInt()));
    p->stringBaudRate = ui->Baud_cbox->currentText();
    serial->setDataBits(p->dataBits);
    p->stringDataBits = ui->Data_cbox->currentText();
    serial->setParity(p->parity);
    p->stringParity = ui->Parity_cbox->currentText();
    serial->setStopBits(p->stopBits);
    p->stringStopBits = ui->Stop_cbox->currentText();


    if (serial->isOpen()) {
        ui->openPort_btn->setText("Open Port");
        ui->statusBar->showMessage(tr("Close Port"));
        serial->close();
    } else {
        if (serial->open(QIODevice::ReadWrite)) {
            ui->openPort_btn->setText("Close Port");
                ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                           .arg(serial->portName()).arg(p->stringBaudRate).arg(p->stringDataBits)
                                           .arg(p->stringParity).arg(p->stopBits).arg("hola"));

        } else {
            QMessageBox::critical(this, tr("Error"), serial->errorString());

            ui->statusBar->showMessage(tr("Open error"));
        }
    }

}

void MainWindow::on_Transmit_btn_clicked()
{

    QString cmd = ui->TxData_inpTxt->text().trimmed();
    ui->TxData_inpTxt->clear();
    ui->textBrowser->append("TX: " + cmd);

    sendString(cmd);


}

bool MainWindow::sendString(const QString &s)
{
    unsigned int charDelay=1;

    QString hex = s;
    hex.remove(QRegExp("\\s"));
    if ((hex.startsWith("0x")) || (hex.startsWith("0X")))
    {
       hex=hex.mid(2);
    }

    if (hex.length()%2 != 0)
    {
       hex="0"+hex;
    }

    for (int i=0; i<hex.length()/2; i++)
    {
       QString nextByte=hex.mid(i*2, 2);
       bool ok=true;
       nextByte.toUInt(&ok, 16);
       if (!ok)
       {
          QMessageBox::information(this, tr("Invalid hex characters"), tr("The input string contains invalid hex characters: 0x%1").arg(nextByte));
          return false;
       }
    }

    for (int i=0; i<hex.length()/2; i++)
    {
       QString nextByte=hex.mid(i*2, 2);
       unsigned int byte=nextByte.toUInt(0, 16);
       sendByte(byte & 0xff, charDelay);
       // fprintf(stderr, " 0x%x d:%d ", byte & 0xff, charDelay);
    }
    return true;

}

bool MainWindow::sendByte(char c, unsigned int delay)
{

    int res=serial->write(&c, 1);
 //   std::cerr<<"wrote "<<(unsigned int)(c)<<std::endl;
    if (res<1)
    {
       //std::cerr<<"write returned "<<res<<" errno: "<<errno<<std::endl;
       //perror("write\n");
        qDebug() << false;
       return false;
    }
    millisleep(delay);
    qDebug() << true;
    return true;

}

