#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

    Settings settings() const;

private slots:
    void on_actionExit_triggered();

    void on_actionHelp_triggered();

    void on_openPort_btn_clicked();

    void on_Transmit_btn_clicked();

private:
    Ui::MainWindow *ui;
<<<<<<< HEAD
=======
    QSerialPort *serial;
    Settings *p;
    void inicializarCombos();
    void updateSetting();
protected:
    bool sendString(const QString& s);
    bool sendByte(char c, unsigned int delay);

    int m_fd;
>>>>>>> b8c37e2c646ec0dbf5e48153953cebb4d75b775f
};

#endif // MAINWINDOW_H
