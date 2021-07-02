#ifndef LAB4_SOLUTION2_H
#define LAB4_SOLUTION2_H

#include <vector>

class Edge {
public:
    int to;     //终点的编号
    int cost;   //路径长度
    Edge(int to, int cost) : to(to), cost(cost) {}
    ~Edge() {}
};

class Node {
public:
    std::vector<Edge> edges;    //记录所有出边
    int pre, preMAX, preMIN;    //当前节点的压力值,到达当前节点的压力最大值和最小值
    Node() {
        edges.clear();
    }
    ~Node() {}
};

class Point {
public:
    std::vector<int> mark;
    Point() {}
    ~Point() {}
};

class DAG {
private:
    int n, m;                   //节点个数 边的个数
    int Pmin, Pmax;             //压力最小值 & 加压后的压力
    std::vector<Node> nodes;    //节点
    std::vector<int> ans;       //答案数组
    int *pre;                   //压力数组
    int leastBoosterCnt, curBoosterCnt; //最少的放大器数量,当前方案的放大器数量

public:
    DAG();
    ~DAG();
    int solve();
    bool check(Point &p);
    void output();
    void output(char* dot, char* image);
};

#endif //LAB4_SOLUTION2_H