#include "pipe_networking.h"

int main() {

  int to_client;
  int from_client;

  while(1){
    from_client = server_setup();
    char line[1000];
    int f = fork();
    if(f) {
    }
    else {
      to_client = server_connect(from_client);
      while(read(from_client, line, sizeof(line)) != 0){
        int i = 0;
        while(line[i]) {
          if(line[i] >= 'a' && line[i] <= 'z') {
            line[i] = line[i] - 'a' + 'A';
          }
          i++;
        }
        write(to_client, line, sizeof(line));
      }
    }
  }
}
