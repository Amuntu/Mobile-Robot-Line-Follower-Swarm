#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fill_map();
    setSerialPort();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSerialPort()
{
    serial.setPortName(COM);
    serial.open(QSerialPort::WriteOnly);
    serial.setBaudRate(BAUDRATE);

    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);

    serial.setFlowControl(QSerialPort::NoFlowControl);
}
void MainWindow::fill_map()
{
    for(int i=0;i<MAX;i++)
        for(int j=0;j<MAX;j++)
            MAP1[i][j]=0;
    for (int i=0;i<MAX;i++)
        {
            if (i==1||i==2||i==3||i==17||i==18||i==19||i==20)
            {

                MAP1[i][i+11]=1;
            }
            else if (i==12)
            {
                MAP1[i][i+1]=1;
                MAP1[i][i+11]=1;
                MAP1[i][i-11]=1;
            }
            else if (i==13||i==14||i==23||i==24||i==25||i==26||i==28||i==29||i==30||i==31||i==35)
            {
              MAP1[i][i+1]=1;
              MAP1[i][i-1]=1;
                MAP1[i][i+11]=1;
                MAP1[i][i-11]=1;
            }
            else if (i==32)
            {

                MAP1[i][i-1]=1;

            }
            else if (i==15)
            {
                MAP1[i][i-1]=1;
                MAP1[i][i+11]=1;
            }
            else if (i==22)
            {
                MAP1[i][i+1]=1;

            }
            else if (i==39||i==40||i==41||i==42||i==46||i==48)
            {
                MAP1[i][i-11]=1;
            }
            else if(i==37)
            {
                MAP1[i][i-1]=1;
                MAP1[i][i-11]=1;
                MAP1[i][i+11]=1;
            }
            else if(i==36)
            {
                MAP1[i][i-1]=1;
                MAP1[i][i+1]=1;
                MAP1[i][i-11]=1;
            }
            else if(i==34)
               {
                   MAP1[i][i+1]=1;
                   MAP1[i][i-11]=1;
               }
               else if(i==27)
               {
                    MAP1[i][i-1]=1;
                     MAP1[i][i+1]=1;
               }
        }
}
QString MainWindow::dijkstra(int startnode, int Goal) {
    int n=MAX; // size of map .
        int cost[MAX][MAX] ; // cost is the map with one diffirence .
         int distance[MAX]; // the distance from start to end .
         int pred[MAX];  // store the path nodes .
        bool visited[MAX]; // set the node if the code visited it or not .
        int count,nextnode = 0,i,j;
        int mindistance;

        for(i=0;i<n;i++)
            for(j=0;j<n;j++)
                if(MAP1[i][j]==0)
                    cost[i][j]=INF; // pass over the map and set the ZERO length as INF .
                else
                    cost[i][j]=MAP1[i][j];
        for(i=0;i<n;i++)
        {
            distance[i]=cost[startnode][i];
            pred[i]=startnode;
            visited[i]=0;
        }
        distance[startnode]=0;
        visited[startnode]=1;
        count=1;
        while(count<n-1)
        {
            mindistance=INF;
            for(i=0;i<n;i++)
                if(distance[i]<mindistance&&!visited[i])
                {
                    mindistance=distance[i];
                    nextnode=i;
                }
            visited[nextnode]=1;
            for(i=0;i<n;i++)
                if(!visited[i])
                    if(mindistance+cost[nextnode][i]<distance[i])
                    {
                        distance[i]=mindistance+cost[nextnode][i];
                        pred[i]=nextnode;
                    }
            count++;
        }
        int PathZise=0; // store the size of path array .
        for(i=0;i<n;i++) // first loop to calculate the size of path array .
            if(i!=startnode && i==Goal)
            {
                j=i;
                do
                {
                    j=pred[j];
                    PathZise++;
                }while(j!=startnode);
            }
        int path[PathZise+1]; // path array to store the path nodes .
        char path2[PathZise+1]; // new path array to store the first .
        int c=PathZise;
        PathZise--;
        for(i=0;i<n;i++) // second loop to store the path nodes .
            if(i!=startnode && i==Goal)
            {
                j=i;
                do
                {
                    j=pred[j];
                    path[PathZise]=j;
                    PathZise--;
                }while(j!=startnode);
            }

         path[c]=Goal; // final node is the Goal .
         QString R;
         for(int i=0;i<=c;i++){
             if(path[i]<10)
               R+='.'+QString::number(0)+QString::number(path[i]);
             else
                 R+='.'+QString::number(path[i]);
         }
         qDebug()<<R;
         return R;
}

void MainWindow::SendToESP32sMaster(QString x)
{
    if(serial.isWritable())
        serial.write(x.toStdString().c_str());
}

void MainWindow::GetFromESP32sMaster()
{
   if(serial.isReadable()){
        QByteArray readNum;
        int PositionNow;
        readNum=serial.readAll();
        PositionNow = readNum.toInt();
    }
}

void MainWindow::on_pushButton_clicked()
{
    int n=atoi(ui->RobotComboBox->currentText().toStdString().c_str())-1;
    /*QString Interrupt='I'+QString::number(n+1);
     SendToESP32sMaster(Interrupt);
     while(!serial.isReadable())
          ;
     Interrupt=serial.readAll();
     */
    int newStart, newMission,newEnd;

    newStart=Robot_positionNow[n];
    newMission=Mission[ui->MissonComboBox->currentIndex()];
    newEnd=Room[ui->RoomComboBox->currentIndex()];

    direction=Robot_direction[n];
    if(ui->MissonComboBox->currentIndex()==1||ui->MissonComboBox->currentIndex()==2){
        Robot_path[n]=dijkstra(newStart,newMission);
        Robot_path[n].resize(Robot_path[n].length()-3);
        newStart=newMission;

        Robot_path[n]+= dijkstra(newStart,newEnd);
        Robot_path[n]+=".99_";
        Robot_positionNow[n]=newEnd;
    }
    else if(ui->MissonComboBox->currentIndex()==3)
    {
        Robot_path[n]=dijkstra(newStart,newEnd);
        Robot_path[n].resize(Robot_path[n].length()-3);
        newStart=newEnd;

        Robot_path[n]+= dijkstra(newStart,newMission);
        Robot_path[n]+=".99_";
        Robot_positionNow[n]=newMission;
    }
    Robot_path[n]='_'+QString::number(n+1)+Robot_path[n];
    SendToESP32sMaster(Robot_path[n]);
    qDebug()<<Robot_path[n];
}

void MainWindow::on_HomePushButton_clicked()
{
    int n=ui->HomeComboBox->currentIndex()-2;
    if(n!=-1){
        Robot_path[n]=dijkstra(Robot_positionNow[n],Home[n]);
        Robot_path[n]='_'+QString::number(n+1)+Robot_path[n];
        SendToESP32sMaster(Robot_path[n]);
        qDebug()<<Robot_path[n];
    }
    else {
        QString s;
        for(n=0;n<3;n++){
            Robot_path[n]=dijkstra(Robot_positionNow[n],Home[n]);
            Robot_path[n]='_'+QString::number(n+1)+Robot_path[n];
            s+=Robot_path[n];
        }        
        SendToESP32sMaster(s);
    }
}
