#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s <size to report> <message>\n", argv[0]);
    return 1;
  }

  int sock = 0;
  struct sockaddr_in serv_addr;
  char *message = argv[2];

  size_t msg_len = atoi(argv[1]);
  size_t actual_len = strlen(argv[2]);

  unsigned char packet[actual_len + 5];
  packet[0] = 0x01; // Plaintext message type

  uint32_t net_len = htonl(msg_len);
  memcpy(packet + 1, &net_len, sizeof(uint32_t));

  const char *msg_ptr = (char *)(packet + 5);
  memcpy(packet + 5, message, actual_len);
  printf("Measured length: %lu", strlen(msg_ptr));

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

  send(sock, packet, 5 + actual_len, 0);
  printf("Message sent (Type=0x01, Len=%zu, Actual Len=%zu)\n", msg_len,
         actual_len);

  close(sock);
  return 0;
}
