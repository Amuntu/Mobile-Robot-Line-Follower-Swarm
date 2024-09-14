#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>

#define MAX 55
#define INF 999999

#define pharmacy 46
#define food  48
#define report  22

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setSerialPort();

    void fill_map(); // set the distance between each node and other nodes in the map .
    QString dijkstra(int startnode, int Goal); // send the Start point ,the End point and the number of Item .
    void SendToESP32sMaster(QString x); // Send the path frome start point to End point to Arduino after store it .
    void GetFromESP32sMaster();
private slots:
    void on_pushButton_clicked();

    void on_HomePushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort serial;
    QString COM ="COM24";
    int BAUDRATE=115200;
     QString DataCommingFromSerial;

    bool MAP1[MAX][MAX]={{0}};
    const int Mission[4]{0,food,pharmacy,report};
    const int Room[9]{39,17,40,18,41,19,42,20,32};

    int Robot_positionNow[3]{1,2,3};
    const int Home[3]{1,2,3};
    char Robot_direction[3]{'U','U','U'};

    QString GO;
    QString Robot_path[3]; // path as charset array .
    char direction;

};

#endif // MAINWINDOW_H
