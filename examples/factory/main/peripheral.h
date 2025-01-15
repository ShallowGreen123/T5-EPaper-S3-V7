#ifndef __PERIPHERAL_H__
#define __PERIPHERAL_H__

#define NFC_PRIORITY     (configMAX_PRIORITIES - 1)
#define LORA_PRIORITY    (configMAX_PRIORITIES - 2)
#define WS2812_PRIORITY  (configMAX_PRIORITIES - 3)
#define BATTERY_PRIORITY (configMAX_PRIORITIES - 4)
#define INFARED_PRIORITY (configMAX_PRIORITIES - 5)

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
void lora_recv_suspend(void);
void lora_recv_resume(void);
#endif