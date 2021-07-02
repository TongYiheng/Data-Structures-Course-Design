#ifndef LAB3_CATALOGTREE_H
#define LAB3_CATALOGTREE_H

#include <string>
#include <vector>
using namespace std;

struct catalogTreeNode
{
    string data;                    //文件名
    bool isFile;                    //1--普通文件 0--目录文件
    int childFileNumber;            //孩子中普通文件的数量
    int childCatalogNumber;         //孩子中目录文件的数量
    catalogTreeNode* parent;        //父节点
    catalogTreeNode* firstChild;    //第一个孩子
    catalogTreeNode* nextSibling;   //下一个兄弟
    catalogTreeNode() {}
    catalogTreeNode(string& theData,bool isF)
    {
        data=theData;
        isFile=isF;
        childFileNumber=childCatalogNumber=0;
        parent=firstChild=nextSibling=NULL;
    }
    catalogTreeNode(string& theData,bool isF,catalogTreeNode* theParent,catalogTreeNode* theFirstChild,catalogTreeNode* theNextSibling)
    {
        data=theData;
        isFile=isF;
        childFileNumber=childCatalogNumber=0;
        parent=theParent;
        firstChild=theFirstChild;
        nextSibling=theNextSibling;
    }
};

class catalogTree
{
private:
    catalogTreeNode* root;                      //根节点
    catalogTreeNode* currentNode;               //当前节点
    void eraseSubCatalog(catalogTreeNode* p);   //删除p所指目录的所有内容
    string workDirectory;                       //当前exe工作的绝对路径
    vector<string> currentNodePath;             //当前节点的绝对路径
    vector<string> pv;                          //load操作中用于重建文件

public:
    catalogTree();
    ~catalogTree();
    //基本操作
    void dir();
    void dir_test();
    void cd();
    void cd_test();
    void cd(string& str);
    void mkdir(string& str);
    void mkfile(string& str);
    void Delete(string& str);
    void save(string& str);
    void load(string& str);
    void clear();
    //扩展
    void setWorkDirectory(char *buf);               //设置当前exe工作的绝对路径
    void getCurrentDirectory();                     //每次操作结束后显示当前的绝对路径
    void rebuild(catalogTreeNode* r);               //从文件中重新加载目录后，重新建立文件
    void setCurrentNodePath();                      //从文件中重新加载目录后，设置当前的路径
    void test(string& str);                         //测试数据集
    void help();                                    //显示帮助界面
};


#endif //LAB3_CATALOGTREE_H
