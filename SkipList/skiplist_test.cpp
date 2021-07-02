//skiplist_test.cpp
#include "skiplist_test.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>

using namespace std;

extern ofstream testInputFile,testOutputFile;

//模板特化
template class skipNodeTest<int>;
template class skipListTest<int>;

template <class T>
skipListTest<T>::skipListTest(T largeElement, int maxNumbers, float prob)
{//构造函数，关键字小于largeElement，数对个数size最多为maxNumbers，0<prob<1
    cutOff=prob*RAND_MAX;
    maxLevel=(int) ceil(logf((float)maxNumbers) / logf(1/prob)) - 1;
    p=prob;
    levels=0;
    size=0;
    tailElement=largeElement;

    //生成头结点、尾结点和数组last
    headerNode=new skipNodeTest<T> (tailElement,maxLevel+1);
    tailNode=new skipNodeTest<T> (tailElement,0);
    last=new skipNodeTest<T> *[maxLevel+1];

    //链表为空时，任意级链表中的头节点都指向尾结点
    for(int i=0;i<=maxLevel;i++)
        headerNode->next[i]=tailNode;
}

template <class T>
skipListTest<T>::~skipListTest()
{//析构函数，删除跳表的所有节点
    //删除除了头结点和尾结点之外的节点
    skipNodeTest<T>* deleteNode=headerNode->next[0];
    while(deleteNode!=tailNode)
    {
        skipNodeTest<T>* nextNode=deleteNode->next[0];
        delete deleteNode;
        deleteNode=nextNode;
    }
    delete headerNode;
    delete tailNode;
    delete []last;
}

/*
template <class T>
void skipListTest<T>::initialize(int m)
{//用读入的m个元素初始化跳表
    int x;
    while(m--)
    {
        int cnt=0;
        cin>>x;
        insert(x,cnt);
    }
}
*/

template <class T>
T* skipListTest<T>::find(const T &theElement,int& cnt) const
{//返回匹配的元素的指针，如果没有匹配的返回NULL
    if(theElement>=tailElement)
        return NULL;

    //位置beforeNode是关键字为theElement的节点之前最右边的位置
    skipNodeTest<T>* beforeNode=headerNode;
    for(int i=levels;i>=0;i--)  //从上级链表到下级链表
        while(beforeNode->next[i]->element<theElement)  //跟踪i级链表指针
        {
            beforeNode=beforeNode->next[i];
            cnt++;
        }

    //检查下一个节点的关键字是否为theElement
    cnt++;
    if(beforeNode->next[0]->element==theElement)
        return &beforeNode->next[0]->element;

    return NULL;    //无匹配的数对
}

template <class T>
int skipListTest<T>::level() const
{//返回一个表示链表级的随机数，这个数不大于maxLevel
    int lev=0;
    while(rand()<=cutOff)
        lev++;
    return (lev<=maxLevel) ? lev : maxLevel;
}

template <class T>
skipNodeTest<T>* skipListTest<T>::search(const T &theElement,int& cnt) const
{//搜索关键字theElement，把每一级链表中要查看的最后一个节点存储在数组last中
    //返回包含关键字theElement的节点

    //位置beforeNode是关键字为theElement的节点之前最右边的位置
    skipNodeTest<T>* beforeNode=headerNode;
    for(int i=levels;i>=0;i--)  //从上级链表到下级链表
    {
        while (beforeNode->next[i]->element < theElement)  //跟踪i级链表指针
        {
            beforeNode = beforeNode->next[i];
            cnt++;
        }
        last[i]=beforeNode;
    }
    return beforeNode->next[0];
}

template <class T>
int skipListTest<T>::insert(const T& theElement,int& cnt)
{//把theElement插入跳表，关键字若存在则覆盖

    if(theElement>=tailElement)
    {
        testOutputFile<<"关键字过大"<<endl;
        return 0;
    }

    //查找关键字为theElement的元素是否存在
    skipNodeTest<T>* theNode=search(theElement,cnt);
    cnt++;
    if(theNode->element==theElement)
    {//存在，覆盖
        return 0;
    }

    //不存在，确定新节点所在的级链表
    int theLevel=level();       //新节点的级
    if(theLevel>levels)
    {
        theLevel=++levels;
        last[theLevel]=headerNode;
    }
    //在节点theNode之后插入新节点
    skipNodeTest<T>* newNode=new skipNodeTest<T> (theElement,theLevel+1);
    for(int i=0;i<=theLevel;i++)
    {
        newNode->next[i]=last[i]->next[i];
        last[i]->next[i]=newNode;
    }
    size++;
    return 1;
}

template <class T>
int skipListTest<T>::erase(const T& theElement,int& cnt)
{//删除关键字为theElement的元素
    if(theElement>=tailElement)
    {
        testOutputFile<<"关键字过大"<<endl;
        return 0;
    }

    //查看是否有匹配元素
    skipNodeTest<T>* theNode=search(theElement,cnt);
    cnt++;
    if(theNode->element!=theElement)
    {//不存在
        return 0;
    }
    //从跳表中删除节点
    for(int i=0;i<=levels && last[i]->next[i]==theNode;i++)
        last[i]->next[i]=theNode->next[i];
    //更新链表级
    while (levels>0 && headerNode->next[levels]==tailNode)
        levels--;
    delete theNode;
    size--;
    return 1;
}

template <class T>
void skipListTest<T>::eraseMax(int& cnt)
{//删除关键字最大的元素
    if(size==0)
    {
        testOutputFile<<"跳表为空"<<endl;
        return ;
    }

    //找关键字最大
    skipNodeTest<T>* beforeNode=headerNode;
    for(int i=0;i<size;i++)
        beforeNode=beforeNode->next[0];

    testOutputFile<<beforeNode->element<<endl;
    skipNodeTest<T>* theNode=search(beforeNode->element,cnt);

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
void skipListTest<T>::eraseMin()
{//删除关键字最小的元素
    if(size==0)
    {
        testOutputFile<<"跳表为空"<<endl;
        return ;
    }

    //找关键字最小的元素
    for(int i=levels;i>=0;i--)  //从上级链表到下级链表
        last[i]=headerNode;
    skipNodeTest<T>* theNode=headerNode->next[0];
    testOutputFile<<theNode->element<<endl;

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
void skipListTest<T>::output()
{//输出跳表中所有元素的异或
    int ans=0;
    skipNodeTest<T>* currentNode=headerNode->next[0];
    while(currentNode->element < tailElement)
    {
        ans^=currentNode->element;
        currentNode=currentNode->next[0];
    }
    testOutputFile<<ans<<endl;
}

template <class T>
void skipListTest<T>::reorganize()
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

    //2.为每个位置建立skipNodeTest
    int *posElement=new int[size+1];
    skipNodeTest<T>* theNode=headerNode;
    int pos=1;
    while(theNode->next[0]->element < tailElement)
    {
        posElement[pos]=theNode->next[0]->element;
        theNode=theNode->next[0];
        pos++;
    }
    skipNodeTest<int>** nodeArray=new skipNodeTest<int>* [size+1];
    for(int i=1;i<=size;i++)
        nodeArray[i]=new skipNodeTest<T> (posElement[i],posLevel[i]+1);

    //3.释放整理前的跳表的每个节点，除了头结点和尾结点
    skipNodeTest<T>* deleteNode=headerNode->next[0];
    while(deleteNode!=tailNode)
    {
        skipNodeTest<T>* nextNode=deleteNode->next[0];
        delete deleteNode;
        deleteNode=nextNode;
    }

    //4.对跳表的每一级进行连接
    for(int iLevel=0;iLevel<=levels;iLevel++) //maxLevel or levels?
    {
        skipNodeTest<T>* beforeNode=headerNode;
        for(int j=1;j<=size;j++)
        {
            if(posLevel[j]>=iLevel)
            {
                skipNodeTest<T>* currentNode=nodeArray[j];
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
