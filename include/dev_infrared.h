#ifndef CURRENT_H_INCLUDED
#define CURRENT_H_INCLUDED

extern uint32_t vbus;
extern uint32_t current[4];

void current_init(void);
void current_update(void);

#endif // CURRENT_H_INCLUDED
