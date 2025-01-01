#include <iostream>
#include <fstream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <unistd.h>

void sendFile(const std::string& filename, const std::string& serverIp, int serverPort);

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <filename> <server-ip> <server-port>\n";
    return 1;
  }

  WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

  sendFile(argv[1], argv[2], std::stoi(argv[3]));

  WSACleanup();
  return 0;
}

void sendFile(const std::string& filename, const std::string& serverIp, int serverPort) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    std::cerr << "Socket creation failed\n";
    return;
  }

  sockaddr_in serverAddr{};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(serverPort);
  inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);

  if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
    std::cerr << "Connection to server failed\n";
    closesocket(sock);
    return;
  }

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file\n";
    closesocket(sock);
    return;
  }

  std::string line, jsonContent;
  while (std::getline(file, line)) {
    jsonContent += line + "\n";
  }

  send(sock, jsonContent.c_str(), jsonContent.size(), 0);
  std::cout << "File sent successfully\n";

  closesocket(sock);
}