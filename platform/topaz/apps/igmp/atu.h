#ifndef _ATU_H_
#define _ATU_H_

void atuTimer();
void atuDump();
void atuIgmpWrite();
void atuIgmpAdd(int s);
void atuIgmpDel(int s);
void atuIgmpChange(int s);

#define ATUPERIOD 10

#endif
