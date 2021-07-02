#include "minHeap.h"
#include <iostream>
using namespace std;

template class minHeap<pair<int,int>>;

template <class T>
minHeap<T>::minHeap(int initialCapacity)
{
    heap=new T[initialCapacity+1];
    arrayLength=initialCapacity+1;
    heapSize=0;
}

template <class T>
void minHeap<T>::pop()
{//删除堆顶元素
    heap[1].~T();   //删除最小元素
    //重新构造堆
    T lastElement=heap[heapSize--];
    //从根开始，为lastElement寻找合适的位置
    int currentNode=1,child=2;
    while(child<=heapSize)
    {
        if(child<heapSize && heap[child]>heap[child+1])
            child++;
        if(lastElement<=heap[child])
            break;
        heap[currentNode]=heap[child];
        currentNode=child;
        child*=2;
    }
    heap[currentNode]=lastElement;
}

template <class T>
void minHeap<T>::push(const T &theElement)
{//向堆中插入一个元素
    if(heapSize==arrayLength-1)     //数组已满
    {
        T* newHeap=new T[arrayLength*2];
        arrayLength*=2;
        copy(heap+1,heap+arrayLength,newHeap+1);
        delete []heap;
        heap=newHeap;
    }

    //为theElement寻找插入位置
    int currentNode=++heapSize;
    while(currentNode!=1 && theElement<heap[currentNode/2])
    {//不能把theElement放入heap[currentNode]
        heap[currentNode]=heap[currentNode/2];
        currentNode/=2;
    }
    heap[currentNode]=theElement;
}

template <class T>
void minHeap<T>::initialize(T *theHeap, int theSize)
{//初始化一个小根堆
    delete []heap;
    heap=theHeap;
    heapSize=theSize;
    arrayLength=theSize+1;

    //堆化
    for(int root=heapSize/2;root>=1;root--)
    {
        T rootElement=heap[root];   //子树的根
        //寻找放置rootElement的位置
        int child=2*root;
        while(child<=heapSize)
        {
            if(child<heapSize && heap[child]>heap[child+1])
                child++;

            if(rootElement<=heap[child])
                break;
            heap[child/2]=heap[child];
            child*=2;
        }
        heap[child/2]=rootElement;
    }
}
