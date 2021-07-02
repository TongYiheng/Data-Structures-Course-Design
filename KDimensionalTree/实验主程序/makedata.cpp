#include <bits/stdc++.h>
using namespace std;

/* Please ensure the number of all cases is greater than SIZE. */
/* Or it will lead to endless loop. */

#define K 4
#define SIZE 1000
#define L 50
#define R 200

int main() {
    freopen("data.txt","w",stdout);
    set<vector<int>> s;
    cout<<K<<" "<<SIZE<<"\n";
    srand(time(NULL));
    for(int i=1;i<=SIZE;i++) {
        vector<int> v;
        for(int j=1;j<=K;j++) {
            int x=rand()%(R-L+1)+L;
            v.emplace_back(x);
        }
        if(s.count(v)) { //duplicate
            i--;
        }
        else {
            for(auto x:v)
                cout<<x<<" ";
            cout<<"\n";
            s.insert(v);
        }
        
    }
    return 0;
}