#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime()); // 获取当前日期
    ui->dateTimeEdit->setDisplayFormat("yyyy/MM/dd");// 以年月日显示

    QRegExp double_rx("100|([0-9]{0,6}[.][0-9]{1,3})");// 限定输入值是小于100000的浮点数或整数
    ui->lineEdit_money->setValidator(new QRegExpValidator(double_rx,this));

    ui->radioButton_out->setChecked(true);// 默认支出
}

MainWindow::~MainWindow()
{
    delete ui;
}

static int ii=0;
void MainWindow::getMoney()
{
    bill[ii].money=ui->lineEdit_money->text();
}

void MainWindow::getType()
{
    bill[ii].type=ui->lineEdit_type->text();
}

void MainWindow::getInType()
{
    InType=ui->lineEdit_type->text();
}

void MainWindow::on_book_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_detail_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_statistics_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

static int typeNum=6;

void MainWindow::on_button_addtype_clicked()
{
    getInType();
    if(InType!=""){
        ui->comboBox->addItem(InType);
        ui->lineEdit_type->clear();
        ui->comboBox->setCurrentIndex(typeNum);
        typeNum++;
    }
}

void MainWindow::on_clear_onetype_clicked()
{
    int j=0;
    for(;j<typeNum;j++){
        if(j==ui->comboBox->currentIndex()) break;
    }
    ui->comboBox->removeItem(j);
    typeNum--;
}

static int i=0;
void MainWindow::on_book_clicked()
{
    getMoney();
    if(bill[ii].money!=""){

    int j=0;
    QDate date = ui->dateTimeEdit->date();
    bill[ii].date=ui->dateTimeEdit->date();
    QString Date=date.toString("yyyy-MM-dd");
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(i,j,new  QTableWidgetItem(Date));
    ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    j++;

    QString type=ui->comboBox->currentText();
    bill[ii].type=ui->comboBox->currentText();
    ui->tableWidget->setItem(i,j,new  QTableWidgetItem(type));
    ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    j++;

    if(ui->radioButton_out->isChecked())
    {
        bill[ii].money.prepend("-");//被选中
    }
    else
    {
        bill[ii].money.prepend("+");//取消选中
    }
    ui->tableWidget->setItem(i,j,new  QTableWidgetItem(bill[ii].money));
    ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    j++;

    QString text=ui->textEdit->toPlainText();
    ui->tableWidget->setItem(i,j,new  QTableWidgetItem(text));
    ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    i++;

    ui->lineEdit_money->clear();
    ui->lineEdit_type->clear();
    ui->textEdit->clear();

    QMessageBox box;
    box.setStyleSheet("QLabel{"
                          "min-width:200px;"
                          "min-height:50px; "
                          "font-size:16px;"
                          "}");
    box.setText(QString::fromLocal8Bit("记录成功！"));
    box.setWindowTitle(QString::fromLocal8Bit("成功"));
    box.setButtonText(QMessageBox::Ok , QString::fromLocal8Bit("确定"));

    box.exec();
    }
    show_bill();
    ii++;
}

void MainWindow::show_bill()
{
    qDebug()<<bill[ii].date;
    qDebug()<<bill[ii].money;
    qDebug()<<bill[ii].type;
}

void MainWindow::on_Price_Sort_clicked()
{
    double val1=0;
    double val2=0;
    Bill temp;
    for(int j=0;j<ii;j++){
        for(int r=j+1;r<ii;r++){
            val1=bill[j].money.toDouble();
            val2=bill[r].money.toDouble();
            if(val1>val2){
                temp=bill[j];
                bill[j]=bill[r];
                bill[r]=temp;
            }
        }
    }
//    for(int j=0;j<ii;j++){
//        qDebug()<<bill[j].money;
//    }
    ui->tableWidget->clear();

//    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
//    ui->tableWidget->setItem(0,0,new  QTableWidgetItem("日期"));
//    ui->tableWidget->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

//    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
//    ui->tableWidget->setItem(0,1,new  QTableWidgetItem("类型"));
//    ui->tableWidget->item(0,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

//    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
//    ui->tableWidget->setItem(0,2,new  QTableWidgetItem("金额"));
//    ui->tableWidget->item(0,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

//    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
//    ui->tableWidget->setItem(0,3,new  QTableWidgetItem("备注"));
//    ui->tableWidget->item(0,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    int p=0;
    for(int r=0;r<ii;r++){
        int j=0;

        QDate date = bill[r].date;
        QString Date=date.toString("yyyy-MM-dd");
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(p,j,new  QTableWidgetItem(Date));
        ui->tableWidget->item(p,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        j++;

        QString type=bill[p].type;
        ui->tableWidget->setItem(p,j,new  QTableWidgetItem(type));
        ui->tableWidget->item(p,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        j++;

//        if(ui->radioButton_out->isChecked())
//        {
//            bill[ii].money.prepend("-");//被选中
//        }
//        else
//        {
//            bill[ii].money.prepend("+");//取消选中
//        }
        QString money=bill[p].money;
        ui->tableWidget->setItem(p,j,new  QTableWidgetItem(money));
        ui->tableWidget->item(p,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        j++;

//        QString text=ui->textEdit->toPlainText();
//        ui->tableWidget->setItem(r,j,new  QTableWidgetItem(text));
//        ui->tableWidget->item(r,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        p++;
    }

}
