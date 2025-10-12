#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filepath>" << endl;
        return 1;
    }

    const char* filepath = argv[1];

    int fd1 = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd1 == -1) {
        cerr << "Error: Cannot open file" << endl;
        return 1;
    }

    int fd2 = dup(fd1);
    if (fd2 == -1) {
        cerr << "Error: Cannot duplicate descriptor" << endl;
        close(fd1);
        return 1;
    }

    const char* line1 = "first line\n";
    const char* line2 = "second line\n";

    ssize_t written = write(fd1, line1, strlen(line1));
    if (written == -1) {
        cerr << "Error: Write failed" << endl;
        close(fd1);
        close(fd2);
        return 1;
    }

    written = write(fd2, line2, strlen(line2));
    if (written == -1) {
        cerr << "Error: Write failed" << endl;
        close(fd1);
        close(fd2);
        return 1;
    }

    close(fd1);
    close(fd2);

    return 0;
}

