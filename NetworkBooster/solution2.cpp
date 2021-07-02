#include <cstdio>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>
#include <ctime>
#include <vector>
#include <queue>
#include "solution2.h"

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
    pre=new int[n+1];
    for(int i=1;i<=n;i++) {
        pre[i]=0;
    }
    leastBoosterCnt = n + 1;
}

DAG::~DAG() {
    delete[] pre;
}

int DAG::solve() {
    Point p1;
    p1.mark.emplace_back(0);
    p1.mark.emplace_back(0);
    ans.emplace_back(0);
    queue<Point> q;
    q.push(p1);
    while(!q.empty()) {
        Point p=q.front();
        q.pop();
        if(p.mark.size() < n+1) {
            if(p.mark[0] < leastBoosterCnt) {
                if(check(p)) {
                    p.mark.emplace_back(0);
                    q.push(p);
                    p.mark[p.mark.size()-1]=1;
                    p.mark[0]++;
                    q.push(p);
                }
            }
        }
        else {
            if(check(p) && curBoosterCnt < leastBoosterCnt) {
                leastBoosterCnt = curBoosterCnt;
                for (int k = 1; k <= n; k++) {
                    ans.emplace_back(p.mark[k]);
                    pre[k] = nodes[k].pre;
                }
            }
        }
    }
    return leastBoosterCnt;
}

bool DAG::check(Point &p) {
    for (int i = 1; i <= n; i++) {
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
    for(int i=2;i<p.mark.size();i++) {
        if (nodes[i].preMIN < Pmin)
            return false;

        if (p.mark[i]) {   //该节点放置放大器
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

void DAG::output() {
    ofstream out("dot\\solve2.dot");
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
    system("dot -Tjpg dot\\solve2.dot -o image\\solve2.jpg");
}

void DAG::output(char* dot, char* image) {
    string sdot(dot);
    string simage(image);
    ofstream out("dot2\\"+sdot);

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
    string todo="dot -Tjpg dot2\\"+ sdot +" -o image2\\" + simage;
    system(todo.c_str());
}

int main(int argc, char **argv) {
    DAG dag;
    cout<<dag.solve();
    if(argc==2 && strcmp(argv[1],"1")==0) {     //测试单个文件生成图像
        dag.output();
    }
    if(argc==3) {
        dag.output(argv[1],argv[2]);//测试数据集生成图像
    }
    return 0;
}