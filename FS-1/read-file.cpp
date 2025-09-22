#include <iostream>
#include <fstream>
using namespace std;

void printFile(const char* path){
    ifstream file(path,ios::in|ios::binary);
    if(!file.is_open()){
        cout<<"Error: cannot open file"<<endl;
        return;
    }
    const int size=1024;
    char buffer[size];
    while(file.read(buffer,size)){
        cout.write(buffer,file.gcount());
    }
    cout.write(buffer,file.gcount());
    file.close();
}

int main(int argc,char* argv[]){
    if(argc<2){
        cout<<"Error: no file path"<<endl;
        return 1;
    }
    printFile(argv[1]);
}
