//skiplist.h
#ifndef SKIPLIST_H
#define SKIPLIST_H

template <class T>
struct skipNode
{
    T element;                  //元素
    skipNode<T> **next;         //指针数组
    int size;                   //指针数组大小
    skipNode(const T& theElement, int theSize):element(theElement)
    {
        next=new skipNode<T>* [theSize];
        size=theSize;
    }
};

template <class T>
class skipList
{
private:
    float cutOff;               //用来确定层数
    float p;                    //属于i-1级链表同时属于i级链表的概率
    int levels;                 //当前最大的非空链表
    int size;                   //元素个数
    int maxLevel;               //允许的最大链表层数
    T tailElement;              //最大元素
    skipNode<T>* headerNode;    //头结点指针
    skipNode<T>* tailNode;      //尾结点指针
    skipNode<T>** last;         //last[i]表示i层的最后节点
public:
    skipList(T largeElement, int maxNumbers, float prob);   //构造函数，关键字小于largeElement，数对个数size最多为maxNumbers，0<prob<1
    ~skipList();                                            //析构函数，删除跳表的所有节点
    int getSize() const {return size;}                      //返回跳表元素个数
    void initialize(int m);                                 //用读入的m个元素初始化跳表
    T* find(const T& theElement) const;                     //返回匹配的元素的指针，如果没有匹配的返回NULL
    int level() const;                                      //返回一个表示链表级的随机数，这个数不大于maxLevel
    skipNode<T>* search(const T& theElement) const;         //搜索关键字theElement，把每一级链表中要查看的最后一个节点存储在数组last中
    void insert(const T& theElement);                       //把theElement插入跳表，关键字若存在则覆盖
    void erase(const T& theElement);                        //删除关键字为theKey的元素
    void eraseMax();                                        //删除关键字最大的元素
    void eraseMin();                                        //删除关键字最小的元素
    void output();                                          //输出跳表中所有元素的异或
    void reorganize();                                      //跳表的整理
};

#endif // SKIPLIST_H
