void pwr_enter_pll(void);
void pwr_leave_pll(void);
bool pwr_in_pll(void);
// enter/leave no-deep-sleep mode
void pwr_enter_tim(void);
void pwr_leave_tim(void);
// go to sleep, deep or shallow
void pwr_sleep(void);
// do WFI until PLL/TIM mode is left
void pwr_wait_tim(void);