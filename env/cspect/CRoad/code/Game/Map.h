
#ifndef __MAP_H__
#define __MAP_H__

#include <stdint.h>

extern void 	Map_Init(void);
extern void 	Map_Load(void);
extern void 	Map_Process(void);
extern void 	Map_Render(void);
extern void 	Map_Quit(void);

extern	char*	Map_In(void);
extern	void 	Map_Out(void);


 #endif // __MAP_H__





