#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#define PORT 8080
  
class GameData {
public:
  bool done;
  char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
  char in[1];
  char turn;
};

class ClientData {
public:
  int sock = 0, valread, client_fd;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0}, message[1024] = {0}, type[1024] = {0}, c_msg[1024] = {0};
  std::string msg, strType;
};

GameData gd;
ClientData cd;

int printBoard() {
  for (int i = 0; i < 9; i++) {
    std::cout << cd.c_msg[i];
    if (i == 2 || i == 5 || i == 8) {
      std::cout << "\n";
    }
  }
  return 0;
}

int client() {
  if ((cd.sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  cd.serv_addr.sin_family = AF_INET;
  cd.serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary
  // form
  if (inet_pton(AF_INET, "127.0.0.1", &cd.serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if ((cd.client_fd = connect(cd.sock, (struct sockaddr *)&cd.serv_addr,
                           sizeof(cd.serv_addr))) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }

  return 0;
}

int write(const char* message){
  send(cd.sock, message, sizeof(message), 0);
  printf("Hello message sent\n");
}


int read(){
  cd.valread = read(cd.sock, cd.buffer, 1024);
  bool done = false;
  int i = 0;
  while(!done){
    if(cd.buffer[i] == ' '){

      for(int j = i; j < strlen(cd.buffer); j++){
	cd.c_msg[j-i] = cd.buffer[j+1];
	cd.msg = cd.buffer[j+1];
      }
      break;
    }
    cd.type[i] = cd.buffer[i];
    cd.strType += cd.buffer[i];
    i++;
  }
}

int makeMove(){
  std::string m = std::string("[MOVE] ") + std::string(gd.in);
  write(m.c_str());
  read();
  return 0;
}

int move() {
  bool invalidMove = true;
  std::cout << "move it";
  while (invalidMove) {
    std::cout << "It's your turn: ";
    std::cin >> gd.in[0];
    makeMove();
    if(cd.strType == "[!MOVE]"){
      int err = std::stoi(cd.msg);
      std::cout << "Error while making move: " << err << "\n";
      switch (err) {
      case 1:
        std::cout << "You made an invalid move... Try again.\n";
      }
    } else if(cd.strType == "[MOVE]" && cd.msg == "done") {
      invalidMove = false;
    }
  }
  return 0;
}
int main(int argc, char *argv[]) {

  // client
  client();

  gd.done = false;
  while (!gd.done) {
    read();
    std::cout << cd.strType;
    std::cout << cd.msg;
    if(cd.strType == "[BOARD]"){
      std::cout << "Board";
      printBoard();
    } else if(cd.strType == "[DOMOVE]"){
      move();
    }
  }
  // closing the connected socket
  close(cd.client_fd);
  return 0;
}
