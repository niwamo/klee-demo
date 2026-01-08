#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void process_packet(unsigned char *data, ssize_t size) {
  if (size < 5)
    return;

  unsigned char type = data[0];

  uint32_t msg_len;
  memcpy(&msg_len, data + 1, sizeof(uint32_t));
  msg_len = ntohl(msg_len);
  char msg[msg_len + 1];

  printf("Processing Message Type %02x of len %d\n", type, msg_len);

  memcpy(msg, data + 5, msg_len);

  msg[msg_len] = '\0';
  printf("Message: %s\n", msg);
}

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  unsigned char buffer[1024] = {0};

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(8080);

  bind(server_fd, (struct sockaddr *)&address, sizeof(address));
  listen(server_fd, 3);

  printf("Server listening on port 8080...\n");
  while ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                              (socklen_t *)&addrlen))) {
    ssize_t valread = read(new_socket, buffer, 1024);
    process_packet(buffer, valread);
    close(new_socket);
  }
  return 0;
}
