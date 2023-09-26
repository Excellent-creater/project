#ifndef FTPSOCKET_H
#define FTPSOCKET_H

#include <QTcpSocket>
#include <QTextBrowser>
#include <QTableWidget>
#include <QByteArray>
#include <QLineEdit>
#include <QDebug>

class FtpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit FtpSocket(const QString& ip="",QTextBrowser* recode=0);
    ~FtpSocket(void);

    // 判断连接是否成功
    bool is_connect(void);
    void send_user(const QString& user);
    bool send_pass(const QString& pass);

    void send_cwd(const QString& path);
    void send_pwd(QLineEdit* src_path);
    void send_list(QTableWidget *tableWidget);
    void send_get(const QString& filename, const QString &path);
    void send_put(const QString& filename,const QString& path_src,const QString& path_loc);

private:
    bool send(const QString& cmd);
    bool recv(const char* status);
    FtpSocket* pasv(void);

signals:

public slots:

private:
    QString ip;
    QTextBrowser* recode;//记录发送和接收的命令
    QByteArray  bufArray;//存储发送指令、接收结果的buf
};

#endif // FTPSOCKET_H
