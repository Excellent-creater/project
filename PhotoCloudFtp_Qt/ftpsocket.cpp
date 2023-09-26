#include "ftpsocket.h"
#include <QColor>
#include <QHostAddress>
#include <QFileInfo>
FtpSocket::FtpSocket(const QString &ip,QTextBrowser* recode):ip(ip),recode(recode){}

FtpSocket::~FtpSocket()
{
    abort();//在对象销毁之前，先正确关闭套接字
}

bool FtpSocket::is_connect()
{
    return recv("220");
}

//开启数据通道
FtpSocket *FtpSocket::pasv()
{
    if(send("PASV\n"))
    {
        if(recv("227"))
        {
            //连接数据通道
            unsigned char port1,port2;
            sscanf(bufArray.data()+27,"%*d,%*d,%*d,%*d,%hhu,%hhu",&port1,&port2);
            unsigned short port = port1*256+port2;

            //创建连接数据通道的FtpSocket对象
            FtpSocket* pasvSocket = new FtpSocket;
            //连接数据通道
            pasvSocket->connectToHost(QHostAddress(ip),port);

            return pasvSocket;
        }
    }
    return NULL;
}

bool FtpSocket::send(const QString &cmd)
{
    long ret = write(cmd.toStdString().c_str(),cmd.size());
    if (0 >= ret){
        //发射 连接断开的信号
        emit disconnect();
        return false;
    }

    //记录要发送的内容到record中
    QString show_cmd = cmd;
    show_cmd[show_cmd.size()-1] = '\0';
    recode->setTextColor(QColor(0,0,0));
    recode->append(show_cmd);
    //这边如果直接返回true的话，可能没有发送完，使用waitForBytesWritten函数可以阻塞等待发送完
    return waitForBytesWritten();
}

bool FtpSocket::recv(const char *status)
{
    //等待接收数据
    waitForReadyRead();
    //接收数据
    bufArray = readAll();
    if (bufArray.size() < 3) {
        qDebug() << "接收到的数据太短:" << bufArray.data();
            return false;
    }
    bufArray[bufArray.size()-2] = '\0';
    recode->setTextColor(QColor(0,128,0));
    //显示数据到recode中
    recode->append(bufArray.data());
    qDebug() << "接收到的数据:" << bufArray.data();
    //解析并检查状态码是否正确
    return ! strncmp(bufArray.data(),status,3);
}

void FtpSocket::send_user(const QString &user)
{
    QString cmd = "USER " + user + "\n";
    if(send(cmd))
        recv("331");
}

bool FtpSocket::send_pass(const QString &pass)
{
    QString cmd = "PASS " + pass + "\n";
    if(send(cmd))
    {
        return recv("230");
    }
    return false;
}

void FtpSocket::send_cwd(const QString &path)
{
    QString cmd = "CWD " + path + "\n";
    if(send(cmd))
        recv("250");
}

void FtpSocket::send_pwd(QLineEdit *src_path)
{
    QString cmd = "PWD\n";
    if(send(cmd))
    {
        if(recv("257"))
        {
            char* temp_ptr = strchr(bufArray.data()+5,'\"');
            if((temp_ptr+1) != nullptr && temp_ptr != nullptr)
            {
                *temp_ptr = '/';
                *(temp_ptr+1) = '\0';
            }
            src_path->setText(bufArray.data()+5);
            return;
        }
    }
}

void FtpSocket::send_list(QTableWidget *tableWidget)
{
    // 开启PASV
    FtpSocket* pasvSocket = pasv();
    if(NULL == pasvSocket)
    {
        qDebug("开启数据通道失败");
        return;
    }

    //发送 LIST
    if(send("LIST -al\n"))
    {
        if(recv("150"))
        {
            //等待服务器数据全部发送完毕
            pasvSocket->waitForReadyRead();
            //再读取数据 否则readAll不阻塞 会快于服务器发送
            bufArray = pasvSocket->readAll();

            char* ret = bufArray.data();
            char name[256],size[20],mode[11],month[5],day[3],time[6];
            char date[14];
            QIcon dpng(":/new/prefix1/dir.png");
            QIcon fpng(":/new/prefix1/file.png");
            QIcon ppng(":/new/prefix1/image.png");

            tableWidget->setRowCount(0);

            //每次list之前先清空table，设置行数为0 相当于清空table
            while(strchr(ret,'\n'))
            {
                sscanf(ret,"%s %*d %*d %*d %s %s %s %s %s",mode,size,month,day,time,name);
                sprintf(date,"%s/%s/%s",month,day,time);
//                qDebug("%s %s %s %s %s %s",mode,size,month,day,time,name);
                //获取现有的行数
                int row = tableWidget->rowCount();
                //在末尾添加一行空白行
                tableWidget->setRowCount(row+1);
                //新加的空白行中添加每列数据
                QString extension = QFileInfo(name).suffix();
                QTableWidgetItem* item0;
                if("jpg" == extension || "jpeg" == extension || "png" == extension || "gif" == extension)
                {
                    //是照片文件
                    item0 = new QTableWidgetItem(ppng,name);
                }else{
                    item0 = new QTableWidgetItem(mode[0] == 'd'? dpng:fpng,name);
                }
                QTableWidgetItem* item1 = new QTableWidgetItem(size);
                QTableWidgetItem* item2 = new QTableWidgetItem(date);
                QTableWidgetItem* item3 = new QTableWidgetItem(mode);

                tableWidget->setItem(row,0,item0);
                tableWidget->setItem(row,1,item1);
                tableWidget->setItem(row,2,item2);
                tableWidget->setItem(row,3 ,item3);

                ret = strchr(ret,'\n')+1;
            }
            recv("226");
        }
    }
    //关闭数据通道 pasv
    delete pasvSocket;
}

void FtpSocket::send_get(const QString &filename,const QString& path)
{
    //开启数据通道
    FtpSocket* pasvSocket = pasv();
    if(NULL == pasvSocket)
    {
        qDebug("开启数据通道失败");
        return;
    }

    QString cmd = "RETR " + filename + "\n";
    if(send(cmd))
    {
        if(recv("150"))
        {
            QString FilePath =path + '/' + filename;
            FILE* fp = fopen(FilePath.toStdString().c_str(),"w");
            if(NULL == fp)
            {
                qDebug("文件权限有误");
                delete pasvSocket;
                return;
            }
            //开始接收数据
            //等待数据全部发送完毕
            while(pasvSocket->waitForReadyRead())
            {
                bufArray = pasvSocket->readAll();
                fwrite(bufArray.data(),1,bufArray.size(),fp);
            }

            //关闭文件和pasv
            fclose(fp);
            delete pasvSocket;
            recv("226");
        }
    }
}

void FtpSocket::send_put(const QString &filename,const QString& path_src,const QString& path_loc)
{
    // 开启数据通道
    FtpSocket *pasvSocket = pasv();
    if (NULL == pasvSocket)
    {
        qDebug("开启数据通道失败");
        return;
    }

    QString cmd = "STOR " + filename + "\r\n";
    if (send(cmd))
    {
        if (recv("150"))
        {
            QString FilePath_src = path_src + '/' + filename;
            QString FilePath_loc = path_loc + '/' + filename;
            QFile inputFile(FilePath_loc);
            if (!inputFile.open(QIODevice::ReadOnly))
            {
                qDebug("文件权限有误");
                delete pasvSocket;
                return;
            }

            // 开始发送数据
            // 读取文件内容并发送
            QByteArray buffer;
            while (!inputFile.atEnd())
            {
                buffer = inputFile.read(1024);
                pasvSocket->write(buffer);
                pasvSocket->waitForBytesWritten();
            }

            // 关闭文件和pasv
            inputFile.close();
            delete pasvSocket;
            recv("226");
        }
    }
}


