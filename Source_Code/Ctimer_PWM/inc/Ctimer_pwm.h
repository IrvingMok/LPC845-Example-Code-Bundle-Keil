#define PWM_FREQ 100      // in cycles/second
#define PWM_RES  100      // in counts/cycle
#define OUT_PORT PORT0
#define OUT_BIT  Bit25
#define IN_PORT  P0_24
#define WKT_FREQ 10000    // Use if the WKT is clocked by the LPOSC



void setup_debug_uart(void);
void WKT_Config(void);


