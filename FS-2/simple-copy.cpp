#include <iostream>
#include <fstream>
using namespace std;

void copyFile(const char* src,const char* dst){
    ifstream in(src,ios::in|ios::binary);
    if(!in.is_open()){
        cout<<"Error: cannot open source file"<<endl;
        return;
    }
    ofstream out(dst,ios::out|ios::binary|ios::trunc);
    if(!out.is_open()){
        cout<<"Error: cannot open destination file"<<endl;
        in.close();
        return;
    }
    const int size=1024;
    char buffer[size];
    while(in.read(buffer,size)){
        out.write(buffer,in.gcount());
    }
    out.write(buffer,in.gcount());
    in.close();
    out.close();
}

int main(int argc,char* argv[]){
    if(argc<3){
        cout<<"Error: not enough arguments"<<endl;
        return 1;
    }
    copyFile(argv[1],argv[2]);
}
