#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int main() {
    // Creating a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        cerr << "Can't create socket!";
        return -1;
    }

    // Binding socket to IP
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr *) &hint, sizeof(hint)) == -1) {
        cerr << "Can't bind to IP!";
        return -2;
    }

    if (listen(listening, SOMAXCONN) == -1) {
        cerr << "Can't listen!";
        return -3;
    }

    sockaddr_in client;
    socklen_t clientSize;
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening, (sockaddr *) &client, &clientSize);

    if (clientSocket == -1) {
        cerr << "Problem with client!";
        return -4;
    }

    close(listening);
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr *) &client, sizeof(client),
                             host, NI_MAXHOST,
                             svc, NI_MAXSERV,
                             0);

    if (result) {
        cout << host << " connected on " << svc << endl;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on " << htons(client.sin_port) << endl;
    }

    char buf[4096];
    while (true) {
        memset(buf, 0, 4096);

        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1) {
            cerr << "Connection issue!" << endl;
            break;
        }

        if (bytesRecv == 0) {
            cout << "Client disconnected!" << endl;
            break;
        }

        cout << "Received: " << string(buf, 0, bytesRecv) << endl;

        send(clientSocket, buf, bytesRecv + 1, 0);
    }

    close(clientSocket);

    return 0;
}