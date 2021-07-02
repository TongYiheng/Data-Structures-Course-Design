#ifndef KDTREE_KDIMENSIONALTREE_H
#define KDTREE_KDIMENSIONALTREE_H

#include <bits/stdc++.h>
using namespace std;

template <class T>
struct point {
    vector<T> data;         //k个维度的值
    int splitDimension;     //划分维度
    point() {
        data.clear();
        splitDimension=-1;
    }
    point(vector<T> theData,int d=-1) {
        data=theData;
        splitDimension=d;
    }
    bool operator < (const point& p) const {
        return data[splitDimension] < p.data[splitDimension];
    }
};

template <class T>
struct kDimensionalTreeNode {
    vector<T> data;                         //k个维度的值
    kDimensionalTreeNode<T> *leftChild;     //左孩子
    kDimensionalTreeNode<T> *rightChild;    //右孩子
    kDimensionalTreeNode()
    {
        data.clear();
        leftChild=rightChild=NULL;
    }
    kDimensionalTreeNode(vector<T> &theData,kDimensionalTreeNode<T> *theLeftChild=NULL,kDimensionalTreeNode<T> *theRightChild=NULL)
    {
        data=theData;
        leftChild=theLeftChild;
        rightChild=theRightChild;
    }
};

template <class T>
struct MINNODE {
    kDimensionalTreeNode<T> *pm,*m; //pm:m的父节点  m:划分维度为dimension下的最小点
    int dimension;                  //划分维度
    MINNODE() {}
    MINNODE(kDimensionalTreeNode<T> *pp,kDimensionalTreeNode<T> *p,int d) {
        pm=pp;
        m=p;
        dimension=d;
    }
};

template <class T>
class kDimensionalTree {
private:
    int k;                                  //维数
    kDimensionalTreeNode<T> *root;          //根节点
    int treeSize;                           //k-d树的节点数量
    MINNODE<T> *minNode;                    //保存右子树中当前划分维度最小的点
    kDimensionalTreeNode<T> *nearestNode;   //最邻近节点
    double nearestDistance;                 //最邻近距离
    priority_queue<pair<double,kDimensionalTreeNode<T>*>> kNearest; //k邻近节点和距离
    int knn;
    vector<vector<T>> radiusVector;         //保存范围内所有节点
    //遍历相关
    static void preOrder(kDimensionalTreeNode<T> *t);               //前序遍历
    static void inOrder(kDimensionalTreeNode<T> *t);                //中序遍历
    static void postOrder(kDimensionalTreeNode<T> *t);              //后序遍历
    static void (*visit)(kDimensionalTreeNode<T> *);                //访问函数
    static void dispose(kDimensionalTreeNode<T> *t) {delete t;}     //删除t指向的节点
    static void output(kDimensionalTreeNode<T> *t) {for(auto x:t->data) cout<<x<<" "; cout<<"\n";} //输出节点的关键字
    //操作相关
    kDimensionalTreeNode<T>* build(int left,int right,int dimension,point<T> *p);   //递归构建k-d树
    void findMin(kDimensionalTreeNode<T> *p,int D);                                 //在p的右子树中寻找划分维度为D的最小点
    void eraseNode();                                                               //递归删除
    kDimensionalTreeNode<T>* nearestSearch(kDimensionalTreeNode<T> *s,vector<T> &target,int D); //最近邻搜索，递归
    void  kNearestSearch(kDimensionalTreeNode<T> *s,vector<T> &target,int D);                   //k近邻搜索，递归
    vector<vector<T>>& radiusSearch(vector<T> &x,vector<T> &y,kDimensionalTreeNode<T> *s);      //范围搜索，递归
    double getDistance(vector<T> &x,vector<T> &y) const;                                        //计算两点间的距离
    bool check(vector<T> &x,vector<T> &y,kDimensionalTreeNode<T> *s);                           //判断s是否在范围内
    bool prob(vector<T> &x,vector<T> &y,kDimensionalTreeNode<T> *s);                            //判断s是否有可能在范围内
    kDimensionalTreeNode<T>* loadTree(vector<T> *pre,vector<T> *in,int size);                   //递归加载k-d树结构

public:
    kDimensionalTree(int kk=2);
    ~kDimensionalTree();
    //数据成员相关
    bool empty() const {return treeSize==0;}
    int size() const {return treeSize;}
    int getK() const {return k;}
    double getNearestDistance() const {return nearestDistance;}
    //遍历相关
    void preOrder(void(*theVisit)(kDimensionalTreeNode<T>*)) {visit=theVisit; preOrder(root);}      //前序
    void inOrder(void(*theVisit)(kDimensionalTreeNode<T>*)) {visit=theVisit; inOrder(root);}        //中序
    void postOrder(void(*theVisit)(kDimensionalTreeNode<T>*)) {visit=theVisit; postOrder(root);}    //后序
    void preOrderOut(kDimensionalTreeNode<T> *t,ofstream &out);     //通过输出流out保存前序序列
    void inOrderOut(kDimensionalTreeNode<T> *t,ofstream &out);      //通过输出流out保存中序序列
    //基本操作
    void initialize(int kk=2);                                      //初始化参数k
    void buildTree(point<T> *p,int pSize,int kk);                   //根据点的数组p[1:pSize]构建k-d树(中值优化)
    vector<T>* find(vector<T>& theElement);                         //查询theElement，若存在返回指向theElement的指针，否则返回空指针
    int insert(vector<T>& theElement);                              //插入theElement，成功插入返回1，否则返回0
    int erase(vector<T>& theElement);                               //删除theElement，成功删除返回1，否则返回0
    void deleteTree();                                              //删除整个k-d树
    //扩展操作
    kDimensionalTreeNode<T>* nearestSearch(vector<T> &target);      //最近邻搜索，返回最近邻的点
    priority_queue<pair<double,kDimensionalTreeNode<T>*>>& kNearestSearch(vector<T> &target,int _k); //k近邻搜索，返回不超过k个节点和对应的距离
    vector<vector<T>>& radiusSearch(vector<T> &x,vector<T> &y);     //范围搜索，返回在范围内的所有点
    void rearrange();                                               //重构k-d树，使之平衡
    void tempVisualization();                                       //k-d树可视化，文件名为temp.dot和temp.jpg
    void visualization(string &strDot,string &strImage);            //k-d树可视化，指定文件名
    void save(string &str1,string &str2);                           //保存k-d树结构，将前序序列保存至文件str1，将中序序列保存至文件str2
    void load(string &str1,string &str2);                           //加载k-d树结构，前序序列文件str1，中序序列文件str2
    void test(string &str1,string &str2);                           //测试数据集
};


#endif //KDTREE_KDIMENSIONALTREE_H
