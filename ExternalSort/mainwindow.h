#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool initialize();
    void generate();
    void merge();

private slots:
    void on_startButton_clicked();

    void on_exitButton_2_clicked();

    void on_exitButton_clicked();

private:
    Ui::MainWindow *ui;
};

struct p
{
    int total;              //待排序的元素规模（int类型的个数）
    int mergeWays;          //归并路数
    int blockSize;          //归并阶段的缓冲区大小（int类型的个数）
    int sumSize;            //内存总容量（int类型的个数）
    int numberOfPlayers;    //内存能容纳的输者树外部节点个数
    int numberOfSequences;  //根据最小输者树产生的顺串数量
};

struct sortPlayer       //用于构建顺串
{
    int id,             //顺串号
        key;            //关键字
    bool operator<(sortPlayer &p) const
    {
        return (id!=p.id) ? (id<p.id) : (key<p.key);
    }
    bool operator>(sortPlayer &p) const
    {
        return (id!=p.id) ? (id>p.id) : (key>p.key);
    }
    bool operator==(sortPlayer &p) const
    {
        return key==p.key;
    }
};

#endif // MAINWINDOW_H
