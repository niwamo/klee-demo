#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <message>\n", argv[0]);
    return 1;
  }

  int sock = 0;
  struct sockaddr_in serv_addr;
  char *message = argv[1];
  size_t msg_len = strlen(message);

  unsigned char packet[msg_len];
  packet[0] = 0x01;

  uint32_t net_len = htonl(msg_len);
  memcpy(packet + 1, &net_len, sizeof(uint32_t));
  memcpy(packet + 5, message, msg_len);

  // Standard Socket Setup
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed. Is the server running?\n");
    return -1;
  }

  send(sock, packet, 5 + msg_len, 0);
  printf("Message sent (Type=0x01, Len=%zu)\n", msg_len);

  close(sock);
  return 0;
}
