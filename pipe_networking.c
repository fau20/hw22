#include "pipe_networking.h"

/*=========================
  server_setup
  args:
  creates the WKP (upstream) and opens it, waiting for a
  connection.
  removes the WKP once a connection has been made
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  mkfifo(WKP, 0777);
  int from_client = open(WKP, O_RDONLY, 0);
  remove(WKP);

  return from_client;
}

/*=========================
  server_connect
  args: int from_client
  handles the subserver portion of the 3 way handshake
  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  char buffer[HANDSHAKE_BUFFER_SIZE];
  int to_client = 0;
  
  read(from_client, buffer, sizeof(buffer));
  printf("received: -%s-\n", buffer);

  *to_client = open(buffer, O_WRONLY, 0);

  write(*to_client, ACK, HANDSHAKE_BUFFER_SIZE);
  read(from_client, buffer, sizeof(buffer));

  if(strcmp(buffer, ACK) != 0) {
    printf("error: expecting %s/t received: %s\n", ACK, buffer);
    return 0;
  }
  else {
    printf("successful handshake\n");
  }

  return to_client;
}


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  char buffer[HANDSHAKE_BUFFER_SIZE];
  mkfifo(WKP, 0777);

  int from_client = open(WKP, O_RDONLY, 0);
  remove(WKP);

  read(from_client, buffer, sizeof(buffer));
  printf("received: -%s-\n", buffer);

  *to_client = open(buffer, O_WRONLY, 0);

  write(*to_client, ACK, HANDSHAKE_BUFFER_SIZE);
  read(from_client, buffer, sizeof(buffer));

  if(strcmp(buffer, ACK) != 0) {
    printf("error: expecting %s/t received: %s\n", ACK, buffer);
    return 0;
  }
  else {
    printf("successful handshake\n");
  }

  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int from_server = 0;
  char buffer[HANDSHAKE_BUFFER_SIZE];
  char priv[HANDSHAKE_BUFFER_SIZE];

  sprintf(priv, "%d", getpid());
  mkfifo(priv, 0777);
  mkfifo(WKP, 0777);
  *to_server = open(WKP, O_WRONLY, 0);
  write(*to_server, priv, sizeof(priv));
  from_server = open(priv, O_RDONLY, 0);
  remove(priv);

  read(from_server, buffer, sizeof(buffer));
  if(strcmp(buffer, ACK) != 0) {
    printf("error: expecting %s/t received: %s\n", ACK, buffer);
    return 0;
  }
  write(*to_server, ACK, HANDSHAKE_BUFFER_SIZE);
  printf("successful handshake\n");
  return from_server;
}
