#include "windows.h"
#include <iostream>
#include "string"
using namespace std;

const unsigned int Size = 50;

string parse(string input) {
    string output;
    while (!input.empty()) {
        auto end = find(input.begin(), input.end(), ' ');
        string tmp = input.substr(0, end - input.begin());
        if (end == input.end()) break;
        end++;
        input.erase(input.begin(), end);

        int num = stoi(tmp, nullptr, 10);
        output.append(to_string(num +3) + " ");
    }
    return output;
}

string parseN(string input) {
    string output;
    while (!input.empty()) {
        auto end = find(input.begin(), input.end(), ' ');
        string tmp = input.substr(0, end - input.begin());
        if (end == input.end()) break;
        end++;
        input.erase(input.begin(), end);

        int num = stoi(tmp, nullptr, 10);
        output.append(to_string(num) + " ");
    }
    return output;
}

int main() {
    HANDLE pipe1 = CreateFile(
        "\\\\.\\pipe\\$Pipe1$",
        PIPE_ACCESS_INBOUND,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (pipe1 == INVALID_HANDLE_VALUE) {
        wcout << "P: Failed to connect to pipe." << endl;
        system("pause");
        return 1;
    }

    char buffer[Size];
    if (!ReadFile(pipe1, buffer, Size,
        nullptr, nullptr)) {
        cout << "P: reading failed!\n";
    }

    string str(buffer);
    string str1 = parseN(str);
    cout << "P got: " << str1 << "\n";
    string input = parse(str);
    cout << "P sent: " << input << "\n";
    cout << str.length() << endl;
    cout << input.length() << endl;

    HANDLE pipe2 = CreateNamedPipe(
        "\\\\.\\pipe\\$Pipe2$", PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE,
        1, Size, Size, 0, nullptr);

    if (pipe2 == nullptr || pipe2 == INVALID_HANDLE_VALUE) {
        wcout << "P: Failed to create outbound pipe instance.\n";
        system("pause");
        return 1;
    }

    ConnectNamedPipe(pipe2, nullptr);

    if (!WriteFile(pipe2, input.c_str(), strlen(buffer),
        nullptr, nullptr)) {
        cout << "P: writing failed!\n";
    }

    CloseHandle(pipe1);
    CloseHandle(pipe2);

    //system("pause");
    return 0;
}