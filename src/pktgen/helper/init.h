#ifndef INIT_H
#define INIT_H

#include "globals.h"

int parse_args(int argc, char **argv, struct master_conf *conf);
struct rte_mempool* setup_rx_port(void);
void setup_tx_port(unsigned int nb_workers);
void check_port_status(uint8_t portid);

#endif