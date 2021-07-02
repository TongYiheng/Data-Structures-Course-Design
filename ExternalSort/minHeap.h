#ifndef MINHEAP_H
#define MINHEAP_H

template <class T>
class minHeap                               //小根堆
{
private:
    T *heap;                                //保存堆的数组
    int arrayLength;                        //数组大小
    int heapSize;                           //堆的大小
public:
    minHeap(int initialCapacity=10);
    ~minHeap() {delete []heap;}
    bool empty() const {return heapSize==0;}
    int size() const {return heapSize;}
    T& top() const {return heap[1];}        //返回堆顶元素
    void pop();                             //删除堆顶元素
    void push(const T& theElement);         //向堆中插入一个元素
    void initialize(T* theHeap,int theSize);//初始化一个小根堆
};

#endif // MINHEAP_H
