#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <budget.h>

#include <QString>
#include <QDebug>
#include <QDialog>
#include <QtCharts>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //win.show();
    win.setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体
    win.show();

    void (Budget::*saveSignal)(double)=&Budget::mySignal;
    connect(&win,saveSignal,this,&MainWindow::save_monthMoney);
    connect(&win,&Budget::CANCEL,this,&MainWindow::cancel);

    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime()); // 获取当前日期
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd");// 以年月日显示
    QRegExp double_rx("100|([0-9]{0,6}[.][0-9]{1,3})");// 限定输入值是小于100000的浮点数或整数
    ui->lineEdit_money->setValidator(new QRegExpValidator(double_rx,this));

    ui->radioButton_out->setChecked(true);// 默认支出

//    ui->tableWidget->setColumnCount(4);
//    ui->tableWidget->setRowCount(4);

    ui->lineEdit_money->setPlaceholderText("输入金额");
    ui->lineEdit_type->setPlaceholderText("此处新添类型");
    //ui->widget_piechart->resize(500,500);

    //鼠标右键动作
//    action = new QAction(this);
//    actions();
//    connect(action, SIGNAL(triggered(bool)), this, SLOT(actionClick()));

    ui->tableWidget->horizontalHeader()->setMidLineWidth(520);
    ui->tableWidget_in->horizontalHeader()->setMidLineWidth(520);
    MainWindow::resize(578,622);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::actions()
//{
//    menu = new QMenu(); //创建菜单项
//    action->setText(QString("action"));
//}

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
    MainWindow::move(650,220);
    MainWindow::resize(578,622);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_detail_clicked()
{
    MainWindow::move(350,220);
    MainWindow::resize(1100,622);
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_statistics_clicked()
{
    MainWindow::move(650,220);
    MainWindow::resize(578,622);
    double moneySum=0;
    struct M{
        double moneyType=0;
        QString TYPE;
        double percentage;
    }m[100];
    for(int i=0;i<ii;i++){
        bill[i].moneyplus=false;
    }
    for(int i=0;i<ii;i++){
        if(bill[i].money.toDouble()<0){
            moneySum+=bill[i].money.toDouble();
        }
    }
    //qDebug()<<"总金额"<<moneySum;
    int k=0;

    for(int i=0;i<ii;i++){
        if(bill[i].moneyplus==false&&bill[i].money.toDouble()<0){
            m[k].TYPE=bill[i].type;
            m[k].moneyType+=bill[i].money.toDouble();
            bill[i].moneyplus=true;
            for(int j=i+1;j<ii;j++){
                if(bill[i].type==bill[j].type&&bill[j].money.toDouble()<0){
                    m[k].moneyType+=bill[j].money.toDouble();
                    bill[j].moneyplus=true;
                }
            }
            //qDebug()<<m[k].moneyType;
            k++;
        }
    }

    for(int i=0;i<k;i++){

        m[i].percentage=m[i].moneyType/moneySum;
        //qDebug()<<m[i].TYPE<<m[i].moneyType<<m[i].percentage;
    }

    M temp;
    for(int i=0;i<ii-1;i++){
        for(int j=i+1;j<ii;j++){
            if(m[i].percentage<m[j].percentage){
                temp=m[i];
                m[i]=m[j];
                m[j]=temp;
            }
        }
    }

    ui->stackedWidget->setCurrentIndex(2);
    QPieSeries *series=new QPieSeries();

    double maxPer=0;
    for(int i=0;i<k;i++){
        maxPer+=m[i].percentage;
        //qDebug()<<maxPer;
        //if(maxPer<0.97){
            series->append(m[i].TYPE, m[i].percentage); //（名字，百分比（10=100%））
        //}
//        else{
//            series->append("其他", (1-maxPer));
//            break;
//        }

    }

    series->setLabelsVisible();
    QChart *chart=new QChart();
    chart->addSeries(series);
    chart->setTitle("支出统计图");               //标题
    //chart->legend()->hide();

    QChartView *chartview=new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    chartview->setParent(ui->pageStatistisc);   //指定父对象
    chartview->resize(578,350);                 //设置大小
    chartview->show();

//    for(int i=0;i<k;i++){
//        qDebug()<<m[i].TYPE<<m[i].moneyType<<m[i].percentage;
//    }

    ui->label->setText("支出排行：");

    QString str=m[0].TYPE;
    QString number=QString::number(m[0].percentage, 'f', 5);
    double NUM=number.toDouble()*100;
    str.append("        ");
    str += QString::number(m[0].moneyType, 'f', 2);
    number = QString::number(NUM, 'f', 2);
    number.append("%");
    ui->label_2->setText(str);
    ui->label_6->setText(number);

    str=m[1].TYPE;
    number=QString::number(m[1].percentage, 'f', 5);
    NUM=number.toDouble()*100;
    str.append("        ");
    str += QString::number(m[1].moneyType, 'f', 2);
    number = QString::number(NUM, 'f', 2);
    number.append("%");
    ui->label_3->setText(str);
    ui->label_7->setText(number);

    str=m[2].TYPE;
    number=QString::number(m[2].percentage, 'f', 5);
    NUM=number.toDouble()*100;
    str.append("        ");
    str += QString::number(m[2].moneyType, 'f', 2);
    number = QString::number(NUM, 'f', 2);
    number.append("%");
    ui->label_4->setText(str);
    ui->label_8->setText(number);

    str=m[3].TYPE;
    number=QString::number(m[3].percentage, 'f', 5);
    NUM=number.toDouble()*100;
    str.append("        ");
    str += QString::number(m[3].moneyType, 'f', 2);
    number = QString::number(NUM, 'f', 2);
    number.append("%");
    ui->label_5->setText(str);
    ui->label_9->setText(number);

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
    int j=6;
    for(;j<typeNum;j++){
        if(j==ui->comboBox->currentIndex()) break;
    }
    if(typeNum>6){
        ui->comboBox->removeItem(j);
        typeNum--;
    }
}

static int row=0;
void MainWindow::on_book_clicked()
{

    getMoney();
    if(bill[ii].money!=""){

    int j=0;
    QDate date = ui->dateTimeEdit->date();
    bill[ii].date=ui->dateTimeEdit->date();
    QString Date=date.toString("yyyy-MM-dd");
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(row,j,new  QTableWidgetItem(Date));
    ui->tableWidget->item(row,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    j++;

    if(ui->radioButton_out->isChecked())
    {
        bill[ii].money.prepend("-");//被选中
        QString type=ui->comboBox->currentText();
        bill[ii].type=ui->comboBox->currentText();
        ui->tableWidget->setItem(row,j,new  QTableWidgetItem(type));
    }
    else
    {
        bill[ii].money.prepend("+");//取消选中
        bill[ii].type="收入";
        ui->tableWidget->setItem(row,j,new  QTableWidgetItem("收入"));
    }
    ui->tableWidget->item(row,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    j++;

    ui->tableWidget->setItem(row,j,new  QTableWidgetItem(bill[ii].money));
    ui->tableWidget->item(row,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    j++;

    QString text=ui->textEdit->toPlainText();
    bill[ii].remarks=text;
    ui->tableWidget->setItem(row,j,new  QTableWidgetItem(text));
    ui->tableWidget->item(row,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    row++;

    ui->lineEdit_money->clear();
    ui->lineEdit_type->clear();
    ui->textEdit->clear();

    QMessageBox box;
    box.setStyleSheet("QLabel{"
                          "min-width:200px;"
                          "min-height:50px; "
                          "font-size:16px;"
                          "}");
    box.setText(QString::fromLocal8Bit("Record Successfully!"));
    box.setWindowTitle(QString::fromLocal8Bit("Right"));
    box.setButtonText(QMessageBox::Ok , QString::fromLocal8Bit("Yes"));

    box.exec();
    }
    //show_bill();
    ii++;
}

//debug
//void MainWindow::show_bill()
//{
//    qDebug()<<bill[ii].date;
//    qDebug()<<bill[ii].money;
//    qDebug()<<bill[ii].type;
//}

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
    ui->tableWidget->clearContents();
    int p=0;
    for(int r=0;r<ii;r++){
        int j=0;
        QDate date = bill[p].date;
        QString Date=date.toString("yyyy-MM-dd");
        //ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(p,j,new  QTableWidgetItem(Date));
        ui->tableWidget->item(p,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        j++;

        QString type=bill[p].type;
        ui->tableWidget->setItem(p,j,new  QTableWidgetItem(type));
        ui->tableWidget->item(p,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        j++;

        QString money=bill[p].money;
        ui->tableWidget->setItem(p,j,new  QTableWidgetItem(money));
        ui->tableWidget->item(p,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        j++;

        QString remarks=bill[p].remarks;
        ui->tableWidget->setItem(p,j,new  QTableWidgetItem(remarks));
        ui->tableWidget->item(p,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        p++;
    }
}



void MainWindow::on_Time_Sort_clicked()
{
    Bill temp;
    for(int j=0;j<ii;j++){
        for(int r=j+1;r<ii;r++){
            if(bill[j].date.year()<bill[r].date.year()){
                temp=bill[j];
                bill[j]=bill[r];
                bill[r]=temp;
            }
            else if(bill[j].date.year()==bill[r].date.year()&&bill[j].date.month()<bill[r].date.month()){
                temp=bill[j];
                bill[j]=bill[r];
                bill[r]=temp;
            }
            else if(bill[j].date.year()==bill[r].date.year()&&bill[j].date.month()==bill[r].date.month()&&bill[j].date.day()<bill[r].date.day()){
                temp=bill[j];
                bill[j]=bill[r];
                bill[r]=temp;
            }
        }
    }

    ui->tableWidget->clearContents();
    int p=0;
    for(int r=0;r<ii;r++){
        int j=0;

        QDate date = bill[r].date;
        QString Date=date.toString("yyyy-MM-dd");
        //ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(p,j,new  QTableWidgetItem(Date));
        ui->tableWidget->item(p,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        j++;

        QString type=bill[p].type;
        ui->tableWidget->setItem(p,j,new  QTableWidgetItem(type));
        ui->tableWidget->item(p,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        j++;

        QString money=bill[p].money;
        ui->tableWidget->setItem(p,j,new  QTableWidgetItem(money));
        ui->tableWidget->item(p,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        j++;

        QString remarks=bill[p].remarks;
        ui->tableWidget->setItem(p,j,new  QTableWidgetItem(remarks));
        ui->tableWidget->item(p,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        p++;
    }
}

void MainWindow::save_monthMoney(double num)
{
    win.hide();
    this->show();
    budgetMoney=num;
    //qDebug()<<budgetMoney;
}

void MainWindow::cancel()
{
    QApplication *app;
    app->exit(0);
}
