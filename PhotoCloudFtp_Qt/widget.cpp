#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QDebug>
#include <QStandardPaths>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->tableWidget_src->setColumnCount(4);
    ui->tableWidget_loc->setColumnCount(4);

    ui->tableWidget_src->setHorizontalHeaderLabels(QStringList()<<"名称"<<"大小"<<"修改时间"<<"属性");
    ui->tableWidget_loc->setHorizontalHeaderLabels(QStringList()<<"名称"<<"大小"<<"修改时间"<<"属性");

    ui->lineEdit_ip->setText("192.168.123.186");
    ui->lineEdit_port->setText("21");
    ui->lineEdit_user->setText("hhj");
    ui->lineEdit_pass->setText("hhj20010901");
}

QString Widget::permissionsToString(QFileDevice::Permissions permissions)
{
    QString result;

    // 文件类型
    if (permissions & QFileDevice::ReadOwner) {
        result += "r";
    }
    else {
        result += "-";
    }

    if (permissions & QFileDevice::WriteOwner) {
        result += "w";
    }
    else {
        result += "-";
    }

    if ((permissions & QFileDevice::ExeOwner) || (permissions & QFileDevice::ReadGroup)) {
        result += "x";
    }
    else {
        result += "-";
    }

    // 组权限
    if (permissions & QFileDevice::ReadGroup) {
        result += "r";
    }
    else {
        result += "-";
    }

    if (permissions & QFileDevice::WriteGroup) {
        result += "w";
    }
    else {
        result += "-";
    }

    if ((permissions & QFileDevice::ExeGroup) || (permissions & QFileDevice::ReadOther)) {
        result += "x";
    }
    else {
        result += "-";
    }

    // 其他用户权限
    if (permissions & QFileDevice::ReadOther) {
        result += "r";
    }
    else {
        result += "-";
    }

    if (permissions & QFileDevice::WriteOther) {
        result += "w";
    }
    else {
        result += "-";
    }

    if (permissions & QFileDevice::ExeOther) {
        result += "x";
    }
    else {
        result += "-";
    }

    return result;
}

void Widget::display_local(QTableWidget *tableWidget, QLineEdit *lineEdit,const QString &path)
{
//    //获取桌面路径
//    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
//    //设置lineEdit为桌面路径
//    lineEdit->setText(desktopPath);
    lineEdit->setText(path);

    //读取桌面文件和文件夹
    QDir dir(path);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    QIcon dpng(":/new/prefix1/dir.png");
    QIcon fpng(":/new/prefix1/file.png");
    QIcon ppng(":/new/prefix1/image.png");

    tableWidget->setRowCount(0);

    for(const QFileInfo &fileInfo : fileInfoList)
    {
        int row = tableWidget->rowCount();
        //添加空白行
        tableWidget->setRowCount(row+1);
        QTableWidgetItem* item0 = new QTableWidgetItem(fileInfo.fileName());
        QString extension = QFileInfo(fileInfo.fileName()).suffix();
        if("jpg" == extension || "jpeg" == extension || "png" == extension || "gif" == extension || "CR2" == extension || "JPG" == extension)
        {
            //是照片文件
            item0->setIcon(ppng);
        }else{
            item0->setIcon(fileInfo.isDir() ? dpng : fpng);
        }

        QTableWidgetItem* item1 = new QTableWidgetItem(QString ::number(fileInfo.size()));
        QTableWidgetItem* item2 = new QTableWidgetItem(fileInfo.lastModified().toString("MMM/dd/yyyy hh:mm"));
        //获取权限信息
        QFileDevice::Permissions permissions = fileInfo.permissions();
        //权限信息转换为字符串
        QString permissionsString = permissionsToString(permissions);
        QTableWidgetItem* item3 = new QTableWidgetItem((fileInfo.isDir() ? 'd' : '-')+permissionsString);

        tableWidget->setItem(row,0,item0);
        tableWidget->setItem(row,1,item1);
        tableWidget->setItem(row,2,item2);
        tableWidget->setItem(row,3,item3);
    }

}

Widget::~Widget()
{
    delete ui;
}

//单击连接按钮
void Widget::on_pushButton_con_clicked()
{
    //创建FtpSocket对象
    ftpSocket = new FtpSocket(ui->lineEdit_ip->text(),ui->textBrowser_record);
    //连接服务器
    ftpSocket->connectToHost(QHostAddress(ui->lineEdit_ip->text()),ui->lineEdit_port->text().toUShort());
    //绑定信号和槽
    connect(ftpSocket,SIGNAL(connected()),this,SLOT(connected_slot()));
    connect(ftpSocket,SIGNAL(disconnected()),this,SLOT(disconnected_slot()));
}

//FTP连接成功
void Widget::connected_slot()
{
    ui->pushButton_con->setText("已连接");
    ui->pushButton_con->setEnabled(false);
    if(ftpSocket->is_connect())
    {
        //ip、port是正确的
        //发送用户名
        ftpSocket->send_user(ui->lineEdit_user->text());
        //发送密码
        if(ftpSocket->send_pass(ui->lineEdit_pass->text()))
        {
            //用户名、密码正确
            //显示服务器当前工作路径
            ftpSocket->send_pwd(ui->lineEdit_src);

            //显示服务器当前路径下的文件详细信息
            ftpSocket->send_list(ui->tableWidget_src);
            QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
            display_local(ui->tableWidget_loc,ui->lineEdit_loc,desktopPath);
            return;
        }
    }
    ui->pushButton_con->setText("连接");
    ui->pushButton_con->setEnabled(true);
    delete ftpSocket;
}

//FTP连接失败、断开连接
void Widget::disconnected_slot()
{
    ui->pushButton_con->setText("连接");
    ui->pushButton_con->setEnabled(true);
    delete ftpSocket;

}

//双击table_src某行 进入目录
void Widget::on_tableWidget_src_doubleClicked(const QModelIndex &index)
{
    //  双击的该行 是目录文件
    if('d' == ui->tableWidget_src->item(index.row(),3)->text().at(0))
    {
        //  cd
        ftpSocket->send_cwd(ui->tableWidget_src->item(index.row(),0)->text());
        //  pwd
        ftpSocket->send_pwd(ui->lineEdit_src);
        //  list
        ftpSocket->send_list(ui->tableWidget_src);
    }
}

//点击下载按钮
void Widget::on_pushButton_get_clicked()
{
    //获取当前table中选中的该行行数
    int row = ui->tableWidget_src->currentRow();
    if('d' != ui->tableWidget_src->item(row,3)->text().at(0))
    {
        QString path = ui->lineEdit_loc->text();
        //选中非目录才能下载
        ftpSocket->send_get(ui->tableWidget_src->item(row,0)->text(),path);
    }
}

//点击上传按钮
void Widget::on_pushButton_put_clicked()
{
    //获取当前table中选中的该行行数
    int row = ui->tableWidget_loc->currentRow();
    if('d' != ui->tableWidget_loc->item(row,3)->text().at(0))
    {
        QString path_src = ui->lineEdit_src->text();
        QString path_loc = ui->lineEdit_loc->text();
        //选中非目录才能下载
        ftpSocket->send_put(ui->tableWidget_loc->item(row,0)->text(),path_src,path_loc);
    }
}

//双击table_loc某行，进入目录
void Widget::on_tableWidget_loc_doubleClicked(const QModelIndex &index)
{
    //先判断是否是目录文件
    if('d' == ui->tableWidget_loc->item(index.row(),3)->text().at(0))
    {
        //获取当前目录路径
        QString currentPath = ui->lineEdit_loc->text();
        //获取双击的目录名
        QString dirName = ui->tableWidget_loc->item(index.row(),0)->text();
        //进行拼接成新的路径
        QString newPath = QDir(currentPath).filePath(dirName);

        //重新显示新目录下文件和和文件夹
        display_local(ui->tableWidget_loc,ui->lineEdit_loc,newPath);
    }
}


void Widget::on_pushButton_back_clicked()
{
    //获取当前路径
    QString currentPath = ui->lineEdit_loc->text();
    std::string path = currentPath.toStdString();
    //查找最后一个斜杠的位置
    size_t lastSlashIndex = path.find_last_of('/');
    if(lastSlashIndex != std::string::npos){

        std::string newpath = path.substr(0,lastSlashIndex);
        QString newPath = QString::fromStdString(newpath);
        display_local(ui->tableWidget_loc,ui->lineEdit_loc,newPath);
    }
}

//刷新页面
void Widget::on_pushButton_flush_clicked()
{
    //服务器页面刷新
    ftpSocket->send_list(ui->tableWidget_src);
    //本地刷新
    ui->tableWidget_loc->setRowCount(0);
    QString currentPath= ui->lineEdit_loc->text();
    display_local(ui->tableWidget_loc,ui->lineEdit_loc,currentPath);
}

