#include "windows.h"
#include <iostream>

using namespace std;

LPCSTR A_path = "C:\\OS\\A\\x64\\Debug\\A.exe";
LPCSTR M_path = "C:\\OS\\M\\x64\\Debug\\M.exe";
LPCSTR S_path = "C:\\OS\\S\\x64\\Debug\\S.exe";
LPCSTR P_path = "C:\\OS\\P\\x64\\Debug\\P.exe";

const unsigned int Size = 50;


int main() {
    PROCESS_INFORMATION pi1 = { 0 }, pi2 = { 0 }, pi3 = { 0 }, pi4 = { 0 };
    STARTUPINFO sa1 = { 0 }, sa2 = { 0 }, sa3 = { 0 }, sa4 = { 0 };

    HANDLE pipe1 = CreateNamedPipe(
        "\\\\.\\pipe\\$Pipe1$", // name of the pipe
        PIPE_ACCESS_OUTBOUND,
        PIPE_TYPE_BYTE, // send data as a byte stream
        1, // only allow 1 instance of this pipe
        Size, //  outbound buffer
        Size, //  inbound buffer
        0, // use default wait time
        nullptr // use default security attributes
    );

    if (pipe1 == nullptr || pipe1 == INVALID_HANDLE_VALUE) {
        wcout << "Main: Failed to create outbound pipe instance.";
        system("pause");
        return 1;
    }

    if (!CreateProcess(P_path, nullptr,
        nullptr, nullptr, FALSE,
        0, nullptr, nullptr,
        &sa1, &pi1)) {
        cout << "Main: failed to create P-process!\n";
    }
    ConnectNamedPipe(pipe1, nullptr);

    const char* data = "1 1 1 1 1 1 7 ";
    string str1 = data;
    cout << "Main sent: " << str1 << endl;
    DWORD numBytesWritten = 0;
    if (!WriteFile(pipe1, data, strlen(data),
        &numBytesWritten, nullptr)) {
        cout << "Main: writing failed!\n";
    }

    CloseHandle(pipe1);

    if (!CreateProcess(A_path, nullptr,
        nullptr, nullptr, FALSE,
        0, nullptr, nullptr,
        &sa2, &pi2)) {
        cout << "Main: failed to create A-process!\n";
    }

    if (!CreateProcess(M_path, nullptr,
        nullptr, nullptr, FALSE,
        0, nullptr, nullptr,
        &sa3, &pi3)) {
        cout << "Main: failed to create M-process!\n";
    }

    if (!CreateProcess(S_path, nullptr,
        nullptr, nullptr, FALSE,
        0, nullptr, nullptr,
        &sa4, &pi4)) {
        cout << "Main: failed to create S-process!\n";
    }

    HANDLE pipe5;
    while ((pipe5 = CreateFile(
        "\\\\.\\pipe\\$Pipe5$",
        PIPE_ACCESS_INBOUND,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    )) == INVALID_HANDLE_VALUE) {
        // do nothing//
    }

    if (pipe5 == INVALID_HANDLE_VALUE) {
        wcout << "Main: Failed to connect to pipe." << endl;
        system("pause");
        return 1;
    }

    char buffer[Size];
    DWORD numBytesRead = 0;
    if (!ReadFile(pipe5, buffer, Size,
        &numBytesRead, nullptr)) {
        cout << "Main: reading failed!\n";
    }

    string res(buffer);

    cout << "\nMain got: " << res << "\n";

    CloseHandle(pipe5);
    //system("pause");
    return 0;
}