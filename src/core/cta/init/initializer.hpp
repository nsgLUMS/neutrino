#include "../Common/globals.h"

extern struct rte_eth_dev_tx_buffer *tx_buffer[RTE_MAX_ETHPORTS];
extern struct rte_mempool *rx_tx_mempool;

void setup_cpf(int, char **);
void check_ports();
void exit_cpf();

void update_stats(void);
void update_rx(unsigned, uint16_t);
void update_tx(unsigned, uint16_t);
