//skiplist.cpp
#include "skiplist.h"

#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

extern ifstream inputFile;
extern ofstream outputFile;

//模板特化
template class skipNode<int>;
template class skipList<int>;

template <class T>
skipList<T>::skipList(T largeElement, int maxNumbers, float prob)
{//构造函数，关键字小于largeElement，数对个数size最多为maxNumbers，0<prob<1
    cutOff=prob*RAND_MAX;
    maxLevel=(int) ceil(logf((float)maxNumbers) / logf(1/prob)) - 1;
    p=prob;
    levels=0;
    size=0;
    tailElement=largeElement;

    //生成头结点、尾结点和数组last
    headerNode=new skipNode<T> (tailElement,maxLevel+1);
    tailNode=new skipNode<T> (tailElement,0);
    last=new skipNode<T> *[maxLevel+1];

    //链表为空时，任意级链表中的头节点都指向尾结点
    for(int i=0;i<=maxLevel;i++)
        headerNode->next[i]=tailNode;
}

template <class T>
skipList<T>::~skipList()
{//析构函数，删除跳表的所有节点

    //删除除了头结点和尾结点之外的节点
    skipNode<T>* deleteNode=headerNode->next[0];
    while(deleteNode!=tailNode)
    {
        skipNode<T>* nextNode=deleteNode->next[0];
        delete deleteNode;
        deleteNode=nextNode;
    }
    delete headerNode;
    delete tailNode;
    delete []last;
}

template <class T>
void skipList<T>::initialize(int m)
{//用读入的m个元素初始化跳表
    int x;
    while(m--)
    {
        inputFile>>x;
        insert(x);
    }
}

template <class T>
T* skipList<T>::find(const T &theElement) const
{//返回匹配的元素的指针，如果没有匹配的返回NULL
    if(theElement>=tailElement)
        return NULL;

    //位置beforeNode是关键字为theElement的节点之前最右边的位置
    skipNode<T>* beforeNode=headerNode;
    for(int i=levels;i>=0;i--)  //从上级链表到下级链表
        while(beforeNode->next[i]->element<theElement)  //跟踪i级链表指针
        {
            beforeNode=beforeNode->next[i];
        }

    //检查下一个节点的关键字是否为theElement
    if(beforeNode->next[0]->element==theElement)
        return &beforeNode->next[0]->element;

    return NULL;    //无匹配的数对
}

template <class T>
int skipList<T>::level() const
{//返回一个表示链表级的随机数，这个数不大于maxLevel
    int lev=0;
    while(rand()<=cutOff)
        lev++;
    return (lev<=maxLevel) ? lev : maxLevel;
}

template <class T>
skipNode<T>* skipList<T>::search(const T &theElement) const
{//搜索关键字theElement，把每一级链表中要查看的最后一个节点存储在数组last中
 //返回包含关键字theElement的节点

    //位置beforeNode是关键字为theElement的节点之前最右边的位置
    skipNode<T>* beforeNode=headerNode;
    for(int i=levels;i>=0;i--)  //从上级链表到下级链表
    {
        while (beforeNode->next[i]->element < theElement)  //跟踪i级链表指针
        {
            beforeNode = beforeNode->next[i];
        }
        last[i]=beforeNode;
    }
    return beforeNode->next[0];
}

template <class T>
void skipList<T>::insert(const T& theElement)
{//把theElement插入跳表，关键字若存在则覆盖

    if(theElement>=tailElement)
    {
        cout<<"关键字过大"<<endl;
        return ;
    }

    //查找关键字为theElement的元素是否存在
    skipNode<T>* theNode=search(theElement);
    if(theNode->element==theElement)
    {//存在，覆盖
        return ;
    }

    //不存在，确定新节点所在的级链表
    int theLevel=level();       //新节点的级
    if(theLevel>levels)
    {
        theLevel=++levels;
        last[theLevel]=headerNode;
    }
    //在节点theNode之后插入新节点
    skipNode<T>* newNode=new skipNode<T> (theElement,theLevel+1);
    for(int i=0;i<=theLevel;i++)
    {
        newNode->next[i]=last[i]->next[i];
        last[i]->next[i]=newNode;
    }
    size++;
    return ;
}

template <class T>
void skipList<T>::erase(const T& theElement)
{//删除关键字为theElement的元素
    if(theElement>=tailElement)
    {
        cout<<"关键字过大"<<endl;
        return ;
    }

    //查看是否有匹配元素
    skipNode<T>* theNode=search(theElement);
    if(theNode->element!=theElement)
    {//不存在
        return ;
    }
    //从跳表中删除节点
    for(int i=0;i<=levels && last[i]->next[i]==theNode;i++)
        last[i]->next[i]=theNode->next[i];
    //更新链表级
    while (levels>0 && headerNode->next[levels]==tailNode)
        levels--;
    delete theNode;
    size--;
    return ;
}

template <class T>
void skipList<T>::eraseMax()
{//删除关键字最大的元素
    if(size==0)
    {
        cout<<"跳表为空"<<endl;
        return ;
    }

    //找关键字最大
    skipNode<T>* beforeNode=headerNode;
    for(int i=0;i<size;i++)
        beforeNode=beforeNode->next[0];

    outputFile<<beforeNode->element<<endl;
    skipNode<T>* theNode=search(beforeNode->element);

    //从跳表中删除节点
    for(int i=0;i<=levels && last[i]->next[i]==theNode;i++)
        last[i]->next[i]=theNode->next[i];
    //更新链表级
    while (levels>0 && headerNode->next[levels]==tailNode)
        levels--;
    delete theNode;
    size--;
    return ;
}

template <class T>
void skipList<T>::eraseMin()
{//删除关键字最小的元素
    if(size==0)
    {
        cout<<"跳表为空"<<endl;
        return ;
    }

    //找关键字最小的元素
    for(int i=levels;i>=0;i--)  //从上级链表到下级链表
        last[i]=headerNode;
    skipNode<T>* theNode=headerNode->next[0];
    outputFile<<theNode->element<<endl;

    //从跳表中删除节点
    for(int i=0;i<=levels && last[i]->next[i]==theNode;i++)
        last[i]->next[i]=theNode->next[i];
    //更新链表级
    while (levels>0 && headerNode->next[levels]==tailNode)
        levels--;
    delete theNode;
    size--;
    return ;
}

template <class T>
void skipList<T>::output()
{//输出跳表中所有元素的异或
    int ans=0;
    skipNode<T>* currentNode=headerNode->next[0];
    while(currentNode->element < tailElement)
    {
        ans^=currentNode->element;
        currentNode=currentNode->next[0];
    }
    outputFile<<ans<<endl;
}

template <class T>
void skipList<T>::reorganize()
{//跳表的整理
    //1.为每个位置分配级数
    int* posLevel=new int[size+1]; //下标位置1到size
    memset(posLevel,0,(size+1)*sizeof(int));
    float prob=p;
    for(int i=0;i<=maxLevel;i++)
    {
        for(int j=1;j<=size;j++)
        {
            if(j % (int)(1/prob)==0)
                posLevel[j]++;
        }
        prob=prob*p;
    }
    //更新levels
    int newLevels=0;
    for(int i=1;i<=size;i++)
    {
        if(posLevel[i]>newLevels)
            newLevels=posLevel[i];
    }
    levels=newLevels;

    //2.为每个位置建立skipNode
    int *posElement=new int[size+1];
    skipNode<T>* theNode=headerNode;
    int pos=1;
    while(theNode->next[0]->element < tailElement)
    {
        posElement[pos]=theNode->next[0]->element;
        theNode=theNode->next[0];
        pos++;
    }
    skipNode<int>** nodeArray=new skipNode<int>* [size+1];
    for(int i=1;i<=size;i++)
        nodeArray[i]=new skipNode<T> (posElement[i],posLevel[i]+1);

    //3.释放整理前的跳表的每个节点，除了头结点和尾结点
    skipNode<T>* deleteNode=headerNode->next[0];
    while(deleteNode!=tailNode)
    {
        skipNode<T>* nextNode=deleteNode->next[0];
        delete deleteNode;
        deleteNode=nextNode;
    }

    //4.对跳表的每一级进行连接
    for(int iLevel=0;iLevel<=levels;iLevel++) //maxLevel or levels?
    {
        skipNode<T>* beforeNode=headerNode;
        for(int j=1;j<=size;j++)
        {
            if(posLevel[j]>=iLevel)
            {
                skipNode<T>* currentNode=nodeArray[j];
                beforeNode->next[iLevel]=currentNode;
                beforeNode=currentNode;
            }
        }
        beforeNode->next[iLevel]=tailNode;
    }

    //5.释放内存
    delete []posLevel;
    delete []posElement;
}
