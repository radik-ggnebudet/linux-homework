#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>

using namespace std;

vector<string> parse_command(const string& input) {
    vector<string> tokens;
    istringstream iss(input);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void execute_command(vector<string>& tokens, bool silent) {
    if (tokens.empty()) {
        return;
    }

    char** argv = new char*[tokens.size() + 1];
    for (size_t i = 0; i < tokens.size(); i++) {
        argv[i] = new char[tokens[i].length() + 1];
        strcpy(argv[i], tokens[i].c_str());
    }
    argv[tokens.size()] = NULL;

    pid_t pid = fork();

    if (pid == -1) {
        cerr << "Error: Fork failed" << endl;
        for (size_t i = 0; i < tokens.size(); i++) {
            delete[] argv[i];
        }
        delete[] argv;
        return;
    }

    if (pid == 0) {
        if (silent) {
            string logfile = to_string(getpid()) + ".log";
            int fd = open(logfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd != -1) {
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                close(fd);
            }
        }

        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            const char* old_path = getenv("PATH");
            string new_path = string(cwd);
            if (old_path) {
                new_path += ":" + string(old_path);
            }
            setenv("PATH", new_path.c_str(), 1);
        }

        execvp(argv[0], argv);
        cerr << "Error: Command execution failed" << endl;
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }

    for (size_t i = 0; i < tokens.size(); i++) {
        delete[] argv[i];
    }
    delete[] argv;
}

int main() {
    string input;

    while (true) {
        cout << "$ ";
        if (!getline(cin, input)) {
            break;
        }

        if (input.empty()) {
            continue;
        }

        vector<string> tokens = parse_command(input);
        if (tokens.empty()) {
            continue;
        }

        if (tokens[0] == "exit") {
            break;
        }

        bool silent = false;
        if (tokens[0] == "silent") {
            silent = true;
            tokens.erase(tokens.begin());
        }

        execute_command(tokens, silent);
    }

    return 0;
}

