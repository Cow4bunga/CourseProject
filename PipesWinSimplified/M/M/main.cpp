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
        output.append(to_string(num + 2) + " ");
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

    HANDLE pipe3;
    while ((pipe3 = CreateFile("\\\\.\\pipe\\$Pipe3$", PIPE_ACCESS_INBOUND,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr))
        == INVALID_HANDLE_VALUE) {
        // do nothing//
    }

    if (pipe3 == INVALID_HANDLE_VALUE) {
        wcout << "M: Failed to connect to pipe." << endl;
        system("pause");
        return 1;
    }

    char buffer[Size];
    if (!ReadFile(pipe3, buffer, Size,
        nullptr, nullptr)) {
        cout << "M: reading failed!\n";
    }

    string str(buffer);
    string str1 = parseN(str);
    cout << "M got: " << str1 << "\n";
    string input = parse(str);
    cout << "M sent: " << input << "\n";

    HANDLE pipe4 = CreateNamedPipe(
        "\\\\.\\pipe\\$Pipe4$", PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE,
        1, Size, Size, 0, nullptr);

    if (pipe4 == nullptr || pipe4 == INVALID_HANDLE_VALUE) {
        wcout << "M: Failed to create outbound pipe instance.\n";
        system("pause");
        return 1;
    }

    ConnectNamedPipe(pipe4, nullptr);

    if (!WriteFile(pipe4, input.c_str(), strlen(buffer),
        nullptr, nullptr)) {
        cout << "M: writing failed!\n";
    }

    CloseHandle(pipe3);
    CloseHandle(pipe4);

    system("pause");
    return 0;
}