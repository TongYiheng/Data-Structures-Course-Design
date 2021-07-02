#include "catalogTree.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>

using namespace std;

ofstream Fout;

catalogTree::catalogTree()
{//构造函数
    root=new catalogTreeNode();
    root->data="/";
    root->isFile=false;
    root->childFileNumber=0;
    root->childCatalogNumber=0;
    root->parent=NULL;
    root->firstChild=NULL;
    root->nextSibling=NULL;
    currentNode=root;
}

catalogTree::~catalogTree()
{//析构函数
    clear();
}

void catalogTree::eraseSubCatalog(catalogTreeNode* p)
{//删除p指向的子目录
    catalogTreeNode* pp=p->firstChild;
    catalogTreeNode* deleteNode;
    while(pp!=NULL)
    {
        if(pp->isFile)
        {
            deleteNode=pp;
            pp=pp->nextSibling;
            delete deleteNode;
        }
        else
        {
            deleteNode=pp;
            pp=pp->nextSibling;
            eraseSubCatalog(deleteNode);
        }
    }
    delete p;
}


void catalogTree::dir()
{//列出当前目录下的所有子目录与文件项
 //所有文件项后加*表示这是一个文件
 //输出顺序按照字典序排序，先输出所有文件再输出所有文件名
    catalogTreeNode* p=currentNode->firstChild;
    while(p!=NULL)
    {
        //输出
        if(p->isFile)
        {//输出一个文件名
            cout<<p->data<<"*"<<"\n";
        }
        else    //输出一个子目录
        {
            cout<<p->data<<"\n";
        }
        p=p->nextSibling;
    }
}

void catalogTree::dir_test()
{//列出当前目录下的所有子目录与文件项
 //所有文件项后加*表示这是一个文件
 //输出顺序按照字典序排序，先输出所有文件再输出所有文件名
    catalogTreeNode* p=currentNode->firstChild;
    while(p!=NULL)
    {
        //输出
        if(p->isFile)
        {//输出一个文件名
            Fout<<p->data<<"*"<<"\n";
        }
        else    //输出一个子目录
        {
            Fout<<p->data<<"\n";
        }
        p=p->nextSibling;
    }
}

void catalogTree::cd()
{//列出当前目录的绝对路径
    catalogTreeNode* p=currentNode;
    stack<catalogTreeNode*> sk;

    //父节点依次入栈
    while(p!=NULL)
    {
        sk.push(p);//入栈
        p=p->parent;
    }

    if(sk.size()==1)
    {
        cout<<workDirectory<<"\\"<<"\n";
        return ;
    }

    sk.pop();   //弹出根节点
    cout<<workDirectory;
    //出栈
    while(!sk.empty())
    {
        cout<<"/"<<sk.top()->data;  //输出
        sk.pop();                   //弹栈
    }
    cout<<"\n";
}

void catalogTree::cd_test()
{//列出当前目录的绝对路径
    catalogTreeNode* p=currentNode;
    stack<catalogTreeNode*> sk;

    //节点依次入栈
    while(p!=NULL)
    {
        sk.push(p);//入栈
        p=p->parent;
    }

    if(sk.size()==1)
    {
        Fout<<"/"<<"\n";
        return ;
    }

    sk.pop();   //弹出根节点
    //出栈
    while(!sk.empty())
    {//注意输出的/的个数
        Fout<<"/"<<sk.top()->data;  //输出
        sk.pop();                   //弹栈
    }
    Fout<<"\n";
}

void catalogTree::cd(string& str)
{//Include "cd.." and "cd str"
    if(str=="..")   //当前目录变为当前目录的父目录
    {
        //如果当前目录已经是根目录，父目录还是本身
        if(currentNode==root)
            return ;
        currentNode=currentNode->parent;
        currentNodePath.pop_back();
    }
    else    //当前目录变为str所表示路径的目录
    {
        //str是绝对路径
        if(str[0]=='/')
        {
            currentNode=root;
            currentNodePath.clear();
            if(str.size()==1) return ;
            string path;
            int i=1;
            while(i<(int)str.size())
            {
                if(str[i]!='/')
                {
                    path.push_back(str[i]);
                    i++;
                }
                else
                {
                    //寻找目录
                    catalogTreeNode* p=currentNode->firstChild;
                    while(p!=NULL && p->isFile)
                        p=p->nextSibling;
                    while(p!=NULL && p->data!=path)
                        p=p->nextSibling;
                    if(p==NULL)
                    {
                        cout<<"系统找不到指定的路径。"<<"\n";
                        return ;
                    }
                    currentNode=p;
                    currentNodePath.emplace_back(path);
                    i++;
                    path.clear();
                }
            }
            //找最后一个目录
            catalogTreeNode* p=currentNode->firstChild;
            while(p!=NULL && p->isFile)
                p=p->nextSibling;
            while(p!=NULL && p->data!=path)
                p=p->nextSibling;
            if(p==NULL)
            {
                cout<<"系统找不到指定的路径。"<<"\n";
                return ;
            }
            currentNode=p;
            currentNodePath.emplace_back(path);
        }
        else    //str是相对路径
        {
            int i=0;
            string path;
            while(i<(int)str.size())
            {
                if(str[i]!='/')
                {
                    path.push_back(str[i]);
                    i++;
                }
                else
                {
                    //寻找目录
                    catalogTreeNode* p=currentNode->firstChild;
                    while(p!=NULL && p->isFile)
                        p=p->nextSibling;
                    while(p!=NULL && p->data!=path)
                        p=p->nextSibling;
                    if(p==NULL)
                    {
                        cout<<"系统找不到指定的路径。"<<"\n";
                        return ;
                    }
                    currentNode=p;
                    currentNodePath.emplace_back(path);
                    i++;
                    path.clear();
                }
            }
            //找最后一个目录
            catalogTreeNode* p=currentNode->firstChild;
            while(p!=NULL && p->isFile)
                p=p->nextSibling;
            while(p!=NULL && p->data!=path)
                p=p->nextSibling;
            if(p==NULL)
            {
                cout<<"系统找不到指定的路径。"<<"\n";
                return ;
            }
            currentNode=p;
            currentNodePath.emplace_back(path);
        }
    }
}

void catalogTree::mkdir(string& str)
{//在当前目录下创建一个子目录（名为str），若存在则不进行任何操作
 //保证顺序就是字典序的顺序
    catalogTreeNode* p=currentNode->firstChild; //p指向当前目录下的第一个孩子

    if(p!=NULL && !p->isFile && p->data==str)
    {
        cout<<"子目录 "<<str<<" 已经存在。"<<"\n";
        return ;
    }  

    //插入位置在第一个孩子的位置
    if(p==NULL || (!p->isFile && p->data>str)) //当前目录还没有孩子 or 第一个孩子是目录且字典序大于str
    {
        catalogTreeNode* newNode=new catalogTreeNode(str,false);
        newNode->parent=currentNode;
        currentNode->firstChild=newNode;
        newNode->nextSibling=p;

        currentNode->childCatalogNumber++;

        //真实建立一个目录
        string op="mkdir ";
        string path=workDirectory;
        for(auto np:currentNodePath)
            path += "\\" + np;
        path = path + "\\" + str;
        op += path;
        system(op.c_str());

        return ;
    }

    //插入位置不在第一个孩子的位置

    //寻找插入位置
    while(p->nextSibling!=NULL && (p->nextSibling->isFile || (!p->nextSibling->isFile && p->nextSibling->data<str)))
        p=p->nextSibling;

    if(p->nextSibling!=NULL && p->nextSibling->data==str)    //存在相同的目录项
    {
        cout<<"子目录 "<<str<<" 已经存在。"<<"\n";
        return ;
    }
    else    //不存在相同的目录项
    {
        catalogTreeNode* newNode=new catalogTreeNode(str,false);
        newNode->parent=currentNode;
        newNode->nextSibling=p->nextSibling;
        p->nextSibling=newNode;
        currentNode->childCatalogNumber++;
        //真实建立一个目录
        string op="mkdir ";
        string path=workDirectory;
        for(auto np:currentNodePath)
            path += "\\" + np;
        path = path + "\\" + str;
        op += path;
        system(op.c_str());
    }
}

void catalogTree::mkfile(string& str)
{//在当前目录下创建一个文件（名为str），若存在则不进行任何操作
    catalogTreeNode* p=currentNode->firstChild; //p指向当前目录下的第一个孩子

    if(p!=NULL && p->isFile && p->data==str)
    {
        cout<<"文件 "<<str<<" 已经存在。"<<"\n";
        return ;
    }

    //插入位置在第一个孩子的位置
    if(p==NULL || (p->isFile && p->data>str) || !p->isFile) //当前目录还没有孩子 or 第一个孩子是文件且字典序大于str or 第一个孩子是子目录
    {
        catalogTreeNode* newNode=new catalogTreeNode(str,true);
        newNode->parent=currentNode;
        currentNode->firstChild=newNode;
        newNode->nextSibling=p;

        currentNode->childFileNumber++;
        //真实建立一个文件
        string op="cd.>";
        string path=workDirectory;
        for(auto np:currentNodePath)
            path += "\\" + np;
        path = path + "\\" + str;
        op += path;
        system(op.c_str());

        return ;
    }

    //插入位置不在第一个孩子的位置

    //寻找插入位置
    while(p->nextSibling!=NULL && p->nextSibling->isFile && p->nextSibling->data<str)
        p=p->nextSibling;

    if(p->nextSibling!=NULL && p->data==str)    //存在相同的文件项
    {
        cout<<"文件 "<<str<<" 已经存在。"<<"\n";
        return ;
    }
    else    //不存在相同的文件项
    {
        catalogTreeNode* newNode=new catalogTreeNode(str,true);
        newNode->parent=currentNode;
        newNode->nextSibling=p->nextSibling;
        p->nextSibling=newNode;
        currentNode->childFileNumber++;
        //真实建立一个文件
        string op="cd.>";
        string path=workDirectory;
        for(auto np:currentNodePath)
            path += "\\" + np;
        path = path + "\\" + str;
        op += path;
        system(op.c_str());
    }
}

void catalogTree::Delete(string& str)
{//删除当前目录下名为str的目录或文件，若不存在则不进行任何操作
    catalogTreeNode* p=currentNode->firstChild;     //p指向当前目录下的第一个孩子

    if(p==NULL) //当前目录没有文件和子目录
        return ;

    int flag=0;

    if(p->data==str)    //第一个孩子需要被删除
    {
        if(p->isFile)   //删除文件
        {
            flag=1;
            catalogTreeNode* deleteNode=p;
            currentNode->firstChild=p->nextSibling;
            p=p->nextSibling;
            //从文件目录系统中删除文件
            delete deleteNode;
            currentNode->childFileNumber--;
            //真实删除文件
            string op="del ";
            string path=workDirectory;
            for(auto np:currentNodePath)
                path += "\\" + np;
            path = path + "\\" + str;
            op += path;
            system(op.c_str());
        }
        else    //删除目录
        {
            catalogTreeNode* deleteNode=p;
            currentNode->firstChild=p->nextSibling;
            p=p->nextSibling;
            //从文件目录系统中删除子目录
            eraseSubCatalog(deleteNode);
            currentNode->childCatalogNumber--;
            //真实删除子目录
            string op="rd/s/q ";
            string path=workDirectory;
            for(auto np:currentNodePath)
                path += "\\" + np;
            path = path + "\\" + str;
            op += path;
            system(op.c_str());

            return ;
        }
    }

    if(p==NULL) //删除第一个孩子文件后没有其他孩子
        return ;

    if(flag)    //第一个孩子为文件且被删除，那么现在的第一个孩子有可能是str子目录，也需要被删除
    {
        if(p->data==str)    //现在的第一个孩子需要被删除,删除子目录
        {
            catalogTreeNode* deleteNode=p;
            currentNode->firstChild=p->nextSibling;
            p=p->nextSibling;
            //从文件目录系统中删除子目录
            eraseSubCatalog(deleteNode);
            currentNode->childCatalogNumber--;
            //真实删除子目录
            string op="rd/s/q ";
            string path=workDirectory;
            for(auto np:currentNodePath)
                path += "\\" + np;
            path = path + "\\" + str;
            op += path;
            system(op.c_str());
            return ;
        }
    }

    //寻找有无名为str的文件
    while(p->nextSibling!=NULL && p->nextSibling->data<str && p->nextSibling->isFile)
        p=p->nextSibling;

    if(p->nextSibling!=NULL && p->nextSibling->data==str && p->nextSibling->isFile)   //存在名为str的文件
    {
        catalogTreeNode* deleteNode=p->nextSibling;
        p->nextSibling=p->nextSibling->nextSibling;
        //从文件目录系统中删除文件
        delete deleteNode;
        currentNode->childFileNumber--;
        //真实删除文件
        string op="del ";
        string path=workDirectory;
        for(auto np:currentNodePath)
            path += "\\" + np;
        path = path + "\\" + str;
        op += path;
        system(op.c_str());
    }

    //指针移动到最后一个文件的位置
    while(p->nextSibling!=NULL && p->nextSibling->isFile)
        p=p->nextSibling;

    if(p->nextSibling==NULL)         //当前目录不存在可以删除的子目录
        return ;

    //寻找有无名为str的子目录
    while(p->nextSibling!=NULL && p->nextSibling->data<str)
        p=p->nextSibling;

    if(p->nextSibling!=NULL && p->nextSibling->data==str)   //存在名为str的子目录
    {
        catalogTreeNode* deleteNode=p->nextSibling;
        p->nextSibling=p->nextSibling->nextSibling;
        //从文件目录系统中删除子目录
        eraseSubCatalog(deleteNode);
        currentNode->childCatalogNumber--;
        //真实删除子目录
        string op="rd/s/q ";
        string path=workDirectory;
        for(auto np:currentNodePath)
            path += "\\" + np;
        path = path + "\\" + str;
        op += path;
        system(op.c_str());
    }
}

void catalogTree::save(string& str)
{//将从根节点开始的目录树结构保存到文件（名为str）中
    ofstream fout;
    fout.open(str.c_str());

    //保存当前进入的目录
    fout<<currentNode->data<<"\n";

    queue<catalogTreeNode*> q;
    q.push(root);
    while(!q.empty())
    {
        catalogTreeNode* pp=q.front();
        q.pop();
        fout<<pp->data<<"\n";
        fout<<pp->childFileNumber+pp->childCatalogNumber<<"\n";
        catalogTreeNode* p=pp->firstChild;
        while(p!=NULL)
        {
            fout<<p->data<<"\n";
            fout<<p->isFile<<"\n";
            q.push(p);
            p=p->nextSibling;
        }
    }
    fout.close();
}

void catalogTree::load(string& str)
{//从文件str中读取之前保存的目录树结构，并根据其重新建立当前目录树
    clear();

    ifstream fin;
    fin.open(str.c_str());

    if(!fin.is_open())
    {
        cout<<"系统找不到指定的输入文件。"<<"\n";
        return ;
    }

    //读取当前进入的目录
    string currentNodeData;
    getline(fin,currentNodeData);

    string parentNodeData;
    int childNumber;
    string childNodeData;
    bool childNodeType;
    catalogTreeNode *lastSibling;

    //建立根节点
    root=new catalogTreeNode();
    root->data="/";
    root->isFile=false;
    root->childFileNumber=0;
    root->childCatalogNumber=0;
    root->parent=NULL;
    root->firstChild=NULL;
    root->nextSibling=NULL;

    queue<catalogTreeNode*> q;
    q.push(root);
    while(!q.empty())
    {
        getline(fin,parentNodeData);
        fin>>childNumber;
        fin.ignore();

        catalogTreeNode* pp=q.front();
        q.pop();

        for(int i=0;i<childNumber;i++)
        {
            getline(fin,childNodeData);
            fin>>childNodeType;
            fin.ignore();

            catalogTreeNode* newNode=new catalogTreeNode(childNodeData,childNodeType);
            if(newNode->data==currentNodeData && !newNode->isFile)
                currentNode=newNode;
            q.push(newNode);
            if(childNodeType)   pp->childFileNumber++;
            else    pp->childCatalogNumber++;
            newNode->parent=pp;
            if(i==0)
            {
                pp->firstChild=newNode;
                lastSibling=newNode;
            }
            else
            {
                lastSibling->nextSibling=newNode;
                lastSibling=newNode;
            }
        }
    }
    fin.close();
    setCurrentNodePath();
    rebuild(root);
    cout<<"load successfully"<<"\n";
}

void catalogTree::clear()
{//删除目录树所有内容
    eraseSubCatalog(root);
}

void catalogTree::setWorkDirectory(char *buf)
{//设置当前exe工作的绝对路径
    for(int i=0;buf[i]!='\0';i++)
        workDirectory.push_back(buf[i]);
}

void catalogTree::getCurrentDirectory()
{//每次操作结束后显示当前的绝对路径
    catalogTreeNode* p=currentNode;
    stack<catalogTreeNode*> sk;

    //节点依次入栈
    while(p!=NULL)
    {
        sk.push(p);//入栈
        p=p->parent;
    }
    cout<<workDirectory;
    if(sk.size()==1)
    {
        cout<<">";
        return ;
    }

    sk.pop();   //弹出根节点
    //出栈
    while(!sk.empty())
    {//注意输出的/的个数
        cout<<"\\"<<sk.top()->data;  //输出
        sk.pop();                   //弹栈
    }
    cout<<">";
}

void catalogTree::rebuild(catalogTreeNode* r)
{//从文件中重新加载目录后，重新建立文件
    catalogTreeNode *p=r->firstChild;
    while(p!=NULL && p->isFile)
    {
        //建立文件
        string op="cd.>";
        string path=workDirectory;
        for(auto np:pv)
            path += "\\" + np;
        path = path + "\\" + p->data;
        op += path;
        system(op.c_str());

        p=p->nextSibling;
    }
    while(p!=NULL && !p->isFile)
    {
        //递归建立子目录
        string op="mkdir ";
        string path=workDirectory;
        for(auto np:pv)
            path += "\\" + np;
        path = path + "\\" + p->data;
        op += path;
        system(op.c_str());
        pv.emplace_back(p->data);
        rebuild(p);
        pv.pop_back();
        p=p->nextSibling;
    }
}

void catalogTree::setCurrentNodePath()
{
    catalogTreeNode* p=currentNode;
    stack<catalogTreeNode*> sk;

    //节点依次入栈
    while(p!=NULL)
    {
        sk.push(p);//入栈
        p=p->parent;
    }

    if(sk.size()==1)
        return ;
        
    sk.pop();   //弹出根节点
    //出栈
    while(!sk.empty())
    {//注意输出的/的个数
        currentNodePath.emplace_back(sk.top()->data);
        sk.pop();                   //弹栈
    }
}

void catalogTree::help()
{
    printf("-----------------------------------File Directory System Help-----------------------------------\n");
    printf("dir                     -  list all subdirectories and files in the current directory\n");
    printf("cd                      -  list the absolute path of the current directory\n");
    printf("cd ..                   -  make the current directory become the parent of the current directory\n");
    printf("cd str                  -  make the current directory become the directory represented by str\n");
    printf("mkdir str               -  create a subdirectory named str\n");
    printf("mkfile str              -  create a file named str\n");
    printf("delete str              -  delete the subdirectory or file named str\n");
    printf("save str                -  save structure of the file directory named str\n");
    printf("load str                -  load structure of the file directory named str\n");
    printf("test str                -  test input file named str\n");
    printf("?                       -  display this help menu\n");
    printf("quit                    -  exit the program\n\n");
}

void catalogTree::test(string& str)
{//测试数据集
    ifstream fin(str.c_str());
    if(!fin.is_open())
    {
        cout<<"系统找不到指定的输入文件。"<<"\n";
        return ;
    }

    cout<<"请输入输出文件名: ";
    string ostr;
    getline(cin,ostr);
    Fout.open(ostr.c_str());
    
    string op;
    while(getline(fin,op))
    {
        if(op[0]=='d' && op[1]=='i')
        {
            dir_test();
        }
        else if(op[0]=='c')
        {
            if(op.size()==2)
            {
                cd_test();
            }
            else if(op.size()==5 && op[3]=='.' && op[4]=='.')
            {
                string str="..";
                cd(str);
            }
            else
            {
                string str(op,3,op.size());
                cd(str);
            }
        }
        else if(op[0]=='m' && op[1]=='k' && op[2]=='d')
        {
            string str(op,6,op.size());
            mkdir(str);
        }
        else if(op[0]=='m' && op[1]=='k' && op[2]=='f')
        {
            string str(op,7,op.size());
            mkfile(str);
        }
        else if((op[0]=='d' && op[1]=='e'))
        {
            string str(op,7,op.size());
            Delete(str);
        }
        else if(op[0]=='s')
        {
            string str(op,5,op.size());
            save(str);
        }
        else if(op[0]=='l')
        {
            string str(op,5,op.size());
            load(str);
        }
        else if(op[0]=='q')
        {
            break;
        }
        else if(op[0]=='?')
        {
            help();
        }
    }
    cout<<"测试结束，请查看输出文件。"<<"\n";
    fin.close();
    Fout.close();
}