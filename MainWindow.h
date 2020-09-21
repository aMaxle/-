#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QString>
#include <QDialog>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QString InType;
    struct Bill{
        QString money;
        QString type;
        QDate date;
    }bill[100];
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QDialog dlg;

    void getMoney();
    void getType();
    void getInType();
    void getbill();
    void DouStr();

private slots:
    void on_book_2_clicked();

    void on_detail_clicked();

    void on_statistics_clicked();

    void on_button_addtype_clicked();

    void on_book_clicked();

    void show_bill();

    void on_clear_onetype_clicked();

    void on_Price_Sort_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
