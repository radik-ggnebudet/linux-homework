#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <source-file> <destination-file>" << endl;
        return 1;
    }

    const char* source_path = argv[1];
    const char* dest_path = argv[2];

    int src_fd = open(source_path, O_RDONLY);
    if (src_fd == -1) {
        cerr << "Error: Cannot open source file" << endl;
        return 1;
    }

    struct stat src_stat;
    if (fstat(src_fd, &src_stat) == -1) {
        cerr << "Error: Cannot stat source file" << endl;
        close(src_fd);
        return 1;
    }

    int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, src_stat.st_mode);
    if (dest_fd == -1) {
        cerr << "Error: Cannot open destination file" << endl;
        close(src_fd);
        return 1;
    }

    const size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    off_t total_bytes = 0;
    off_t data_bytes = 0;
    off_t hole_bytes = 0;

    off_t pos = 0;
    off_t file_size = src_stat.st_size;

    while (pos < file_size) {
        off_t data_start = lseek(src_fd, pos, SEEK_DATA);

        if (data_start == -1) {
            if (errno == ENXIO) {
                hole_bytes += file_size - pos;
                if (lseek(dest_fd, file_size - 1, SEEK_SET) != -1) {
                    write(dest_fd, "", 1);
                }
                break;
            } else {
                cerr << "Error: lseek SEEK_DATA failed" << endl;
                close(src_fd);
                close(dest_fd);
                return 1;
            }
        }

        if (data_start > pos) {
            hole_bytes += data_start - pos;
            if (lseek(dest_fd, data_start, SEEK_SET) == -1) {
                cerr << "Error: Cannot seek in destination" << endl;
                close(src_fd);
                close(dest_fd);
                return 1;
            }
        }

        off_t hole_start = lseek(src_fd, data_start, SEEK_HOLE);
        if (hole_start == -1) {
            cerr << "Error: lseek SEEK_HOLE failed" << endl;
            close(src_fd);
            close(dest_fd);
            return 1;
        }

        off_t data_len = hole_start - data_start;

        if (lseek(src_fd, data_start, SEEK_SET) == -1) {
            cerr << "Error: Cannot seek to data start" << endl;
            close(src_fd);
            close(dest_fd);
            return 1;
        }

        off_t bytes_to_copy = data_len;
        while (bytes_to_copy > 0) {
            ssize_t to_read = (bytes_to_copy > BUFFER_SIZE) ? BUFFER_SIZE : bytes_to_copy;
            ssize_t bytes_read = read(src_fd, buffer, to_read);

            if (bytes_read == -1) {
                cerr << "Error: Read failed" << endl;
                close(src_fd);
                close(dest_fd);
                return 1;
            }

            if (bytes_read == 0) {
                break;
            }

            ssize_t bytes_written = write(dest_fd, buffer, bytes_read);
            if (bytes_written == -1) {
                cerr << "Error: Write failed" << endl;
                close(src_fd);
                close(dest_fd);
                return 1;
            }

            data_bytes += bytes_written;
            bytes_to_copy -= bytes_read;
        }

        pos = hole_start;
    }

    total_bytes = data_bytes + hole_bytes;

    close(src_fd);
    close(dest_fd);

    cout << "Successfully copied " << total_bytes << " bytes (data: "
         << data_bytes << ", hole: " << hole_bytes << ")." << endl;

    return 0;
}

