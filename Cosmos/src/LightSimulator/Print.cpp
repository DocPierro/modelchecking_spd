//
//  Print.cpp
//  Cosmos
//
//  Created by Alexandru Mereacre on 13/02/2015.
//
//

#include "Print.h"
#include <pthread.h>
#include <iostream>

static pthread_mutex_t gPrintLock = PTHREAD_MUTEX_INITIALIZER;

void print(const char * s){
    pthread_mutex_lock( &gPrintLock );
    std::cerr << s;
    pthread_mutex_unlock( &gPrintLock );
}

void print(TR_PL_ID i){
    pthread_mutex_lock( &gPrintLock );
    std::cerr << (int)i;
    pthread_mutex_unlock( &gPrintLock );
}

void print(REAL_TYPE r){
    pthread_mutex_lock( &gPrintLock );
    std::cerr << r;
    pthread_mutex_unlock( &gPrintLock );
}
