//
//  Timer1.h
//  Cosmos
//
//  Created by Benoît Barbot on 11/02/2015.
//
//

#ifndef __Cosmos__Timer1__
#define __Cosmos__Timer1__

#ifdef __cplusplus
extern "C"{
#endif


void initTimer1();
unsigned long millis1();
void sleepMillis(unsigned long k);

#define SET_BOTH_LOW(PORT_ONE, PORT_TWO) B00000000
#define SET_BOTH_HIGH(PORT_ONE, PORT_TWO) ((B1<<PORT_ONE) | (B1<<PORT_TWO))
#define SET_ONEH_TWOL(PORT_ONE, PORT_TWO) (B1<<PORT_ONE)
#define SET_ONEL_TWOH(PORT_ONE, PORT_TWO) (B1<<PORT_TWO)

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* defined(__Cosmos__Timer1__) */
