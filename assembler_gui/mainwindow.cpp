#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include "sic.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_3->hide();
    ui->pushButton_4->hide();

    QFile file("sourcecode.txt");
    if(!file.open(QIODevice::WriteOnly))
        QMessageBox::information(0,"info",file.errorString());
    QTextStream out(&file);

   // QString str=ui->textEdit->toPlainText();

    out<<"";

    file.flush();
    file.close();

    QFile file1("memory.txt");
    if(!file1.open(QIODevice::WriteOnly))
        QMessageBox::information(0,"info",file1.errorString());
    QTextStream out1(&file1);

   // QString str=ui->textEdit->toPlainText();

    out1<<"";

    file1.flush();
    file1.close();

    QFile file2("register.txt");
    if(!file2.open(QIODevice::WriteOnly))
        QMessageBox::information(0,"info",file2.errorString());
    QTextStream out2(&file2);

   // QString str=ui->textEdit->toPlainText();

    out2<<"";

    file2.flush();
    file2.close();

    //QMessageBox::information(0,"info","Enter source code in source code field and enter SUBMIT button");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

  ui->textEdit->setText(" ");
  ui->pushButton_2->show();
  ui->pushButton_3->hide();

  /* QFile file("/home/rubel/share.txt");
   if(!file.open(QIODevice::ReadOnly))
       QMessageBox::information(0,"info",file.errorString());
   QTextStream in(&file);
   ui->textBrowser->setText(in.readAll());
   file.close();


   file("/home/rubel/share12.txt");
   if(!file.open(QIODevice::Append))
       QMessageBox::information(0,"info",file.errorString());
   QTextStream out(&file);

   for(int i=0;i<10;i++)
       out<< i<<"\n";
   //ui->textBrowser->setText(in.readAll());
   file.flush();
   file.close();*/

  /* QString str = ui->textBrowser->toPlainText();

   QFile file1("/home/rubel/share12.txt");
   if(!file1.open(QIODevice::WriteOnly))
       QMessageBox::information(0,"info",file1.errorString());
   QTextStream out(&file1);
   out<< str;
   //ui->textBrowser->setText(in.readAll());
   file1.flush();
   file1.close();*/

}

void MainWindow::on_pushButton_3_clicked()
{

    QFile file1("input.txt");
    if(!file1.open(QIODevice::WriteOnly))
        QMessageBox::information(0,"info",file1.errorString());
    QTextStream out(&file1);

    QString str=ui->textEdit->toPlainText();

    out<<str;

    file1.flush();
    file1.close();

    ui->pushButton_4->show();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();

    QMessageBox::information(0,"Info","Enter EXECUTE button to execute it");

}

void MainWindow::on_pushButton_2_clicked()
{
    QFile file("sourcecode.txt");
    if(!file.open(QIODevice::WriteOnly))
        QMessageBox::information(0,"info",file.errorString());
    QTextStream out(&file);

    QString str=ui->textEdit->toPlainText();

    out<<str;

    file.flush();
    file.close();
    //ui->pushButton_3->show();
    ui->pushButton_2->hide();
    QMessageBox::information(0,"Info","Enter input (if necessary) in source code field and press INPUT button");
    ui->pushButton_3->show();
    ui->textEdit->setText("");

   // ui->pushButton_2->hide();

}


void MainWindow::on_pushButton_5_clicked()
{

     QFile file("memory.txt");
     if(!file.open(QIODevice::ReadOnly))
         QMessageBox::information(0,"info",file.errorString());
     QTextStream in(&file);
     ui->textBrowser->setText(in.readAll());
     file.close();

}

void MainWindow::on_pushButton_6_clicked()
{
    QFile file("register.txt");
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"info",file.errorString());
    QTextStream in(&file);
    ui->textBrowser->setText(in.readAll());
    file.close();
}

void MainWindow::on_pushButton_4_clicked()
{

    ui->pushButton_4->hide();
    ui->pushButton->hide();

    Sic sc;
    sc.init();
    sc.load_memory();
    sc.OPTAB();
    QMessageBox::information(0,"Info","Pass 1 is in progress");
    sc.PC=sc.pass_1();
    sc.delay();
    QMessageBox::information(0,"Info","Pass 1 is successfully completed");
    sc.allocate_memory();
    sc.delay();
    QMessageBox::information(0,"Info","Pass 2 is in progress");
    sc.pass_2();
    sc.delay();
    QMessageBox::information(0,"Info","Pass 2 is successfully completed");
    sc.delay();
    QMessageBox::information(0,"Info","It is ready for execution");
    sc.execute();
    sc.delay();
    //int sz=sc.PC.size();
    //for(int i=sz;i<6;i++)
      //  sc.PC='0'+sc.PC;
    QMessageBox::information(0,"Info","Execution is successfully completed");
    ui->pushButton_2->show();
    ui->pushButton_3->show();
    ui->pushButton->show();
}

void MainWindow::on_pushButton_7_clicked()
{
     QFile file("sourcecode.txt");
     if(!file.open(QIODevice::ReadOnly))
         QMessageBox::information(0,"info",file.errorString());
     QTextStream in(&file);
     ui->textBrowser->setText(in.readAll());
     file.close();

}

void MainWindow::on_pushButton_8_clicked()
{
    QFile file("OBJECTFILE.txt");
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"info",file.errorString());
    QTextStream in(&file);
    ui->textBrowser->setText(in.readAll());
    file.close();
}
