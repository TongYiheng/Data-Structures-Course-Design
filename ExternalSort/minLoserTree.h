#ifndef MINLOSERTREE_H
#define MINLOSERTREE_H

template <class T>
class minLoserTree {
private:
    int* tree;          //内部节点 1:n-1为每场比赛输者的索引 0记录整场比赛的胜者
    int* tempWinner;    //初始化过程中临时记录每场比赛的胜者
    T* player;          //外部节点（选手）1:n
    int n;              //选手个数
    int s;              //最底层最左端的内部节点
    int lowExt;         //最底层外部节点个数
    int offset;         //偏移
    void play(int p,int leftPlayer,int rightPlayer);
    int loser(int leftPlayer,int rightPlayer);
    int winner(int leftPlayer,int rightPlayer);
public:
    minLoserTree() {}
    ~minLoserTree()
    {
        delete []player;
        delete []tree;
        delete []tempWinner;
    }
    void initialize(T* thePlayer,int theNumberOfPlayers);   //用数组thePlayer[1:numberOfPlayers]生成最小输者树
    int getWinnerIndex() const;                             //返回赢者的索引
    T getWinnerValue() const;                               //返回赢者的值
    void rePlay(T newValue);                                //在上一场比赛的赢者的分数变化后重赛
    void rePlay(int thePlayer,T newValue);                  //在参赛者thePlayer的分数变化后重赛
};

#endif // MINLOSERTREE_H
