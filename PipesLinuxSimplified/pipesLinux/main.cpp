#include <iostream>
#include<unistd.h>
using namespace std;
// The order of execution is the following: M,A,P,S.
const int END=-1;
const int LEN=4;
int pipes[LEN - 1];
string processes[]{ "../../procM/cmake-build-debug/procM",
                   "../../procA/cmake-build-debug/procA",
                   "../../procP/cmake-build-debug/procP",
                   "../../procS/cmake-build-debug/procS" };
void run(int i) {
    if (pipe(pipes) < 0)
        exit(1);
    if (!fork()) {
        if (dup2(pipes[1], 1) < 0) exit(1);
        system(processes[i].c_str());
        exit(0);
    }
    if (dup2(pipes[0], 0) < 0) exit(1);
    close(pipes[1]);
}

int main() {
    std::cout << "Alright, throw me some numbers:\n";
    for (int i = 0; i < LEN - 1; i++) {
        run(i);
    }
    system(processes[3].c_str());
    return 0;
}