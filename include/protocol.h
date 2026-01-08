#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <sys/types.h>

void process_message(unsigned char *data, ssize_t size);

#endif
