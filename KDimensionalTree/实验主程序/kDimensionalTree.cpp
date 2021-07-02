#include "kDimensionalTree.h"

template class kDimensionalTree<int>;

template <class T>
void (*kDimensionalTree<T>::visit)(kDimensionalTreeNode<T>*)=NULL;    //类的静态成员的初始化

template <class T>
kDimensionalTree<T>::kDimensionalTree(int kk) { //构造函数
    root=NULL;
    treeSize=0;
    minNode=NULL;
    nearestNode=NULL;
    nearestDistance=1e9;
    k=kk;
}

template <class T>
kDimensionalTree<T>::~kDimensionalTree<T>() { //析构函数
    deleteTree();
}

/* ------------------------------------- 私有成员函数 -------------------------------------*/

template <class T>
kDimensionalTreeNode<T>* kDimensionalTree<T>::build(int left,int right,int dimension,point<T> *p) {
//递归构建k-d树
    if(left>right)
        return NULL;
    for(int i=left;i<=right;i++) {
        p[i].splitDimension=dimension;
    }
    int mid=(left+right) >> 1;
    nth_element(p+left,p+mid,p+right+1);
    //若有多个点在当前维度的划分值相等，选择mid为最前面的位置（保证k-d树的正确结构：等于进右子树）
    int f=mid;
    for(int i=mid-1;i>=left;i--) {
        if(p[mid].data[dimension]==p[i].data[dimension]) f=i;
        else break;
    }
    mid=f;
    kDimensionalTreeNode<T> *pNode=new kDimensionalTreeNode<T>(p[mid].data);
    if(left<mid)    pNode->leftChild=build(left,mid-1,(dimension+1)%k,p);
    if(right>mid)   pNode->rightChild=build(mid+1,right,(dimension+1)%k,p);
    return pNode;
}

template <class T>
void kDimensionalTree<T>::preOrder(kDimensionalTreeNode<T> *t)
{
    if(t!=NULL)
    {
        kDimensionalTree<T>::visit(t);
        preOrder(t->leftChild);
        preOrder(t->rightChild);
    }
}

template <class T>
void kDimensionalTree<T>::inOrder(kDimensionalTreeNode<T> *t)
{
    if(t!=NULL)
    {
        inOrder(t->leftChild);
        kDimensionalTree<T>::visit(t);
        inOrder(t->rightChild);
    }
}

template <class T>
void kDimensionalTree<T>::postOrder(kDimensionalTreeNode<T> *t)
{
    if(t!=NULL)
    {
        postOrder(t->leftChild);
        postOrder(t->rightChild);
        kDimensionalTree<T>::visit(t);
    }
}

template <class T>
void kDimensionalTree<T>::eraseNode() { //递归删除
    MINNODE<T> *deleteNode=minNode;
    if(deleteNode->m->leftChild==NULL && deleteNode->m->rightChild==NULL) {//删除叶节点
        if(deleteNode->m==deleteNode->pm->leftChild) {
            deleteNode->pm->leftChild=NULL;
            delete deleteNode->m;
        }
        else {
            deleteNode->pm->rightChild=NULL;
            delete deleteNode->m;
        }
    }
    else if(deleteNode->m->rightChild!=NULL) {//要删除的节点有右孩子
        findMin(deleteNode->m,deleteNode->dimension);
        deleteNode->m->data=minNode->m->data;
        eraseNode();
    }
    else {  //要删除的节点没有右孩子，有左孩子：交换左右孩子，转换为有右孩子的情况
        //交换左右孩子
        kDimensionalTreeNode<T> *temp=deleteNode->m->leftChild;
        deleteNode->m->leftChild=deleteNode->m->rightChild;
        deleteNode->m->rightChild=temp;
        //转换为有右孩子的情况
        findMin(deleteNode->m,deleteNode->dimension);
        deleteNode->m->data=minNode->m->data;
        eraseNode();
    }
}

template <class T>
void kDimensionalTree<T>::findMin(kDimensionalTreeNode<T> *p,int D) {
//在p的右子树中寻找划分维度为D的最小点
    bool mark=true;
    queue<MINNODE<T>*> q;
    MINNODE<T> *x=new MINNODE<T>();
    x->pm=p; x->m=p->rightChild; x->dimension=(D+1)%k;
    q.push(x);
    while(!q.empty()) {
        MINNODE<T> *t=q.front();
        q.pop();

        if(mark) {
            minNode=t;
            mark=false;
        }
        else {
            if(minNode->m->data[D] > t->m->data[D])
                minNode=t;
        }

        if(t->m->leftChild!=NULL) {
            MINNODE<T> *y=new MINNODE<T>(t->m,t->m->leftChild,(t->dimension+1)%k);
            q.push(y);
        }

        if(t->m->rightChild!=NULL) {
            MINNODE<T> *y=new MINNODE<T>(t->m,t->m->rightChild,(t->dimension+1)%k);
            q.push(y);
        }
    }
}

template <class T>
kDimensionalTreeNode<T>* kDimensionalTree<T>::nearestSearch(kDimensionalTreeNode<T> *s,vector<T> &target,int D) {
    if(s==NULL)
        return NULL;
    //1.进行二叉查找，建立搜索路径，直到找到一个叶结点或只有一个孩子的节点
    //二分查找target结点应该落在哪个区域
    kDimensionalTreeNode<T> *currentNode=s;
    int d=D;
    stack<pair<kDimensionalTreeNode<T>*,int>> searchStack;
    while(currentNode!=NULL) {
        searchStack.push(make_pair(currentNode,d));
        if(target[d]<currentNode->data[d])
            currentNode=currentNode->leftChild;
        else
            currentNode=currentNode->rightChild;
        d=(d+1)%k;
    }
    //2.设置以s为根的树中的初始最近邻点i_nearest_node
    pair<kDimensionalTreeNode<T>*,int> iNearestNode=searchStack.top();
    if(nearestDistance > getDistance(iNearestNode.first->data,target)) {
        nearestNode=iNearestNode.first;
        nearestDistance=getDistance(iNearestNode.first->data,target);
    }

    //3.回溯更新全局最近邻点nearest_node
    while(!searchStack.empty()) {
        currentNode=searchStack.top().first;
        int curD=searchStack.top().second;
        searchStack.pop();

        if(nearestDistance > getDistance(currentNode->data,target)) {
            nearestNode=currentNode;
            nearestDistance=getDistance(currentNode->data,target);
        }

        if(fabs(currentNode->data[curD]-target[curD]) < nearestDistance) {
            if(target[curD] < currentNode->data[curD])
                nearestSearch(currentNode->rightChild,target,curD);
            else
                nearestSearch(currentNode->leftChild,target,curD);
        }
    }
    return nearestNode;
}
/*
template <class T>
kDimensionalTreeNode<T>* kDimensionalTree<T>::nearestSearch(kDimensionalTreeNode<T> *s,vector<T> &target,int D) {
    if(s==NULL)
        return NULL;
    //1.进行二叉查找，建立搜索路径，直到找到一个叶结点或者只有一个孩子的节点
    //二分查找target结点应该落在哪个区域
    kDimensionalTreeNode<T> *currentNode=s;
    int d=D;
    stack<pair<kDimensionalTreeNode<T>*,int>> searchStack;
    while(currentNode!=NULL) {
        searchStack.push(make_pair(currentNode,d));
        if(target[d]<currentNode->data[d])
            currentNode=currentNode->leftChild;
        else
            currentNode=currentNode->rightChild;
        d=(d+1)%k;
    }
    //2.设置在以s为根的树中的初始最近邻点i_nearest_node
    pair<kDimensionalTreeNode<T>*,int> iNearestNode=searchStack.top();
    searchStack.pop();
    if(nearestDistance > getDistance(iNearestNode.first->data,target)) {
        nearestNode=iNearestNode.first;
        nearestDistance=getDistance(iNearestNode.first->data,target);
    }
    //3.回溯更新全局最近邻点nearest_node
    if(iNearestNode.first->leftChild!=NULL) { //iNearestNode只有左孩子
        if(fabs(target[iNearestNode.second]-iNearestNode.first->data[iNearestNode.second]) < nearestDistance) {
            nearestSearch(iNearestNode.first->leftChild,target,(iNearestNode.second+1)%k);
        }
    }
    else if(iNearestNode.first->rightChild!=NULL) { //iNearestNode只有右孩子
        if(fabs(target[iNearestNode.second]-iNearestNode.first->data[iNearestNode.second]) < nearestDistance) {
            nearestSearch(iNearestNode.first->rightChild,target,(iNearestNode.second+1)%k);
        }
    }
    else { //iNearestNode是叶节点
        while(!searchStack.empty()) {
            currentNode=searchStack.top().first;
            int curD=searchStack.top().second;
            searchStack.pop();

            if(nearestDistance > getDistance(currentNode->data,target)) {
                nearestNode=currentNode;
                nearestDistance=getDistance(currentNode->data,target);
            }

            if(fabs(currentNode->data[curD]-target[curD]) < nearestDistance) {//相交
                if(target[curD] < currentNode->data[curD])
                    nearestSearch(currentNode->rightChild,target,curD);
                else
                    nearestSearch(currentNode->leftChild,target,curD);
            }
        }
    }
    return nearestNode;
}
*/

template <class T>
void kDimensionalTree<T>::kNearestSearch(kDimensionalTreeNode<T> *s, vector<T> &target, int D) {
    if(s==NULL)
        return ;
    //1.进行二叉查找，建立搜索路径，直到找到一个叶结点或只有一个孩子的节点
    //二分查找target结点应该落在哪个区域
    kDimensionalTreeNode<T> *currentNode=s;
    int d=D;
    stack<pair<kDimensionalTreeNode<T>*,int>> searchStack;
    while(currentNode!=NULL) {
        searchStack.push(make_pair(currentNode,d));
        if(target[d]<currentNode->data[d])
            currentNode=currentNode->leftChild;
        else
            currentNode=currentNode->rightChild;
        d=(d+1)%k;
    }
    //2.设置以s为根的树中的初始近邻点i_nearest_node
    /*pair<kDimensionalTreeNode<T>*,int> iNearestNode=searchStack.top();
    if(kNearest.top().first > getDistance(iNearestNode.first->data,target)) {
        kNearest.pop();
        kNearest.push(make_pair(getDistance(iNearestNode.first->data,target),iNearestNode.first));
    }*/

    //3.回溯更新全局k近邻点
    while(!searchStack.empty()) {
        currentNode=searchStack.top().first;
        int curD=searchStack.top().second;
        searchStack.pop();

        if(kNearest.top().first==1e9) {
            if(kNearest.size()==knn+1) { //将大根堆的初始值删除
                kNearest.pop();
            }
        }

        if(kNearest.top().first==1e9) {
            if(kNearest.size()<knn+1) { //直接加入大根堆
                kNearest.push(make_pair(getDistance(currentNode->data,target),currentNode));
            }
        }
        else {
            if(kNearest.top().first > getDistance(currentNode->data,target) && kNearest.size()==knn) { //删除后再加入大根堆
                kNearest.pop();
                kNearest.push(make_pair(getDistance(currentNode->data,target),currentNode));
            }
        }


        if(kNearest.top().first==1e9) {
            if(fabs(currentNode->data[curD]-target[curD]) < kNearest.top().first || kNearest.size()<knn+1) {
                if(target[curD] < currentNode->data[curD])
                    kNearestSearch(currentNode->rightChild,target,curD);
                else
                    kNearestSearch(currentNode->leftChild,target,curD);
            }
        }
        else {
            if(fabs(currentNode->data[curD]-target[curD]) < kNearest.top().first || kNearest.size()<knn) {
                if(target[curD] < currentNode->data[curD])
                    nearestSearch(currentNode->rightChild,target,curD);
                else
                    nearestSearch(currentNode->leftChild,target,curD);
            }
        }
    }
    return ;
}

template <class T>
vector<vector<T>>& kDimensionalTree<T>::radiusSearch(vector<T> &x,vector<T> &y,kDimensionalTreeNode<T> *s) {
//范围搜索，递归
    if(s==NULL)
        return radiusVector;
    if(check(x,y,s))
        radiusVector.template emplace_back(s->data);
    if(s->leftChild!=NULL && prob(x,y,s->leftChild))
        radiusSearch(x,y,s->leftChild);
    if(s->rightChild!=NULL && prob(x,y,s->rightChild))
        radiusSearch(x,y,s->rightChild);
    return radiusVector;
}

template <class T>
double kDimensionalTree<T>::getDistance(vector<T> &x,vector<T> &y) const {
//计算两点间的距离
    double dist=0;
    for(int i=0;i<k;i++)
        dist+=(x[i]-y[i])*(x[i]-y[i]);
    return sqrt(dist);
}

template <class T>
bool kDimensionalTree<T>::check(vector<T> &x,vector<T> &y,kDimensionalTreeNode<T> *s) {
//判断s是否在范围内
    for(int i=0;i<k;i++)
        if(s->data[i]<x[i] || s->data[i]>y[i])
            return false;
    return true;
}

template <class T>
bool kDimensionalTree<T>::prob(vector<T> &x,vector<T> &y,kDimensionalTreeNode<T> *s) {
//判断s是否有可能在范围内
    for(int i=0;i<k;i++)
        if(s->data[i]>=x[i] && s->data[i]<=y[i])
            return true;
    return false;
}

template <class T>
kDimensionalTreeNode<T>* kDimensionalTree<T>::loadTree(vector<T> *pre,vector<T> *in,int size) {
//递归加载k-d树结构
    if(size<=0) //序列长度小于等于0，不需要继续构造
        return NULL;
    kDimensionalTreeNode<T>* subRoot=new kDimensionalTreeNode<T>(pre[0]);   //pre[0]为根节点
    int index=0;
    //在中序序列中查找pre[0]
    for(int i=0;i<size;i++)
        if(in[i]==pre[0])
        {
            index=i;
            break;
        }
    //递归构造左子树和右子树
    subRoot->leftChild=loadTree(pre+1,in,index);
    subRoot->rightChild=loadTree(pre+index+1,in+index+1,size-index-1);
    return subRoot;
}

/* ------------------------------------- 公有成员函数 -------------------------------------*/

template <class T>
void kDimensionalTree<T>::preOrderOut(kDimensionalTreeNode<T> *t,ofstream &out) {
    if(t!=NULL)
    {
        for(auto x:t->data) out<<x<<" "; out<<"\n";
        preOrderOut(t->leftChild,out);
        preOrderOut(t->rightChild,out);
    }
}

template <class T>
void kDimensionalTree<T>::inOrderOut(kDimensionalTreeNode<T> *t,ofstream &out) {
    if(t!=NULL)
    {
        inOrderOut(t->leftChild,out);
        for(auto x:t->data) out<<x<<" "; out<<"\n";
        inOrderOut(t->rightChild,out);
    }
}


template <class T>
void kDimensionalTree<T>::initialize(int kk) { //初始化参数k
    k=kk;
}

template <class T>
void kDimensionalTree<T>::buildTree(point<T> *p,int pSize,int kk) {
//根据点的数组p[1:pSize]构建k-d树(中值优化)
    deleteTree();
    root=build(1,pSize,0,p);
    treeSize=pSize;
    k=kk;
}

template <class T>
vector<T>* kDimensionalTree<T>::find(vector<T> &theElement) {
//查询theElement，若存在返回指向theElement的指针，否则返回空指针
    kDimensionalTreeNode<T> *p=root;    //指针p从根节点开始
    int D=0;                            //当前的划分维度

    while(p!=NULL) {
        if(theElement[D] < p->data[D])
            p=p->leftChild;
        else if(theElement[D] > p->data[D])
            p=p->rightChild;
        else {
            if(theElement == p->data)   //成功找到
                return &p->data;
            else
                p=p->rightChild;
        }
        D=(D+1)%k;
    }
    //未找到
    return NULL;
}

template <class T>
int kDimensionalTree<T>::insert(vector<T> &theElement) {
//插入theElement，成功插入返回1，否则返回0
    kDimensionalTreeNode<T> *p=root,*pp=NULL;   //指针p从根节点开始
    int D=0;                                    //p所在的划分维度

    while(p!=NULL) {
        pp=p;
        if(theElement[D] < p->data[D])
            p=p->leftChild;
        else if(theElement[D] > p->data[D])
            p=p->rightChild;
        else {
            if(theElement == p->data)   //已经存在
                return 0;
            else
                p=p->rightChild;
        }
        D=(D+1)%k;
    }
    D=(D-1+k)%k;    //pp所在的划分维度
    //建立新节点
    kDimensionalTreeNode<T> *newNode=new kDimensionalTreeNode<T>(theElement);
    if(root!=NULL)  //k-d树非空
    {
        if(theElement[D] < pp->data[D])
            pp->leftChild=newNode;
        else
            pp->rightChild=newNode;
    }
    else    //插入到空的k-d树中
        root=newNode;

    treeSize++;
    return 1;
}

template <class T>
int kDimensionalTree<T>::erase(vector<T> &theElement) {
//删除theElement，成功删除返回1，否则返回0
    if(root==NULL)
        return 0;

    kDimensionalTreeNode<T> *p=root,*pp=NULL;   //指针p从根节点开始
    int D=0;                                    //p所再的划分维度

    while(p!=NULL && p->data!=theElement) {
        pp=p;
        if(theElement[D] < p->data[D])
            p=p->leftChild;
        else if(theElement[D] > p->data[D])
            p=p->rightChild;
        else {
            if(theElement == p->data)   //待删除的节点为p
                break;
            else
                p=p->rightChild;
        }
        D=(D+1)%k;
    }

    if(p==NULL)     //不存在要删除的节点
        return 0;

    if(p->leftChild==NULL && p->rightChild==NULL) {//要删除的是叶节点
        if(p==pp->leftChild) {
            pp->leftChild=NULL;
            delete p;
        }
        else {
            pp->rightChild=NULL;
            delete p;
        }
    }
    else if(p->rightChild!=NULL) {//要删除的节点有右孩子
        findMin(p,D);
        p->data=minNode->m->data;
        eraseNode();
    }
    else {  //要删除的节点没有右孩子，有左孩子：交换左右孩子，转换为有右孩子的情况
        //交换左右孩子
        kDimensionalTreeNode<T> *temp=p->leftChild;
        p->leftChild=p->rightChild;
        p->rightChild=temp;
        //转换为有右孩子的情况
        findMin(p,D);
        p->data=minNode->m->data;
        eraseNode();
    }

    treeSize--;
    return 1;
}

template <class T>
void kDimensionalTree<T>::deleteTree() { //删除整个k-d树
    postOrder(dispose);
    root=NULL;
    treeSize=0;
}

template <class T>
void kDimensionalTree<T>::rearrange() { //重构k-d树，使之平衡
    if(root==NULL) {
        return ;
    }
    point<T> *dp=new point<T>[treeSize+1];
    kDimensionalTreeNode<T> *p=root;
    queue<kDimensionalTreeNode<T>*> q;
    q.push(p);
    int cnt=1;
    while(!q.empty()) {
        kDimensionalTreeNode<T> *x=q.front();
        q.pop();
        dp[cnt++].data=x->data;
        if(x->leftChild!=NULL) {
            q.push(x->leftChild);
        }
        if(x->rightChild!=NULL) {
            q.push(x->rightChild);
        }
    }
    buildTree(dp,treeSize,k);
    delete []dp;
}

template <class T>
void kDimensionalTree<T>::tempVisualization() {
//k-d树可视化，文件名为temp.dot和temp.jpg
    ofstream out("temp\\temp.dot");
    out << "digraph g {\n";
    out << "node [shape = Mrecord, style = filled, color = black, fontcolor = white];\n";
    kDimensionalTreeNode<T> *p=root;
    queue<pair<kDimensionalTreeNode<T>*,int>> q;
    int num=1;
    q.push(make_pair(p,1));
    while(!q.empty()) {
        pair<kDimensionalTreeNode<T>*,int> x=q.front();
        q.pop();

        out << x.second;
        out << "[label = \"<f0> | <f1> (";
        for(int i=0;i<k-1;i++)
            out << x.first->data[i] << ",";
        out << x.first->data[k-1];
        out << ") | <f2>\",color = black, fontcolor = white, style = filled];\n";

        if(x.first->leftChild!=NULL) {
            num++;
            q.push(make_pair(x.first->leftChild,num));
            out << num;
            out << "[label = \"<f0> | <f1> (";
            for(int i=0;i<k-1;i++)
                out << x.first->leftChild->data[i] << ",";
            out << x.first->leftChild->data[k-1];
            out << ") | <f2>\",color = black, fontcolor = white, style = filled];\n";
            out << x.second << ":f0:sw -> " << num << ":f1\n";
        }

        if(x.first->rightChild!=NULL) {
            num++;
            q.push(make_pair(x.first->rightChild,num));
            out << num;
            out << "[label = \"<f0> | <f1> (";
            for(int i=0;i<k-1;i++)
                out << x.first->rightChild->data[i] << ",";
            out << x.first->rightChild->data[k-1];
            out << ") | <f2>\",color = black, fontcolor = white, style = filled];\n";
            out << x.second << ":f2:se -> " << num << ":f1\n";

        }
    }
    out << "}\n";
    out.close();
    system("dot -Tjpg temp\\temp.dot -o temp\\temp.jpg");
    system("temp\\temp.jpg");
}

template <class T>
void kDimensionalTree<T>::visualization(string &strDot,string &strImage) {
//k-d树可视化，指定文件名
    ofstream out(strDot);
    out << "digraph g {\n";
    out << "node [shape = Mrecord, style = filled, color = black, fontcolor = white];\n";
    kDimensionalTreeNode<T> *p=root;
    queue<pair<kDimensionalTreeNode<T>*,int>> q;
    int num=1;
    q.push(make_pair(p,1));
    while(!q.empty()) {
        pair<kDimensionalTreeNode<T>*,int> x=q.front();
        q.pop();

        out << x.second;
        out << "[label = \"<f0> | <f1> (";
        for(int i=0;i<k-1;i++)
            out << x.first->data[i] << ",";
        out << x.first->data[k-1];
        out << ") | <f2>\",color = black, fontcolor = white, style = filled];\n";

        if(x.first->leftChild!=NULL) {
            num++;
            q.push(make_pair(x.first->leftChild,num));
            out << num;
            out << "[label = \"<f0> | <f1> (";
            for(int i=0;i<k-1;i++)
                out << x.first->leftChild->data[i] << ",";
            out << x.first->leftChild->data[k-1];
            out << ") | <f2>\",color = black, fontcolor = white, style = filled];\n";
            out << x.second << ":f0:sw -> " << num << ":f1\n";
        }

        if(x.first->rightChild!=NULL) {
            num++;
            q.push(make_pair(x.first->rightChild,num));
            out << num;
            out << "[label = \"<f0> | <f1> (";
            for(int i=0;i<k-1;i++)
                out << x.first->rightChild->data[i] << ",";
            out << x.first->rightChild->data[k-1];
            out << ") | <f2>\",color = black, fontcolor = white, style = filled];\n";
            out << x.second << ":f2:se -> " << num << ":f1\n";

        }
    }
    out << "}\n";
    out.close();
    string todo="dot -Tjpg " + strDot + " -o " + strImage;
    system(todo.c_str());
    todo=strImage;
    system(todo.c_str());
}

template <class T>
kDimensionalTreeNode<T>* kDimensionalTree<T>::nearestSearch(vector<T> &target) {
//最近邻搜索，返回最近邻的点
    nearestNode=NULL;
    nearestDistance=1e9;
    return nearestSearch(root,target,0);
}

template <class T>
priority_queue<pair<double,kDimensionalTreeNode<T>*>>& kDimensionalTree<T>::kNearestSearch(vector<T> &target,int _k) {
    kNearest=priority_queue<pair<double,kDimensionalTreeNode<T>*>>();
    knn=_k;
    kNearest.push(make_pair(1e9, nullptr));
    kNearestSearch(root,target,0);
    return kNearest;
}

template <class T>
vector<vector<T>>& kDimensionalTree<T>::radiusSearch(vector<T> &x,vector<T> &y) {
//范围搜索，返回在范围内的所有点
    radiusVector.clear();
    return radiusSearch(x,y,root);
}

template <class T>
void kDimensionalTree<T>::save(string &str1,string &str2) {
//保存k-d树结构，将前序序列保存至文件str1，将中序序列保存至文件str2
    ofstream fout1,fout2;
    fout1.open(str1);               fout2.open(str2);
    fout1<<k<<"\n";                 fout2<<k<<"\n";
    fout1<<treeSize<<"\n";          fout2<<treeSize<<"\n";
    preOrderOut(root,fout1);    inOrderOut(root,fout2);
    fout1.close();                  fout2.close();
}

template <class T>
void kDimensionalTree<T>::load(string &str1,string &str2) {
//加载k-d树结构，前序序列文件str1，中序序列文件str2
    deleteTree();

    ifstream fin1,fin2;
    fin1.open(str1);
    fin2.open(str2);
    if(!fin1.is_open()) {
        cout<<"Can't open file "<<str1<<".\n\n";
        fin1.close();
        fin2.close();
        return ;
    }
    if(!fin2.is_open()) {
        cout<<"Can't open file "<<str2<<".\n\n";
        fin1.close();
        fin2.close();
        return ;
    }
    fin1>>k;        fin2>>k;
    fin1>>treeSize; fin2>>treeSize;
    vector<T> *p1=new vector<T>[treeSize];
    vector<T> *p2=new vector<T>[treeSize];
    T a,b;
    for(int i=0;i<treeSize;i++) {
        for(int j=0;j<k;j++) {
            fin1>>a;
            fin2>>b;
            p1[i].template emplace_back(a);
            p2[i].template emplace_back(b);
        }
    }
    root=loadTree(p1,p2,treeSize);
    fin1.close();
    fin2.close();
}

template <class T>
void kDimensionalTree<T>::test(string &str1,string &str2) {
//测试数据集
    deleteTree();

    ifstream fin;
    ofstream fout;
    fin.open(str1);
    fout.open(str2);
    if (!fin.is_open()) {
        cout<<"Can't open file "<<str1<<".\n\n";
        fin.close();
        fout.close();
        return ;
    }
    if (!fout.is_open()) {
        cout<<"Can't open file "<<str2<<".\n\n";
        fin.close();
        fout.close();
        return ;
    }

    int n;
    string op;
    fin>>n;
    for(int i=1;i<=n;i++) {
        fin>>op;
        if(op=="initialize") {
            int kk;
            fin>>kk;
            initialize(kk);
        }
        else if(op=="find") {
            vector<int> v;
            int x;
            for(int j=0;j<k;j++) {
                fin>>x;
                v.emplace_back(x);
            }
            auto f=find(v);
            if(f==nullptr)
                fout<<"Not Found."<<"\n";
            else
                fout<<"Found.\n";
        }
        else if(op=="insert") {
            vector<int> v;
            int x;
            for(int j=0;j<k;j++) {
                fin>>x;
                v.emplace_back(x);
            }
            if(insert(v))
                fout<<"Successfully Insert.\n";
            else
                fout<<"Existed.\n";
        }
        else if(op=="erase") {
            vector<int> v;
            int x;
            for(int j=0;j<k;j++) {
                fin>>x;
                v.emplace_back(x);
            }
            if(erase(v))
                fout<<"Successfully Erase.\n";
            else
                fout<<"Not Existed.\n";
        }
        else if(op == "rearrange" || op == "ra") {
            rearrange();
            fout<<"Successfully Rearrange.\n";
        }
        else if(op=="nns") {
            vector<int> v;
            int x;
            for(int j=0;j<k;j++) {
                fin>>x;
                v.emplace_back(x);
            }
            kDimensionalTreeNode<int> *p=nearestSearch(v);
            if(p==nullptr) {
                fout<<"Empty Tree.\n";
            }
            else {
                fout<<"Nearest Node : ";
                for (auto m:p->data)
                    fout<<m<< " ";
                fout<<"Nearest Distance : "<<nearestDistance<<"\n";
            }
        }
        else if (op == "radius search" || op == "rs") {
            vector<int> v1, v2;
            int x;
            for(int j=0;j<k;j++) {
                fin>>x;
                v1.emplace_back(x);
            }
            for(int j=0;j<k;j++) {
                fin>>x;
                v2.emplace_back(x);
            }
            auto f=radiusSearch(v1, v2);
            if(f.empty()) {
                fout<<"None Result.\n";
            }
            else {
                fout<<"Results : \n";
                for(const auto &m:f) {
                    for(auto z:m)
                        fout<<z<<" ";
                    fout<<"\n";
                }
            }
        }
    }
}
