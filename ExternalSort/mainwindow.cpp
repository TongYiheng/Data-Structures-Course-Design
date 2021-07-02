#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "minLoserTree.h"
#include "minHeap.h"

#include <QMessageBox>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

p parameter;
map<int,int> mp;
ifstream Fin;
ofstream Fout;
int diskAccessTimes,diskReadTimes,diskWriteTimes;
int diskAccessTimesInGenerate,diskReadTimesInGenerate,diskWriteTimesInGenerate;
int diskAccessTimesInMerge,diskReadTimesInMerge,diskWriteTimesInMerge;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::initialize()
{
    //接收源文件名和目标文件名
    QString qStr_1=ui->sourceFileLine->text();
    QByteArray byteArray_1=qStr_1.toUtf8();
    char *ch_1=byteArray_1.data();
    QString qStr_2=ui->targetFileLine->text();
    QByteArray byteArray_2=qStr_2.toUtf8();
    char *ch_2=byteArray_2.data();

    //初始化参数
    Fin.open(ch_1);
    Fout.open(ch_2);
    if(!Fin)
    {
        QMessageBox::information(this,"提示","数据集源文件打开失败");
        Fin.close();
        Fout.close();
        return false;
    }
    Fin>>parameter.total;
    parameter.mergeWays=ui->spinBox_2->value();
    parameter.blockSize=ui->spinBox_3->value();
    parameter.sumSize=ui->spinBox->value();
    parameter.numberOfPlayers=ui->spinBox_4->value();
    parameter.numberOfSequences=0;
    return true;
}

void MainWindow::generate() //构建初始顺串
{
    int inputCnt=0;

    if(parameter.total<=parameter.numberOfPlayers)
    {
        minLoserTree<sortPlayer> MLT;
        //读入前parameter.total个元素
        sortPlayer* iniPlayer=new sortPlayer[parameter.total+1];
        for(int i=1;i<=parameter.total;i++)
        {
            Fin>>iniPlayer[i].key;
            inputCnt++;
            iniPlayer[i].id=1;          //初始化顺串号均为1
        }
        MLT.initialize(iniPlayer,parameter.total);    //建立输者树
        parameter.numberOfSequences=1;

        for(int i=0;i<parameter.total;i++)
        {
            sortPlayer lastWinner,newPlayer;      //上个赢者，新的比赛者

            //将最终赢者lastWinner移入对应的顺串中
            lastWinner=MLT.getWinnerValue();
            string generateFile="sequence"+to_string(lastWinner.id)+".txt";
            ofstream fout(generateFile,ios::app);   //追加模式打开顺串所在的文件
            fout<<lastWinner.key<<' ';
            fout.close();
            parameter.numberOfSequences=max(parameter.numberOfSequences,lastWinner.id);

            //在map中记录每个顺串的大小
            mp[lastWinner.id]++;

            //设置关键字值
            if(!Fin.eof())  //文件没有读完
            {
                Fin>>newPlayer.key;
                inputCnt++;
                if(Fin.eof())   //处理文件末尾的空格
                {
                    inputCnt--;
                    newPlayer.key=INT_MAX;
                    newPlayer.id=INT_MAX;
                }
                else
                {
                    //设置newPlayer的顺串号
                    if(newPlayer.key>=lastWinner.key)
                        newPlayer.id=lastWinner.id;
                    else
                        newPlayer.id=lastWinner.id+1;
                }
            }
            else    //文件读完，设置INT_MAX
            {
                newPlayer.key=INT_MAX;
                newPlayer.id=INT_MAX;
            }
            //输者树重构
            MLT.rePlay(newPlayer);
        }
    }
    else
    {
        minLoserTree<sortPlayer> MLT;
        //读入前numberOfPlayers个元素
        sortPlayer* iniPlayer=new sortPlayer[parameter.numberOfPlayers+1];
        for(int i=1;i<=parameter.numberOfPlayers;i++)
        {
            Fin>>iniPlayer[i].key;
            inputCnt++;
            iniPlayer[i].id=1;          //初始化顺串号均为1
        }
        MLT.initialize(iniPlayer,parameter.numberOfPlayers);    //建立输者树
        parameter.numberOfSequences=1;

        for(int i=0;i<parameter.total;i++)
        {
            sortPlayer lastWinner,newPlayer;      //上个赢者，新的比赛者

            //将最终赢者lastWinner移入对应的顺串中
            lastWinner=MLT.getWinnerValue();
            string generateFile="sequence"+to_string(lastWinner.id)+".txt";
            ofstream fout(generateFile,ios::app);   //追加模式打开顺串所在的文件
            fout<<lastWinner.key<<' ';
            fout.close();
            parameter.numberOfSequences=max(parameter.numberOfSequences,lastWinner.id);

            //在map中记录每个顺串的大小
            mp[lastWinner.id]++;

            //设置关键字值
            if(!Fin.eof())  //文件没有读完
            {
                Fin>>newPlayer.key;
                inputCnt++;
                if(Fin.eof())   //处理文件末尾的空格
                {
                    inputCnt--;
                    newPlayer.key=INT_MAX;
                    newPlayer.id=INT_MAX;
                }
                else
                {
                    //设置newPlayer的顺串号
                    if(newPlayer.key>=lastWinner.key)
                        newPlayer.id=lastWinner.id;
                    else
                        newPlayer.id=lastWinner.id+1;
                }
            }
            else    //文件读完，设置INT_MAX
            {
                newPlayer.key=INT_MAX;
                newPlayer.id=INT_MAX;
            }
            //输者树重构
            MLT.rePlay(newPlayer);
        }
    }
    Fin.close();
    diskReadTimesInGenerate=inputCnt/parameter.blockSize+1;
    for(auto it=mp.begin();it!=mp.end();it++)
        diskWriteTimesInGenerate+=it->second/parameter.blockSize+1;
}

void MainWindow::merge()    //归并
{
    if(parameter.numberOfSequences==1)
    {//只有一个顺串，直接输出到最终的结果文件
        int count=0;
        string inputFile="sequence"+to_string(1)+".txt";
        ifstream fin(inputFile);

        //记录顺串合并次序
        ui->textEdit->append("<font color=\"#0A7F9F\">" + QString("第1次合并:") +"</font>");//显示蓝色
        ui->textEdit->append("<font color=\"#0A7F9F\">" + QString("合并的顺串号:") + "</font>");
        for(int i=1;i<=parameter.numberOfSequences;i++)
        {
            ui->textEdit->insertHtml("<font color=\"#0A7F9F\">" + QString::number(i) +"</font>" + "&nbsp;&nbsp;&nbsp;" );
        }
        ui->textEdit->append("<font color=\"#0A7F9F\">" + QString("生成的顺串号:") + QString::number(parameter.numberOfSequences+1) + "</font>");

        int num;
        while(fin>>num)
        {
            Fout<<num<<' ';
            count++;
        }
        fin.close();
        Fout.close();
        diskReadTimesInMerge=count/(parameter.sumSize/2)+1;
        diskWriteTimesInMerge=count/(parameter.sumSize/2)+1;
    }
    else    //顺串数量大于1
    {
        if(parameter.numberOfSequences<=parameter.mergeWays)
        {//生成的顺串数量小于等于设置的归并路数，一次归并即可得到最终结果
            int outputCnt=0;
            minLoserTree<int> MLT;
            int iniPlayer[parameter.numberOfSequences+1];   //iniPlayer
            int offset[parameter.numberOfSequences+1];      //各个存储顺串的文件的定位指针
            for(int i=1;i<=parameter.numberOfSequences;i++)
            {
                string inputFile="sequence"+to_string(i)+".txt";
                ifstream fin(inputFile);
                fin>>iniPlayer[i];
                offset[i]=fin.tellg();
                fin.close();
            }
            MLT.initialize(iniPlayer,parameter.numberOfSequences);  //初始化最小输者树

            //记录顺串合并次序
            ui->textEdit->append("<font color=\"#0A7F9F\">" + QString("第1次合并:") +"</font>");//显示蓝色
            ui->textEdit->append("<font color=\"#0A7F9F\">" + QString("合并的顺串号:") + "</font>");
            for(int i=1;i<=parameter.numberOfSequences;i++)
            {
                ui->textEdit->insertHtml("<font color=\"#0A7F9F\">" + QString::number(i) +"</font>" + "&nbsp;&nbsp;&nbsp;" );
            }
            ui->textEdit->append("<font color=\"#0A7F9F\">" + QString("生成的顺串号:") + QString::number(parameter.numberOfSequences+1) + "</font>");

            int cnt=0;
            while(cnt<parameter.total)
            {
                cnt++;
                int lastWinnerIndex=MLT.getWinnerIndex();
                int lastWinnerValue=MLT.getWinnerValue();
                Fout<<lastWinnerValue<<' ';
                outputCnt++;

                string inputFile="sequence"+to_string(lastWinnerIndex)+".txt";
                ifstream fin(inputFile);
                fin.clear();
                fin.seekg(offset[lastWinnerIndex]+1);

                int newValue;
                if(offset[lastWinnerIndex]==-1)
                    newValue=INT_MAX;
                else {
                    fin>>newValue;
                    offset[lastWinnerIndex]=fin.tellg();
                    if(offset[lastWinnerIndex]==-1)    //这里的if处理的是文件中最后一个元素后面的空格
                        newValue=INT_MAX;
                }
                MLT.rePlay(newValue);
                fin.close();
            }
            Fout.close();

            for(auto it=mp.begin();it!=mp.end();it++)
                diskReadTimesInMerge+=it->second/parameter.blockSize+1;
            diskWriteTimesInMerge=outputCnt/parameter.blockSize+1;
        }
        else
        {//生成的顺串数量大于设置的归并路数，需要考虑多次归并
         //利用K-Huffman树可以产生最优归并策略，使用小根堆，每次选择K个长度最小的顺串进行归并

            int sequenceOrder=parameter.numberOfSequences;              //每次归并产生的顺串的id

            //初始化最小堆
            pair<int,int>* pairArray=new pair<int,int>[mp.size()+1];
            minHeap<pair<int,int>> heap;
            int pos=1;
            for(auto it=mp.begin();it!=mp.end();it++)
            {
                pairArray[pos]=make_pair(it->second,it->first); //顺串长度在前，顺串号在后
                pos++;
            }
            heap.initialize(pairArray,mp.size());

            while(heap.size()!=1)
            {
                //1.设置本次归并的文件输出流
                sequenceOrder++;
                string outputFile="sequence"+to_string(sequenceOrder)+".txt";
                ofstream fout1(outputFile);

                //2.选择k个长度最小的顺串
                vector<pair<int,int>> v;
                while(!heap.empty() && (int)v.size()<parameter.mergeWays)
                {
                    v.emplace_back(heap.top());
                    heap.pop();
                }
                //记录顺串合并次序
                ui->textEdit->append("<font color=\"#0A7F9F\">" + QString("第") + QString::number(sequenceOrder-parameter.numberOfSequences) + QString("次合并:") +"</font>");//显示蓝色
                ui->textEdit->append("<font color=\"#0A7F9F\">" + QString("合并的顺串号:") + "</font>");
                for(auto it=v.begin();it!=v.end();it++)
                {
                    ui->textEdit->insertHtml("<font color=\"#0A7F9F\">" + QString::number(it->second) +"</font>" + "&nbsp;&nbsp;&nbsp;" );
                }
                ui->textEdit->append("<font color=\"#0A7F9F\">" + QString("合并的顺串长度:") + "</font>");
                for(auto it=v.begin();it!=v.end();it++)
                {
                    ui->textEdit->insertHtml("<font color=\"#0A7F9F\">" + QString::number(it->first) +"</font>" + "&nbsp;&nbsp;&nbsp;" );
                }
                ui->textEdit->append("<font color=\"#0A7F9F\">" + QString("生成的顺串号:") + QString::number(sequenceOrder) + "</font>");

                //3.本次k路归并的总规模
                int mergeTotal=0;
                for(int i=0;i<(int)v.size();i++)
                    mergeTotal+=v[i].first;
                ui->textEdit->append("<font color=\"#0A7F9F\">" + QString("生成的顺串长度:") + QString::number(mergeTotal) + "</font>");

                //4.初始化最小输者树
                minLoserTree<int> MLT;
                int iniPlayer[v.size()+1];   //iniPlayer
                int offset[v.size()+1];      //各个存储顺串的文件的定位指针
                for(int i=0;i<(int)v.size();i++)
                {
                    string inputFile="sequence"+to_string(v[i].second)+".txt";
                    ifstream fin(inputFile);
                    fin>>iniPlayer[i+1];
                    offset[i+1]=fin.tellg();
                    fin.close();
                }
                MLT.initialize(iniPlayer,v.size());

                //5.开始归并
                int cnt=0;
                int outputCnt=0;
                while(cnt<mergeTotal)
                {
                    cnt++;
                    int lastWinnerIndex=MLT.getWinnerIndex();
                    int lastWinnerValue=MLT.getWinnerValue();
                    fout1<<lastWinnerValue<<' ';
                    outputCnt++;
                    mp[sequenceOrder]++;    //记录生成的顺串的长度

                    string inputFile="sequence"+to_string(v[lastWinnerIndex-1].second)+".txt";  //需要映射到实际的顺串号
                    ifstream fin(inputFile);
                    fin.clear();
                    fin.seekg(offset[lastWinnerIndex]+1);

                    int newValue;
                    if(offset[lastWinnerIndex]==-1)
                        newValue=INT_MAX;
                    else
                    {
                        fin>>newValue;
                        offset[lastWinnerIndex]=fin.tellg();
                        if(offset[lastWinnerIndex]==-1) //这里的if处理的是文件中最后一个元素后面的空格
                            newValue=INT_MAX;
                    }
                    MLT.rePlay(newValue);
                    fin.close();
                }

                //6.归并完成后的处理
                fout1.close();
                heap.push(make_pair(mp[sequenceOrder],sequenceOrder));
                diskWriteTimesInMerge+=outputCnt/parameter.blockSize+1;
            }// end while(!heap.empty())

            //将最终结果转移到result.txt中
            string inputFile="sequence"+to_string(sequenceOrder)+".txt";
            ifstream fin1(inputFile);
            int x;
            while(fin1>>x)
                Fout<<x<<" ";
            fin1.close();
            Fout.close();   //close result.txt
            for(auto it=mp.begin();it!=mp.end();it++)
                diskReadTimesInMerge+=it->second/parameter.blockSize+1;

        }// end else 多次归并
    }
}

void MainWindow::on_startButton_clicked()
{
    mp.clear();
    if(ui->sourceFileLine->text()=="")
        QMessageBox::information(this,"提示", "请输入数据集源文件名");
    else if(ui->targetFileLine->text()=="")
        QMessageBox::information(this,"提示", "请输入目标文件名");
    else
    {
        if(!initialize())
            return ;
        ui->textEdit->append("<font color=\"#ff0000\">" + QString("**START**") +"</font>");
        generate();
        ui->textEdit->append("<font color=\"#000000\">" + QString("初始顺串已生成，生成的顺串数量为") + QString::number(parameter.numberOfSequences) +"</font>");
        ui->textEdit->append("<font color=\"#000000\">" + QString("顺串产生情况可通过文件sequence_id.txt查看(id为顺串号)") +"</font>");
        ui->textEdit->append("<font color=\"#349F48\">" + QString("生成顺串阶段: 读取磁盘次数:") + QString::number(diskReadTimesInGenerate) + "&nbsp;&nbsp;&nbsp;"
                             + QString("写回磁盘次数:") + QString::number(diskWriteTimesInGenerate) + "&nbsp;&nbsp;&nbsp;"
                             + QString("总磁盘访问次数:") + QString::number(diskReadTimesInGenerate+diskWriteTimesInGenerate) +"</font>");

        ui->textEdit->append("<font color=\"#000000\">" + QString("*下面对顺串进行归并*") +"</font>");
        merge();
        ui->textEdit->append("<font color=\"#000000\">" + QString("*归并完成*") +"</font>");
        ui->textEdit->append("<font color=\"#349F48\">" + QString("归并阶段: 读取磁盘次数:") + QString::number(diskReadTimesInMerge) + "&nbsp;&nbsp;&nbsp;"
                             + QString("写回磁盘次数:") + QString::number(diskWriteTimesInMerge) + "&nbsp;&nbsp;&nbsp;"
                             + QString("总磁盘访问次数:") + QString::number(diskReadTimesInMerge+diskWriteTimesInMerge) + "</font>");

        ui->textEdit->append("<font color=\"#9F4F99\">" + QString("外排序整个过程: 读取磁盘次数:") + QString::number(diskReadTimesInGenerate+diskReadTimesInMerge) + "&nbsp;&nbsp;&nbsp;"
                             + QString("写回磁盘次数:") + QString::number(diskWriteTimesInMerge+diskWriteTimesInGenerate) + "&nbsp;&nbsp;&nbsp;"
                             + QString("总磁盘访问次数:") + QString::number(diskReadTimesInMerge+diskWriteTimesInMerge+diskReadTimesInGenerate+diskWriteTimesInGenerate) + "</font>");

        ui->textEdit->append("<font color=\"#ff0000\">" + QString("**END**") +"</font>");
        QMessageBox::information(this,"提示", "验证完成，请查看结果和分析");
    }
}

void MainWindow::on_exitButton_2_clicked()
{
    ui->textEdit->clear();
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}
