#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  while(1) {
      printf("Enter input: ");
      char line[1000];
      fgets(line, sizeof(line), stdin);
      write(to_server, line, strlen(line));
      printf("Response: ");
      char line2[1000];
      read(from_server, line2, sizeof(line2));
      printf("%s\n", line2);
    }
}
