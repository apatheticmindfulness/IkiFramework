#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void	IkiTimeInit(void);
void	IkiTimeCount(void);

int64_t IkiGetPerformanceFrequency(void);
int64_t IkiGetPerformanceCounter(void);

float	IkiGetTimeInMicroseconds(void);
float	IkiGetTimeInMiliseconds(void);
float	IkiGetTimeInSeconds(void);

#ifdef __cplusplus
}
#endif