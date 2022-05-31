#ifndef __header__
#define __header__

#include <stdint.h>

typedef unsigned long int guti_t;
typedef unsigned long int lgclock_t;

extern const size_t C_HEADER_SIZE;

typedef struct custom_header {
  double timestamp;
  lgclock_t logical_clock;
  size_t payload_length;
  guti_t guti;
  uint8_t cpf_id;
  uint8_t last_msg;
} custom_header;

double timestamp_micro(void);
void add_header(uint8_t * buff, guti_t guti, size_t payload_len);
void update_header(uint8_t* src, uint8_t* dst, size_t payload_len);

#endif