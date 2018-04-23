#ifndef LPC8XX_PMU_H_
#define LPC8XX_PMU_H_

// PCON register shifters
#define PM        0
#define NODPD     3
#define SLEEPFLAG 8
#define DPDFLAG   11

// DPDCTRL register shifters
#define WAKEUPHYS          0
#define WAKEPAD_DISABLE    1
#define LPOSCEN            2
#define LPOSCDPDEN         3
#define WAKEUPCLKHYS       4
#define WAKECLKPAD_DISABLE 5
#define WAKEUP2HYS         6
#define WAKE2PAD_DISABLE   7

#endif // LPC8XX_PMU_H_
