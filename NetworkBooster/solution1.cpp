#include <cstdio>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>
#include <ctime>
#include <vector>
#include "solution1.h"

using namespace std;


DAG::DAG() {
    cin >> n >> m >> Pmax;
    Pmin = 0;
    nodes.clear();
    // 初始化元素
    for (int i = 0; i <= n; ++i)
        nodes.push_back(Node());
    // 读入m条边
    for (int i = 1; i <= m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        nodes[u].edges.emplace_back(v, w);
    }
    mark = new int[n + 1];
    ans = new int[n + 1];
    pre = new int[n + 1];

    for (int i = 0; i < n + 1; i++) {
        mark[i] = 0;
        ans[i] = 0;
        pre[i] = 0;
    }
    leastBoosterCnt = n + 1;
}

DAG::~DAG() {
    delete[] mark;
    delete[] ans;
    delete[] pre;
}

int DAG::solve() {
    rSubset(2);
    return leastBoosterCnt;
}

bool DAG::check(int x) {
    //对每个node的preMAX和preMIN进行初始化设置
    //curBoosterCnt的初始化
    for (int i = 1; i <= x; i++) {
        nodes[i].preMIN = Pmax;
        nodes[i].preMAX = -1e9;
    }
    curBoosterCnt = 0;
    nodes[1].pre=Pmax;

    //从源点S开始计算
    for (auto x:nodes[1].edges) {
        nodes[x.to].preMIN = min(nodes[x.to].preMIN, Pmax - x.cost);
        nodes[x.to].preMAX = max(nodes[x.to].preMAX, Pmax - x.cost);
    }

    for (int i = 2; i <= x; i++) {
        if (nodes[i].preMIN < Pmin)
            return false;

        if (mark[i]) {   //该节点放置放大器
            curBoosterCnt++;
            nodes[i].pre = Pmax;
        } else {  //该节点不放置放大器
            nodes[i].pre = nodes[i].preMAX;
        }

        for (auto x:nodes[i].edges) {
            nodes[x.to].preMIN = min(nodes[x.to].preMIN, nodes[i].pre - x.cost);
            nodes[x.to].preMAX = max(nodes[x.to].preMAX, nodes[i].pre - x.cost);
        }
    }
    return true;
}

void DAG::rSubset(int i) {
    if (i == n + 1) {  //已经确定了所有放大器的放置位置
        if (check(i-1) && curBoosterCnt < leastBoosterCnt) {   //方案合法且比当前方案更优
            leastBoosterCnt = curBoosterCnt;
            for (int k = 1; k <= n; k++) {
                ans[k] = mark[k];
                pre[k] = nodes[k].pre;
            }
            return;
        } else {  //方案不合法或方案合法但不是更优
            return;
        }
    }
    else
    {
        if(check(i-1) && mark[0] < leastBoosterCnt) {
            mark[i] = 0;
            rSubset(i + 1);
            mark[i] = 1;
            mark[0]++;
            rSubset(i + 1);
            mark[0]--;
        }
        else {
            return ;
        }
    }
}

void DAG::output() {
    ofstream out("dot\\solve1.dot");
    out << "digraph g {\n";
    for (int i = 1; i < nodes.size(); i++) {
        out << i;
        if(ans[i]==0)
            out << "[label=\"node" << i << ": pre=" << pre[i] << "\",style=filled, fillcolor=white];\n";
        else
            out << "[label=\"node" << i << ": pre=" << pre[i] << "\",style=filled, fillcolor=red];\n";
    }
    for (auto i = 1; i <= n; ++i) {
        for (auto &edge : nodes[i].edges) {
            out << i << "->" << edge.to << "[label=\"cost=" << edge.cost << "\"];\n";
        }
    }
    out << "}\n";
    out.close();
    system("dot -Tjpg dot\\solve1.dot -o image\\solve1.jpg");
}

void DAG::output(char* dot, char* image) {
    string sdot(dot);
    string simage(image);
    ofstream out("dot1\\"+sdot);

    out << "digraph g {\n";
    for (int i = 1; i < nodes.size(); i++) {
        out << i;
        if(ans[i]==0)
            out << "[label=\"node" << i << ": pre=" << pre[i] << "\",style=filled, fillcolor=white];\n";
        else
            out << "[label=\"node" << i << ": pre=" << pre[i] << "\",style=filled, fillcolor=red];\n";
    }
    for (auto i = 1; i <= n; ++i) {
        for (auto &edge : nodes[i].edges) {
            out << i << "->" << edge.to << "[label=\"cost=" << edge.cost << "\"];\n";
        }
    }
    out << "}\n";
    out.close();
    string todo="dot -Tjpg dot1\\"+ sdot +" -o image1\\" + simage;
    system(todo.c_str());
}

int main(int argc, char **argv) {
    DAG dag;
    cout<<dag.solve();
    if(argc==2 && strcmp(argv[1],"1")==0)       //测试单个文件生成图像
        dag.output();
    if(argc==3) {
        dag.output(argv[1],argv[2]);//测试数据集生成图像
    }
    return 0;
}