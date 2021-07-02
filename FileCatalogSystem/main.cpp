#include "catalogTree.h"
#include <iostream>
#include <unistd.h> 
using namespace std;


int main()
{
    catalogTree cT;
    //获取并设置当前工作路径
    char buf[1000];
    memset(buf,'\0',sizeof(buf));
    getcwd(buf,sizeof(buf));
    cT.setWorkDirectory(buf);
    cT.help();                  //显示help界面
    string op;
    cout<<buf<<">";
    while(getline(cin,op))      //执行操作
    {
        if(op[0]=='d' && op[1]=='i')    // dir
        {
            cT.dir();
        }
        else if(op[0]=='c')             // cd or cd .. or cd str
        {
            if(op.size()==2)            // cd
            {
                cT.cd();
            }
            else if(op.size()==5 && op[3]=='.' && op[4]=='.')   // cd ..
            {
                string str="..";
                cT.cd(str);
            }
            else        // cd str
            {
                string str(op,3,op.size());
                cT.cd(str);
            }
        }
        else if(op[0]=='m' && op[1]=='k' && op[2]=='d')     // mkdir str
        {
            string str(op,6,op.size());
            cT.mkdir(str);
        }
        else if(op[0]=='m' && op[1]=='k' && op[2]=='f')     // mkfile str
        {
            string str(op,7,op.size());
            cT.mkfile(str);
        }
        else if((op[0]=='d' && op[1]=='e'))                 // delete str
        {
            string str(op,7,op.size());
            cT.Delete(str);
        }
        else if(op[0]=='s')                 // save str
        {
            string str(op,5,op.size());
            cT.save(str);
        }
        else if(op[0]=='l')                 // load str
        {
            string str(op,5,op.size());
            cT.load(str);
        }
        else if(op[0]=='q')                 // quit
        {
            break;
        }
        else if(op[0]=='?')                 // ?
        {
            cT.help();
        }
        else if(op[0]=='t')                 // test str
        {
            string str(op,5,op.size());
            cT.test(str);
        }
        cout<<"\n";
        cT.getCurrentDirectory();
    }
    return 0;
}
