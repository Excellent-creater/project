#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "ftpsocket.h"
#include <QDir>
#include <QFileInfoList>
#include <QTableWidgetItem>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void display_local(QTableWidget* tableWidget, QLineEdit* lineEdi, const QString &path);
    QString permissionsToString(QFileDevice::Permissions permissions);
    ~Widget();

private slots:
    void on_pushButton_con_clicked();
    void connected_slot(void);
    void disconnected_slot(void);
    void on_tableWidget_src_doubleClicked(const QModelIndex &index);

    void on_pushButton_get_clicked();

    void on_pushButton_put_clicked();

    void on_tableWidget_loc_doubleClicked(const QModelIndex &index);

    void on_pushButton_back_clicked();

    void on_pushButton_flush_clicked();

private:
    Ui::Widget *ui;
    FtpSocket* ftpSocket;
};
#endif // WIDGET_H
