#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>

using namespace std;

void help() {
    printf("----------------------------------------Help----------------------------------------\n");
    printf("test file                    -  test input file\n");
    printf("test file -i                 -  test input file and generate image\n");
    printf("test folder                  -  test input files in the folder\n");
    printf("test folder -i               -  test input files in the folder and generate images\n");
    printf("?                            -  display this help menu\n");
    printf("quit                         -  exit the program\n\n");
}

int main() {
    help();
    cout<<"> ";
    string op;
    while(getline(cin,op)) {
        if(op=="test file") {
            string inputFileName,outputFileName;
            printf("input file name: ");
            getline(cin,inputFileName);
            ifstream fin;
            fin.open(inputFileName.c_str());
            if(!fin.is_open()) {
                cout<<"找不到指定的输入文件"<<"\n\n";
                cout<<"> ";
                continue ;
            }
            fin.close();
            printf("output file name: ");
            getline(cin,outputFileName);

            string todo;
            clock_t start1,stop1,start2,stop2;

            todo="solution1 0 < " + inputFileName + " > " + outputFileName;
            system("g++ solution1.cpp -o solution1 -g");
            start1=clock();//起始时刻
            system(todo.c_str());
            stop1=clock();//结束时间
            double duration1=(double(stop1-start1)/CLK_TCK);

            todo="solution2 0 < " + inputFileName + " > " + outputFileName;
            system("g++ solution2.cpp -o solution2 -g");
            start2=clock();//起始时刻
            system(todo.c_str());
            stop2=clock();//结束时间
            double duration2=(double(stop2-start2)/CLK_TCK);
            cout<<"------------------\n";
            cout<<"| Time1: "<<duration1<<" |\n";
            cout<<"| Time2: "<<duration2<<" |\n";
            cout<<"------------------\n\n";
        }
        else if(op=="test file -i") {
            string inputFileName,outputFileName;
            printf("input file name: ");
            getline(cin,inputFileName);
            ifstream fin;
            fin.open(inputFileName.c_str());
            if(!fin.is_open()) {
                cout<<"找不到指定的输入文件"<<"\n\n";
                cout<<"> ";
                continue ;
            }
            fin.close();
            printf("output file name: ");
            getline(cin,outputFileName);

            string todo;
            clock_t start1,stop1,start2,stop2;

            todo="solution1 1 < " + inputFileName + " > " + outputFileName;
            system("g++ solution1.cpp -o solution1 -g");
            start1=clock();//起始时刻
            system(todo.c_str());
            stop1=clock();//结束时间
            double duration1=(double(stop1-start1)/CLK_TCK);

            todo="solution2 1 < " + inputFileName + " > " + outputFileName;
            system("g++ solution2.cpp -o solution2 -g");
            start2=clock();//起始时刻
            system(todo.c_str());
            stop2=clock();//结束时间
            double duration2=(double(stop2-start2)/CLK_TCK);
            cout<<"------------------\n";
            cout<<"| Time1: "<<duration1<<" |\n";
            cout<<"| Time2: "<<duration2<<" |\n";
            cout<<"------------------\n\n";
        }
        else if(op=="test folder") {
            clock_t start1,stop1,start2,stop2;
            double time1=0,time2=0;

            system("g++ solution1.cpp -o solution1 -g");

            for (int i = 1; i <= 100; i++) {
                std::cout << "Test case " << i <<" : "<< std::endl;
                std::string name = std::to_string(i);
                std::string todo;
                todo = "solution1 < input\\input" + name + ".in > output1\\" + name + ".out";

                start1=clock();//起始时刻
                system(todo.c_str());
                stop1=clock();//结束时间
                time1+=(double(stop1-start1)/CLK_TCK);

                todo = "fc output1\\" + name + ".out outputSTD\\" + name + ".out";
                auto ret = system(todo.c_str());
                if (ret) {
                    cout<<"Case " << i << " Error\n\n";
                    //return 0;
                } else {
                    cout<<"Case " << i << " Accept\n\n";
                }
            }
            puts("Solution1 All Accept\n");

            system("g++ solution2.cpp -o solution2 -g");
            for (int i = 1; i <= 100; i++) {
                std::cout << "Test " << i <<" : "<< std::endl;
                std::string name = std::to_string(i);
                std::string todo;
                todo = "solution2 < input\\input" + name + ".in > output2\\" + name + ".out";

                start2=clock();//起始时刻
                system(todo.c_str());
                stop2=clock();//结束时间
                time2+=(double(stop2-start2)/CLK_TCK);

                todo = "fc output2\\" + name + ".out outputSTD\\" + name + ".out";
                auto ret = system(todo.c_str());
                if (ret) {
                    cout<<"Case " << i << " Error\n\n";
                    return 0;
                } else {
                    cout<<"Case " << i << " Accept\n\n";
                }
            }
            puts("Solution2 All Accept\n");
            cout<<"------------------\n";
            cout<<"| Time1: "<<time1<<" |\n";
            cout<<"| Time2: "<<time2<<" |\n";
            cout<<"------------------\n\n";
        }
        else if(op=="test folder -i") {
            clock_t start1,stop1,start2,stop2;
            double time1=0,time2=0;

            system("g++ solution1.cpp -o solution1 -g");

            for (int i = 1; i <= 100; i++) {
                std::cout << "Test case " << i <<" : "<< std::endl;
                std::string name = std::to_string(i);
                std::string todo;
                todo = "solution1 "+ name + ".dot solve" + name + ".jpg < input\\input" + name + ".in > output1\\" + name + ".out";

                start1=clock();//起始时刻
                system(todo.c_str());
                stop1=clock();//结束时间
                time1+=(double(stop1-start1)/CLK_TCK);

                todo = "fc output1\\" + name + ".out outputSTD\\" + name + ".out";
                auto ret = system(todo.c_str());
                if (ret) {
                    cout<<"Case " << i << " Error\n\n";
                    return 0;
                } else {
                    cout<<"Case " << i << " Accept\n\n";
                }
            }
            puts("Solution1 All Accept\n");


            system("g++ solution2.cpp -o solution2 -g");

            for (int i = 1; i <= 100; i++) {
                std::cout << "Test " << i <<" : "<< std::endl;
                std::string name = std::to_string(i);
                std::string todo;
                todo = "solution2 "+ name + ".dot solve" + name + ".jpg < input\\input" + name + ".in > output2\\" + name + ".out";

                start2=clock();//起始时刻
                system(todo.c_str());
                stop2=clock();//结束时间
                time2+=(double(stop2-start2)/CLK_TCK);

                todo = "fc output2\\" + name + ".out outputSTD\\" + name + ".out";
                auto ret = system(todo.c_str());
                if (ret) {
                    cout<<"Case " << i << " Error\n\n";
                    return 0;
                } else {
                    cout<<"Case " << i << " Accept\n\n";
                }
            }
            puts("Solution2 All Accept\n");
            cout<<"------------------\n";
            cout<<"| Time1: "<<time1<<" |\n";
            cout<<"| Time2: "<<time2<<" |\n";
            cout<<"------------------\n\n";
        }
        else if(op=="?") {
            help();
        }
        else if(op=="quit") {
            break;
        }
        else {
            cout<<"无效的命令\n\n";
        }
        cout<<"> ";
    }
    return 0;
}