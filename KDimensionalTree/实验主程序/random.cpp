#include <bits/stdc++.h>
using namespace std;

/* All operation types are as followed : 
   1. initialize <k>
   2. find <element>
   3. insert <element>
   4. erase <element>
   5. rearrange
   6. nns <target>
   7. rs <radius>
*/

#define K 4
#define N 1000
#define L 50
#define R 200

int main() {
    freopen("random.txt","w",stdout);
    set<vector<int>> s;
    cout<<N<<"\n";
    /* The first operation must be "initialize <k>". */
    cout<<"initialize "<<K<<"\n";
    srand(time(NULL));
    for(int i=2;i<=N;i++) {
        int op=rand()%6;
        if(op==0) { // find
            cout<<"find ";
            vector<int> v;
            for(int j=1;j<=K;j++) {
                int x=rand()%(R-L+1)+L;
                v.emplace_back(x);
            }
            for(auto x:v)
                cout<<x<<" ";
            cout<<"\n";
        }
        else if(op==1) { // insert
            cout<<"insert ";
            vector<int> v;
            for(int j=1;j<=K;j++) {
                int x=rand()%(R-L+1)+L;
                v.emplace_back(x);
            }
            for(auto x:v)
                cout<<x<<" ";
            cout<<"\n";
            s.insert(v);
        }
        else if(op==2) { // erase
            cout<<"erase ";
            vector<int> v;
            for(int j=1;j<=K;j++) {
                int x=rand()%(R-L+1)+L;
                v.emplace_back(x);
            }
            for(auto x:v)
                cout<<x<<" ";
            cout<<"\n";
            s.erase(v);
        }
        else if(op==3) { // rearrange
            cout<<"rearrange"<<"\n";
        }
        else if(op==4) { // nns
            cout<<"nns ";
            vector<int> v;
            for(int j=1;j<=K;j++) {
                int x=rand()%(R-L+1)+L;
                v.emplace_back(x);
            }
            for(auto x:v)
                cout<<x<<" ";
            cout<<"\n";
        }
        else if(op==5) { // rs
            cout<<"rs ";
            vector<int> l,r;
            for(int j=0;j<K;j++) {
                int x=rand()%(R-L/2+1)+L/2;
                l.emplace_back(x);
            }
            for(int j=0;j<K;j++) {
                int offset=rand()%(R-L+1);
                r.emplace_back(l[j]+offset);
            }
            for(auto x:l)
                cout<<x<<" ";
            for(auto x:r)
                cout<<x<<" ";
            cout<<"\n";
        }  
    }
    return 0;
}