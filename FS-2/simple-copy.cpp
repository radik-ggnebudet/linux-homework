#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
using namespace std;

void copyFile(const char* src,const char* dst){
    int in=open(src,O_RDONLY);
    if(in==-1){
        cout<<"Error: cannot open source file"<<endl;
        return;
    }
    int out=open(dst,O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(out==-1){
        cout<<"Error: cannot open destination file"<<endl;
        close(in);
        return;
    }
    const int size=1024;
    char buffer[size];
    ssize_t bytes;
    while((bytes=read(in,buffer,size))>0){
        write(out,buffer,bytes);
    }
    close(in);
    close(out);
}

int main(int argc,char* argv[]){
    if(argc<3){
        cout<<"Error: not enough arguments"<<endl;
        return 1;
    }
    copyFile(argv[1],argv[2]);
}
