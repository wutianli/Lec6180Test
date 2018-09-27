
#ifndef _LIB_AT24_H_
#define _LIB_AT24_H_

int lec_eep_init(void);
int lec_eep_read(int offset, unsigned char* buf, int size);
int lec_eep_write(int offset, unsigned char* buf, int size);
int lec_eep_exit(void);
#endif

