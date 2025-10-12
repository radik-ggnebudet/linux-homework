#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file-to-erase>" << endl;
        return 1;
    }

    const char* filepath = argv[1];

    struct stat file_stat;
    if (stat(filepath, &file_stat) == -1) {
        cerr << "Error: Cannot access file" << endl;
        return 1;
    }

    if (!S_ISREG(file_stat.st_mode)) {
        cerr << "Error: Not a regular file" << endl;
        return 1;
    }

    off_t file_size = file_stat.st_size;

    int fd = open(filepath, O_WRONLY);
    if (fd == -1) {
        cerr << "Error: Cannot open file" << endl;
        return 1;
    }

    lseek(fd, 0, SEEK_SET);

    const size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', BUFFER_SIZE);

    off_t bytes_written = 0;
    while (bytes_written < file_size) {
        size_t to_write = BUFFER_SIZE;
        if (bytes_written + BUFFER_SIZE > file_size) {
            to_write = file_size - bytes_written;
        }

        ssize_t result = write(fd, buffer, to_write);
        if (result == -1) {
            cerr << "Error: Write failed" << endl;
            close(fd);
            return 1;
        }

        bytes_written += result;
    }

    close(fd);

    if (unlink(filepath) == -1) {
        cerr << "Error: Cannot delete file" << endl;
        return 1;
    }

    cout << "File deleted successfully" << endl;
    return 0;
}
