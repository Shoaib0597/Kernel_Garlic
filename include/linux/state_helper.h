#ifndef __LINUX_STATE_HELPER_H
#define __LINUX_STATE_HELPER_H

extern void reschedule_helper(void);
extern void batt_level_notify(int);
extern void load_notify (unsigned int cpu, unsigned int k);

#endif /* _LINUX_STATE_HELPER_H */
