#include <bits/stdc++.h>
#include "kDimensionalTree.h"

using namespace std;

void help() {
    printf("-----------------------------------K-Dimensional Tree Program-----------------------------------\n");
    printf("1.initialize(ini)                    -  initialize k-d tree if not load or build from file\n");
    printf("2.build(bui)                         -  build k-d tree from file\n");
    printf("3.find                               -  find element in k-d tree\n");
    printf("4.insert(ins)                        -  insert element into k-d tree\n");
    printf("5.erase(era)                         -  erase element in k-d tree\n");
    printf("6.delete(del)                        -  erase all elements in k-d tree\n");
    printf("7.rearrange(ra)                      -  rebuild a balanced k-d tree\n");
    printf("8.nearest neighbor search(nns)       -  search the nearest neighbor of target in k-d tree\n");
    printf("9.k nearest neighbor search(knns)    -  search k nearest neighbors of target in k-d tree\n");
    printf("10.radius search(rs)                 -  search all elements inside the radius in k-d tree\n");
    printf("11.visualization(vis)                -  generate image\n");
    printf("12.visualization -i(vis -i)          -  generate image with specific file name\n");
    printf("13.show                              -  show parameters of k-d tree\n");
    printf("14.save                              -  save structure of k-d tree\n");
    printf("15.load                              -  load structure of k-d tree\n");
    printf("16.test                              -  test input file named str\n");
    printf("17.?                                 -  display this help menu\n");
    printf("18.quit(q)                           -  exit the program\n\n");
}

int main() {
    kDimensionalTree<int> TDT;
    int k=2;
    help();
    cout<<"> ";

    string op;
    while(getline(cin,op)) {
        if(op=="initialize" || op=="ini") {
            cout<<"Please input the value of k : ";
            int kk;
            cin>>kk;
            if(kk<1)
                cout<<"k must be >= 1."<<"\n\n";
            else {
                TDT.initialize(kk);
                k=kk;
                cout<<"Successfully Initialize.\n\n";
            }
            cin.ignore();
        }
        else if(op=="build" || op=="bui") {
            cout<<"Please input the data file name : ";
            string str;
            cin>>str;
            ifstream fin;
            fin.open(str);
            if(fin.is_open()) {
                int kk,size,x;
                fin>>kk>>size;
                k=kk;
                point<int> *p=new point<int>[size+1];
                for(int i=1;i<=size;i++) {
                    for(int j=0;j<kk;j++) {
                        fin>>x;
                        p[i].data.emplace_back(x);
                    }
                }
                TDT.buildTree(p,size,kk);
                delete []p;
                fin.close();
                cout<<"Successfully Build.\n\n";
            }
            else {
                cout<<"Can't open the file.\n\n";
            }
            cin.ignore();
        }
        else if(op=="find") {
            cout<<"Please input the target element : ";
            vector<int> v;
            int x;
            for(int i=0;i<k;i++) {
                cin>>x;
                v.emplace_back(x);
            }
            if(v.size()==k) {
                auto f=TDT.find(v);
                if(f==nullptr) {
                    cout<<"Not Found."<<"\n\n";
                }
                else {
                    /*for(int i=0;i<k;i++)
                        cout<<f->at(i)<<" ";
                    cout<<"\n\n";*/
                    cout<<"Found.\n\n";
                }
            }
            else {
                cout<<"Element Fault."<<"\n\n";
            }
            cin.ignore();
        }
        else if(op=="insert" || op=="ins") {
            cout<<"Please input the target element : ";
            vector<int> v;
            int x;
            for(int i=0;i<k;i++) {
                cin>>x;
                v.emplace_back(x);
            }
            if(v.size()==k) {
                if(TDT.insert(v))
                    cout<<"Successfully Insert.\n\n";
                else
                    cout<<"Existed.\n\n";
            }
            else {
                cout<<"Element Fault."<<"\n\n";
            }
            cin.ignore();
        }
        else if(op=="erase" || op=="era") {
            cout<<"Please input the target element : ";
            vector<int> v;
            int x;
            for(int i=0;i<k;i++) {
                cin>>x;
                v.emplace_back(x);
            }
            if(v.size()==k) {
                if(TDT.erase(v))
                    cout<<"Successfully Erase.\n\n";
                else
                    cout<<"Not Existed.\n\n";
            }
            else {
                cout<<"Element Fault."<<"\n\n";
            }
            cin.ignore();
        }
        else if(op=="delete" || op=="del") {
            cout<<"Confirm? (Y/N)\n";
            string str;
            cin>>str;
            if(str=="Y" || str=="y") {
                TDT.deleteTree();
                cout<<"Successfully Delete.\n\n";
            }
            else {
                cout<<"Not Delete.\n\n";
            }
            cin.ignore();
        }
        else if(op=="rearrange" || op=="ra") {
            TDT.rearrange();
            cout<<"Successfully Rearrange.\n\n";
        }
        else if(op=="nearest neighbor search" || op=="nns") {
            cout<<"Please input the target element : ";
            vector<int> v;
            int x;
            for(int i=0;i<k;i++) {
                cin>>x;
                v.emplace_back(x);
            }
            if(v.size()==k) {
                kDimensionalTreeNode<int>* p=TDT.nearestSearch(v);
                if(p==nullptr) {
                    cout<<"Empty Tree.\n\n";
                }
                else {
                    cout<<"Nearest Node : ";
                    for(auto m:p->data)
                        cout<<m<<" ";
                    cout<<"\n";
                    cout<<"Nearest Distance : "<<TDT.getNearestDistance()<<"\n\n";
                }
            }
            else {
                cout<<"Element Fault."<<"\n\n";
            }
            cin.ignore();
        }
        else if(op=="k nearest neighbor search" || op=="knns") {
            cout<<"Please input the value of k : ";
            int knn;
            cin>>knn;
            cout<<"Please input the target element : ";
            vector<int> v;
            int x;
            for(int i=0;i<k;i++) {
                cin>>x;
                v.emplace_back(x);
            }
            if(v.size()==k) {
                vector<pair<kDimensionalTreeNode<int>*,double>> ans;
                priority_queue<pair<double,kDimensionalTreeNode<int>*>> q=TDT.kNearestSearch(v,knn);
                while(!q.empty()) {
                    auto z=q.top();
                    q.pop();
                    ans.template emplace_back(z.second,z.first);
                }

                if(ans.empty()) {
                    cout<<"Empty Tree.\n\n";
                }
                else {
                    cout<<"K Nearest Node and Distance : \n";
                    if(ans[0].first==NULL)
                        ans.erase(ans.begin());
                    for(auto m:ans) {
                        for(auto z:m.first->data) {
                            cout<<z<<" ";
                        }
                        cout<<"distance : "<<m.second<<"\n";
                    }
                    cout<<"\n\n";
                }
            }
            else {
                cout<<"Element Fault."<<"\n\n";
            }
            cin.ignore();
        }
        else if(op=="radius search" || op=="rs") {
            cout<<"Please input the radius : \n";
            cout<<"(Format : The first line with k values represents the lower bound and the second line with k values represents the upper bound.)\n";
            vector<int> v1,v2;
            int x;
            for(int i=0;i<k;i++) {
                cin>>x;
                v1.emplace_back(x);
            }
            for(int i=0;i<k;i++) {
                cin>>x;
                v2.emplace_back(x);
            }
            if(v1.size()==k && v2.size()==k) {
                auto f=TDT.radiusSearch(v1,v2);
                if(f.empty()) {
                    cout<<"None Result.\n\n";
                }
                else {
                    cout<<"Results : \n";
                    for(const auto& m:f) {
                        for(auto z:m)
                            cout<<z<<" ";
                        cout<<"\n";
                    }
                    cout<<"\n\n";
                }
            }
            else {
                cout<<"Format Fault."<<"\n\n";
            }
            cin.ignore();
        }
        else if(op=="visualization" || op=="vis") {
            TDT.tempVisualization();
            cout<<"\n";
        }
        else if(op=="visualization -i" || op=="vis -i") {
            string str1,str2;
            cout<<"Please input dot file name and image file name : ";
            cin>>str1>>str2;
            TDT.visualization(str1,str2);
            cout<<"Successfully Generated.\n\n";
            cin.ignore();
        }
        else if(op=="show") {
            cout<<"k : "<<TDT.getK()<<"\n";
            cout<<"size : "<<TDT.size()<<"\n\n";
        }
        else if(op=="save") {
            string str1,str2;
            cout<<"Please input two file names to save the data (preorder && inorder) : ";
            cin>>str1>>str2;
            TDT.save(str1,str2);
            cout<<"Successfully Save.\n\n";
            cin.ignore();
        }
        else if(op=="load") {
            string str1,str2;
            cout<<"Please input two file names to load the data (preorder && inorder) : ";
            cin>>str1>>str2;
            TDT.load(str1,str2);
            k=TDT.getK();
            cout<<"Successfully Load.\n\n";
            cin.ignore();
        }
        else if(op=="test") {
            string str1,str2;
            cout<<"Please input the input file name : ";
            cin>>str1;
            cout<<"Please input the output file name : ";
            cin>>str2;
            TDT.test(str1,str2);
            cout<<"Finish.\n\n";
            cin.ignore();
        }
        else if(op=="?") {
            help();
        }
        else if(op=="quit") {
            break;
        }
        else {
            cout<<"Invalid Instruction."<<"\n\n";
        }
        cout<<"> ";
    }

    return 0;
}