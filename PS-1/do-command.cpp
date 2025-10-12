#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <cstring>

using namespace std;

void do_command(char** argv) {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    pid_t pid = fork();

    if (pid == -1) {
        cerr << "Error: Fork failed" << endl;
        return;
    }

    if (pid == 0) {
        execvp(argv[0], argv);
        cerr << "Error: Command execution failed" << endl;
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);

        gettimeofday(&end, NULL);

        double elapsed = (end.tv_sec - start.tv_sec) +
                        (end.tv_usec - start.tv_usec) / 1000000.0;

        int exit_code = 0;
        if (WIFEXITED(status)) {
            exit_code = WEXITSTATUS(status);
        }

        cout << "\nCommand completed with " << exit_code
             << " exit code and took " << elapsed << " seconds." << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <command> [arguments...]" << endl;
        return 1;
    }

    char** command_args = new char*[argc];
    for (int i = 1; i < argc; i++) {
        command_args[i - 1] = argv[i];
    }
    command_args[argc - 1] = NULL;

    do_command(command_args);

    delete[] command_args;

    return 0;
}

