#include "minLoserTree.h"
#include "mainwindow.h"
#include <iostream>
#include <cmath>

using namespace std;

template class minLoserTree<int>;
template class minLoserTree<sortPlayer>;

template <class T>
void minLoserTree<T>::play(int p,int leftPlayer, int rightPlayer)
{
    tree[p]=loser(leftPlayer,rightPlayer);
    tempWinner[p]=winner(leftPlayer,rightPlayer);

    //若在右孩子处，则可能还有多场比赛
    while(p>1 && p%2)           //在右孩子处
    {
        tree[p/2]=loser(tempWinner[p-1],tempWinner[p]);
        tempWinner[p/2]=winner(tempWinner[p-1],tempWinner[p]);
        p/=2;                   //到父节点
        if(p==1)    //到tree[1]，在tree[0]记录胜者
        {
            tree[0]=winner(tempWinner[2],tempWinner[3]);
        }
    }
}

template <class T>
int minLoserTree<T>::loser(int leftPlayer,int rightPlayer)
{
    if(player[leftPlayer]==player[rightPlayer])
        return rightPlayer;
    return player[leftPlayer]>player[rightPlayer] ? leftPlayer:rightPlayer;
}

template <class T>
int minLoserTree<T>::winner(int leftPlayer,int rightPlayer)
{
    if(player[leftPlayer]==player[rightPlayer])
        return leftPlayer;
    return player[leftPlayer]<player[rightPlayer] ? leftPlayer:rightPlayer;
}

template <class T>
void minLoserTree<T>::initialize(T *thePlayer, int theNumberOfPlayers)
{//用数组thePlayer[1:numberOfPlayers]生成最小输者树
    //player=thePlayer;
    player=new T[theNumberOfPlayers+1];
    for(int i=1;i<=theNumberOfPlayers;i++)
        player[i]=thePlayer[i];
    tree=new int[theNumberOfPlayers];
    tempWinner=new int[theNumberOfPlayers];
    n=theNumberOfPlayers;
    s=pow(2,floor(logf((float)(n-1)) / logf((float)2)));
    lowExt=2*(n-s);
    offset=2*s-1;

    int i;
    //最底层外部节点的比赛
    for(i=2;i<=lowExt;i+=2)     //选取右孩子进行比赛
        play((i+offset)/2,i-1,i);
    //其余外部节点间进行比赛
    if(n%2)                     //若n为奇数，有一个内部节点和外部节点进行比赛
    {
        play(n/2,tempWinner[n-1],lowExt+1);
        i=lowExt+3;             //下一个外部节点
    }
    else
        i=lowExt+2;             //若n为偶数，加2后为右孩子

    //i为倒数第二层剩余的右孩子节点
    for(;i<=n;i+=2)
        play((i-lowExt+n-1)/2,i-1,i);

    tree[0]=tempWinner[1];
}

template <class T>
int minLoserTree<T>::getWinnerIndex() const
{//返回赢者的索引
    return tree[0];
}

template <class T>
T minLoserTree<T>::getWinnerValue() const
{//返回赢者的索引
    return player[tree[0]];
}

template <class T>
void minLoserTree<T>::rePlay(T newValue)
{//在上一场比赛的赢者的分数变化后重赛
    player[tree[0]]=newValue;
    int p,leftChild,rightChild;   //比赛节点，左孩子，右孩子
    int thePlayer=tree[0];
    //从下到上找到第一个比赛节点及其孩子
    if(thePlayer<=lowExt)               //若thePlayer位于最外层
    {
        p=(offset+thePlayer)/2;         //父节点的位置
        leftChild=2*p-offset;           //p的左孩子
        rightChild=leftChild+1;         //p的右孩子
    }
    else                    //thePlayer不在最外层
    {
        p=(thePlayer-lowExt+n-1)/2;
        if(2*p==n-1)        //一个外部节点与一个内部节点进行比赛
        {
            leftChild=tree[2*p];
            rightChild=thePlayer;
        }
        else                //两个外部节点进行比赛
        {
            leftChild=2*p+lowExt-n+1;
            rightChild=leftChild+1;
        }
    }
    for (;p>=1;p/=2)
    {
        int oldLoser=tree[p];       //上一场比赛的输者
        tree[p]=loser(oldLoser,thePlayer);
        tempWinner[p]=winner(oldLoser,thePlayer);
        thePlayer=tempWinner[p];    //这场比赛的赢者
    }
    tree[0]=tempWinner[1];              //最终的赢者
}

template <class T>
void minLoserTree<T>::rePlay(int thePlayer,T newValue)
{//在参赛者thePlayer的分数变为newValue后重赛
    player[thePlayer]=newValue;
    int p,leftChild,rightChild;   //比赛节点，左孩子，右孩子
    //从下到上找到第一个比赛节点及其孩子
    if(thePlayer<=lowExt)               //若thePlayer位于最外层
    {
        p=(offset+thePlayer)/2;         //父节点的位置
        leftChild=2*p-offset;           //p的左孩子
        rightChild=leftChild+1;         //p的右孩子
    }
    else                    //thePlayer不在最外层
    {
        p=(thePlayer-lowExt+n-1)/2;
        if(2*p==n-1)        //一个外部节点与一个内部节点进行比赛
        {
            leftChild=tree[2*p];
            rightChild=thePlayer;
        }
        else                //两个外部节点进行比赛
        {
            leftChild=2*p+lowExt-n+1;
            rightChild=leftChild+1;
        }
    }

    if(thePlayer==tree[0])  //分数变化的是上一场比赛的赢者
    {
        for(;p>=1;p/=2)
        {
            int oldLoser=tree[p];       //上一场比赛的输者
            tree[p]=loser(oldLoser,thePlayer);
            tempWinner[p]=winner(oldLoser,thePlayer);
            thePlayer=tempWinner[p];    //这场比赛的赢者
        }
    }
    else
    {
        tree[p]=loser(leftChild,rightChild);
        tempWinner[p]=winner(leftChild,rightChild);
        if(p==n-1 && n%2==1)        //比赛结点是最后一层的最后一场比赛
        {                           //p/2后，左孩子是晋级的选手，右孩子是外部节点
            p/=2;
            tree[p]=loser(tempWinner[n-1],lowExt+1);
            tempWinner[p]=winner(tempWinner[n-1],lowExt+1);
        }
        p/=2;
        for(;p>=1;p/=2)
        {
            tree[p]=loser(tempWinner[p*2],tempWinner[p*2+1]);
            tempWinner[p]=winner(tempWinner[p*2],tempWinner[p*2+1]);
        }
    }

    tree[0]=tempWinner[1];              //最终的赢者
}
