//mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "skiplist.h"
#include "skiplist_test.h"

#include <QMessageBox>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <set>

//#define reorg

using namespace std;

ifstream inputFile;
ofstream outputFile,testInputFile,testOutputFile;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startButton_clicked()
{
    if(ui->operatorBox->currentText()=="数据集验证")
    {
        if(ui->sourceFileLine->text()=="")
            QMessageBox::information(this,"提示", "请输入数据集源文件名");
        else if(ui->targetFileLine->text()=="")
            QMessageBox::information(this,"提示", "请输入目标文件名");
        else
        {
            //接收源文件名和目标文件名
            QString qStr_1=ui->sourceFileLine->text();
            QByteArray byteArray_1=qStr_1.toUtf8();
            char *ch_1=byteArray_1.data();
            QString qStr_2=ui->targetFileLine->text();
            QByteArray byteArray_2=qStr_2.toUtf8();
            char *ch_2=byteArray_2.data();
            //qDebug()<<ch_1<<ch_2;

            //开始验证
            inputFile.open(ch_1);
            outputFile.open(ch_2);

            if(!inputFile)
            {
                QMessageBox::information(this,"提示","数据集源文件打开失败");
                inputFile.close();
                outputFile.close();
                return ;
            }

            int n,m,op,num;
            inputFile>>n>>m;
            skipList<int> SL(1e9,m+n,0.5);
            SL.initialize(m);

            n-=1;
            while(n--)
            {
                inputFile>>op;
                if(op==1)
                {
                    inputFile>>num;
                    int *p=SL.find(num);
                    if(p!=NULL)
                        outputFile<<"YES"<<endl;
                    else
                        outputFile<<"NO"<<endl;
                }
                else if(op==2)
                {
                    inputFile>>num;
                    SL.insert(num);
                }
                else if(op==3)
                {
                    inputFile>>num;
                    SL.erase(num);
                }
                else if(op==4)
                {
                    SL.eraseMin();
                }
                else if(op==5)
                {
                    SL.eraseMax();
                }
                else if(op==6)
                {
                    SL.output();
                }
            }
            inputFile.close();
            outputFile.close();
            QMessageBox::information(this,"提示", "验证完成，请查看结果");
        }
    }
    else if(ui->operatorBox->currentText()=="时间复杂度分析")
    {
        if(ui->spinBox_4->value() < ui->spinBox->value())
        {
            QMessageBox::information(this,"提示", "随机数最大值不可小于N值");
        }
        else
        {
            //定义计数器
            int initializeTimes=0,initializeSteps=0;                                //初始化的元素个数 初始化的步次
            int findOperatorTimes=0,successfulFindTimes=0,failedFindTimes=0;        //查找操作的次数 查找成功的次数 查找失败的次数
            int findSteps=0,successfulFindSteps=0,failedFindSteps=0;                //查找操作的步次 查找成功的步次 查找失败的步次
            int insertOperatorTimes=0,successfulInsertTimes=0,failedInsertTimes=0;  //插入操作的次数 插入成功的次数 插入失败的次数
            int insertSteps=0,successfulInsertSteps=0,failedInsertSteps=0;          //插入操作的步次 插入成功的步次 插入失败的步次
            int deleteOperatorTimes=0,successfulDeleteTimes=0,failedDeleteTimes=0;  //删除操作的次数 删除成功的次数 删除失败的次数
            int deleteSteps=0,successfulDeleteSteps=0,failedDeleteSteps=0;          //删除操作的步次 删除成功的步次 删除失败的步次
#ifdef reorg
            int reorganizeTimes=0;
#endif


            //文件
            testInputFile.open("input.txt");
            testOutputFile.open("output.txt");
            ofstream analysisFile;
            analysisFile.open("analysis.txt");

            //可变参数
            int N=ui->spinBox->value();
            int M=ui->spinBox_2->value();
            int K=ui->spinBox_3->value();
            int maxNumber=ui->spinBox_4->value();
            testInputFile<<M*K*3+1<<" "<<N<<"\n";

            skipListTest<int> SL(1e9,1e9,0.5);
            //srand(time(NULL));
            srand(1);  //set seed

            //初始化，这里的随机数保证了不能相同
            set<int> s;                 //store the elements that have been inserted successfully
            for(int i=0;i<N;i++)
            {
                int a=rand()%maxNumber+1;
                auto it=s.find(a);
                if(it!=s.end())         //duplicate value
                {
                    i--;
                }
                else                    //non-duplicate value
                {
                    testInputFile<<a<<" ";
                    int steps=0;
                    SL.insert(a,steps);
                    initializeSteps+=steps;
                    s.insert(a);
                }
            }

            testInputFile<<"\n";
            //SL.reorganize();

            //M个操作序列
            for(int i=0;i<M;i++)
            {
                int insertTimes=0,deleteTimes=0,searchTimes=0;

                //每个操作序列包括插入、删除、查找均为K次
                while(insertTimes<K || deleteTimes<K || searchTimes<K)
                {
                    int op=rand()%3+1;
                    if(op==1 && searchTimes<K)   //op:find num
                    {
                        int num=rand()%maxNumber+1;
                        testInputFile<<op<<" "<<num<<"\n";
                        int steps=0;
                        searchTimes++;
                        int *p=SL.find(num,steps);
                        if(p!=NULL)
                        {
                            testOutputFile<<"YES"<<"\n";
                            successfulFindTimes++;  successfulFindSteps+=steps;
                        }
                        else
                        {
                            testOutputFile<<"NO"<<"\n";
                            failedFindTimes++;  failedFindSteps+=steps;
                        }
                        //qDebug()<<SL.getSize()<<"  "<<logf((float)SL.getSize()) / logf((float)2)<<"  "<<1.0*(successfulFindSteps+failedFindSteps)/(successfulFindTimes+failedFindTimes);
                        //整理判断
#ifdef reorg
                        if(1.0*(successfulFindSteps+failedFindSteps)/(successfulFindTimes+failedFindTimes) > logf((float)SL.getSize()) / logf((float)2))
                        {
                            SL.reorganize();
                            reorganizeTimes++;
                        }
#endif
                    }
                    else if(op==2 && insertTimes<K)  //op:insert num
                    {
                        int num=rand()%maxNumber+1;
                        testInputFile<<op<<" "<<num<<"\n";
                        int steps=0;
                        insertTimes++;
                        if(SL.insert(num,steps))    //successful insert
                        {
                            successfulInsertTimes++;    successfulInsertSteps+=steps;
                        }
                        else    //failed insert
                        {
                            failedInsertTimes++;    failedInsertSteps+=steps;
                        }
                        //整理判断
#ifdef reorg
                        if(1.0*(successfulInsertSteps+failedInsertSteps)/(successfulInsertTimes+failedInsertTimes) > logf((float)SL.getSize()) / logf((float)2))
                        {
                            SL.reorganize();
                            reorganizeTimes++;
                        }
#endif
                    }
                    else if(op==3 && deleteTimes<K && SL.getSize()>0)   //op:delete
                    {                                                   //ensure not delete an empty skipList
                        int opp=rand()%3+3;
                        if(opp==3)                  //opp:delete num
                        {
                            int num=rand()%maxNumber+1;
                            testInputFile<<opp<<" "<<num<<"\n";
                            int steps=0;
                            deleteTimes++;
                            if(SL.erase(num,steps)) //successful delete
                            {
                                successfulDeleteTimes++;    successfulDeleteSteps+=steps;
                            }
                            else    //failed delete
                            {
                                failedDeleteTimes++;    failedDeleteSteps+=steps;
                            }
                            //整理判断
#ifdef reorg
                            if(1.0*(successfulDeleteSteps+failedDeleteSteps)/(successfulDeleteTimes+failedDeleteTimes) > logf((float)SL.getSize()) / logf((float)2))
                            {
                                SL.reorganize();
                                reorganizeTimes++;
                            }
#endif
                        }
                        else if(opp==4)             //opp:delete min
                        {
                            testInputFile<<opp<<"\n";
                            deleteTimes++;  successfulDeleteTimes++;
                            SL.eraseMin();
                            //整理判断
#ifdef reorg
                            if(1.0*(successfulDeleteSteps+failedDeleteSteps)/(successfulDeleteTimes+failedDeleteTimes) > logf((float)SL.getSize()) / logf((float)2))
                            {
                                SL.reorganize();
                                reorganizeTimes++;
                            }
#endif
                        }
                        else                        //opp:delete max
                        {
                            testInputFile<<opp<<"\n";
                            int steps=0;
                            deleteTimes++;  successfulDeleteTimes++;
                            SL.eraseMax(steps);
                            successfulDeleteSteps+=steps;
                            //整理判断
#ifdef reorg
                            if(1.0*(successfulDeleteSteps+failedDeleteSteps)/(successfulDeleteTimes+failedDeleteTimes) > logf((float)SL.getSize()) / logf((float)2))
                            {
                                SL.reorganize();
                                reorganizeTimes++;
                            }
#endif
                        }
                    }
                }
            }

            //统计结果
            initializeTimes=N;
            findOperatorTimes=successfulFindTimes+failedFindTimes;
            findSteps=successfulFindSteps+failedFindSteps;
            insertOperatorTimes=successfulInsertTimes+failedInsertTimes;
            insertSteps=successfulInsertSteps+failedInsertSteps;
            deleteOperatorTimes=successfulDeleteTimes+failedDeleteTimes;
            deleteSteps=successfulDeleteSteps+failedDeleteSteps;

            //输出统计结果到文件

            analysisFile<<"         "<<"操作数"<<"         "<<"步次"<<"      "<<"平均步次"<<endl;

            analysisFile<<"初始化插入:"<<initializeTimes<<"      "<<initializeSteps<<"        "<<1.0*initializeSteps/initializeTimes<<endl;

            analysisFile<<"查询操作:"<<findOperatorTimes<<"        "<<findSteps<<"        "<<1.0*findSteps/findOperatorTimes<<endl;
            analysisFile<<"查询成功:"<<successfulFindTimes<<"        "<<successfulFindSteps<<"        "<<1.0*successfulFindSteps/successfulFindTimes<<endl;
            analysisFile<<"查询失败:"<<failedFindTimes<<"        "<<failedFindSteps<<"        "<<1.0*failedFindSteps/failedFindTimes<<endl;

            analysisFile<<"插入操作:"<<insertOperatorTimes<<"        "<<insertSteps<<"      "<<1.0*insertSteps/insertOperatorTimes<<endl;
            analysisFile<<"插入成功:"<<successfulInsertTimes<<"        "<<successfulInsertSteps<<"        "<<1.0*successfulInsertSteps/successfulInsertTimes<<endl;
            analysisFile<<"插入失败:"<<failedInsertTimes<<"        "<<failedInsertSteps<<"      "<<1.0*failedInsertSteps/failedInsertTimes<<endl;

            analysisFile<<"删除操作:"<<deleteOperatorTimes<<"        "<<deleteSteps<<"        "<<1.0*deleteSteps/deleteOperatorTimes<<endl;
            analysisFile<<"删除成功:"<<successfulDeleteTimes<<"        "<<successfulDeleteSteps<<"        "<<1.0*successfulDeleteSteps/successfulDeleteTimes<<endl;
            analysisFile<<"删除失败:"<<failedDeleteTimes<<"        "<<failedDeleteSteps<<"        "<<1.0*failedDeleteSteps/failedDeleteTimes<<endl;

#ifdef reorg
            analysisFile<<"整理次数:"<<reorganizeTimes<<endl;
#endif

            testInputFile.close();
            testOutputFile.close();
            analysisFile.close();

            //输出统计结果到textEdit
            ui->textEdit->clear();
            QString title,title_1="操作数",title_2="步次",title_3="平均步次";
            QString recv="123";
            QString qstr = QString::number(initializeTimes);


            /*
            QTextCursor cursor = ui->textEdit->textCursor();
            QTextBlockFormat textBlockFormat = cursor.blockFormat();
            textBlockFormat.setAlignment(Qt::AlignRight);
            cursor.mergeBlockFormat(textBlockFormat);
            ui->textEdit->setTextCursor(cursor);
            */


            ui->textEdit->append("\t\t");//水平制表
            ui->textEdit->insertHtml("<font color=\"#000000\">" + title_1 +"</font>" + "&nbsp;&nbsp;&nbsp;&nbsp;" +
                                     "<font color=\"#000000\">" + title_2 +"</font>" + "&nbsp;&nbsp;&nbsp;&nbsp;" +
                                     "<font color=\"#000000\">" + title_3 +"</font>" + "&nbsp;&nbsp;&nbsp;&nbsp;" );//显示黑色

            title="初始化插入:";
            ui->textEdit->append("<font color=\"#0000ff\">" + title + "&nbsp;&nbsp;&nbsp;" + QString::number(initializeTimes) + "&nbsp;&nbsp;&nbsp;&nbsp;"
                                 + QString::number(initializeSteps) + "&nbsp;&nbsp;&nbsp;&nbsp;" + QString::number(1.0*initializeSteps/initializeTimes) +"</font>");//显示蓝色

            ui->textEdit->append("");
            title="查询操作:";
            ui->textEdit->append("<font color=\"#0000ff\">" + title + "&nbsp;&nbsp;&nbsp;" + QString::number(findOperatorTimes) + "&nbsp;&nbsp;&nbsp;&nbsp;"
                                 + QString::number(findSteps) + "&nbsp;&nbsp;&nbsp;&nbsp;" + QString::number(1.0*findSteps/findOperatorTimes) +"</font>");//显示蓝色
            title="查询成功:";
            ui->textEdit->append("<font color=\"#00ff00\">" + title + "&nbsp;&nbsp;&nbsp;" + QString::number(successfulFindTimes) + "&nbsp;&nbsp;&nbsp;&nbsp;"
                                 + QString::number(successfulFindSteps) + "&nbsp;&nbsp;&nbsp;&nbsp;" + QString::number(1.0*successfulFindSteps/successfulFindTimes) +"</font>");//显示绿色
            title="查询失败:";
            ui->textEdit->append("<font color=\"#ff0000\">" + title + "&nbsp;&nbsp;&nbsp;" + QString::number(failedFindTimes) + "&nbsp;&nbsp;&nbsp;&nbsp;"
                                 + QString::number(failedFindSteps) + "&nbsp;&nbsp;&nbsp;&nbsp;" + QString::number(1.0*failedFindSteps/failedFindTimes) +"</font>");//显示红色

            ui->textEdit->append("");
            title="插入操作:";
            ui->textEdit->append("<font color=\"#0000ff\">" + title + "&nbsp;&nbsp;&nbsp;" + QString::number(insertOperatorTimes) + "&nbsp;&nbsp;&nbsp;&nbsp;"
                                 + QString::number(insertSteps) + "&nbsp;&nbsp;&nbsp;&nbsp;" + QString::number(1.0*insertSteps/insertOperatorTimes) +"</font>");//显示蓝色
            title="插入成功:";
            ui->textEdit->append("<font color=\"#00ff00\">" + title + "&nbsp;&nbsp;&nbsp;" + QString::number(successfulInsertTimes) + "&nbsp;&nbsp;&nbsp;&nbsp;"
                                 + QString::number(successfulInsertSteps) + "&nbsp;&nbsp;&nbsp;&nbsp;" + QString::number(1.0*successfulInsertSteps/successfulInsertTimes) +"</font>");//显示绿色
            title="插入失败:";
            ui->textEdit->append("<font color=\"#ff0000\">" + title + "&nbsp;&nbsp;&nbsp;" + QString::number(failedInsertTimes) + "&nbsp;&nbsp;&nbsp;&nbsp;"
                                 + QString::number(failedInsertSteps) + "&nbsp;&nbsp;&nbsp;&nbsp;" + QString::number(1.0*failedInsertSteps/failedInsertTimes) +"</font>");//显示红色

            ui->textEdit->append("");
            title="删除操作:";
            ui->textEdit->append("<font color=\"#0000ff\">" + title + "&nbsp;&nbsp;&nbsp;" + QString::number(deleteOperatorTimes) + "&nbsp;&nbsp;&nbsp;&nbsp;"
                                 + QString::number(deleteSteps) + "&nbsp;&nbsp;&nbsp;&nbsp;" + QString::number(1.0*deleteSteps/deleteOperatorTimes) +"</font>");//显示蓝色
            title="删除成功:";
            ui->textEdit->append("<font color=\"#00ff00\">" + title + "&nbsp;&nbsp;&nbsp;" + QString::number(successfulDeleteTimes) + "&nbsp;&nbsp;&nbsp;&nbsp;"
                                 + QString::number(successfulDeleteSteps) + "&nbsp;&nbsp;&nbsp;&nbsp;" + QString::number(1.0*successfulDeleteSteps/successfulDeleteTimes) +"</font>");//显示绿色
            title="删除失败:";
            ui->textEdit->append("<font color=\"#ff0000\">" + title + "&nbsp;&nbsp;&nbsp;" + QString::number(failedDeleteTimes) + "&nbsp;&nbsp;&nbsp;&nbsp;"
                                 + QString::number(failedDeleteSteps) + "&nbsp;&nbsp;&nbsp;&nbsp;" + QString::number(1.0*failedDeleteSteps/failedDeleteTimes) +"</font>");//显示红色
#ifdef reorg
            title="整理次数:";
            ui->textEdit->append("<font color=\"#0000ff\">" + title + QString::number(reorganizeTimes) +"</font>");//显示蓝色
#endif
            //ui->textEdit->append("");
            ui->textEdit->append("<font color=\"#000000\">" + QString("输入数据、输出数据、分析结果可通过下列文件查看:input.txt, output.txt, analysis.txt") +"</font>");//显示黑色
        }
    }
    else
    {
        QMessageBox::information(this,"提示", "请选择一项操作");
    }
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}

void MainWindow::on_exitButton_2_clicked()
{
    ui->textEdit->clear();
}
