
#ifndef _LIB_WATCHDOG_H_
#define _LIB_WATCHDOG_H_

int lec_watchdog_open(void);
int lec_watchdog_close(void);
int lec_watchdog_set_outtime(int OutTime);
int lec_watchdog_keep_alive(void);

#endif

