#ifndef __PERIPHERAL_H__
#define __PERIPHERAL_H__

// lora sx1262
#define LORA_FREQ      850.0
#define LORA_MODE_SEND 0
#define LORA_MODE_RECV 1

bool lora_sx1262_init(void);
void lora_set_mode(int mode);
int lora_get_mode(void);
void lora_receive_loop(void);
void lora_transmit(const char *str);
bool lora_get_recv(const char **str, int *rssi);
void lora_set_recv_flag(void);
void lora_sleep(void);
#endif