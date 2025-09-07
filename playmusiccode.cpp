#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

    const char* server_ip = "127.0.0.1";
    const int server_port = 6600;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "can't create socket";
        return 1;
    }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server.sin_addr);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        cerr << "Can't connect to MPD" << endl;
        return 1;
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    read(sock, buffer, sizeof(buffer) - 1);
    cout << "MPD: " << buffer << endl;

    string user_command;
    while (true) {
        cout << ">";
        getline(cin, user_command);
        if (user_command == "quit") break;

        user_command += "\n";
        send(sock, user_command.c_str(), user_command.size(), 0);

        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer) - 1);
        cout << buffer;
    }

    memset(buffer, 0, sizeof(buffer));
    read(sock, buffer, sizeof(buffer) - 1);
    cout << "MPD: " << buffer << endl;

    close(sock);

    return 0;



