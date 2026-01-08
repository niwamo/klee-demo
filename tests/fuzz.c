#include "../include/protocol.h"
#include <stddef.h>
#include <stdint.h>

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size > 0) {
    process_message((unsigned char *)Data, Size);
  }
  return 0;
}
