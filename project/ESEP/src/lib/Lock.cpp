/**
 * @file    Lock.cpp
 * @author  Simon Brummer
 * @version 0.1
 *
 * Einfache implementierung des scoped locking patterns.
 * Der Konstruktoraufruf belegt einen Mutex, der
 * Dekonstruktor gibt den Mutex frei. 
 */

#include <pthread.h>
#include "Lock.h"

/**
 * Standard Konstruktor. Belegt uebergebenen Mutex
 * @param pMtx Pointer auf einen Pthread Mutex
 */
Lock::Lock(pthread_mutex_t* pMtx): pMtx_(pMtx) {
    pthread_mutex_lock(pMtx_);
}


/**
 * Dekonstruktor. Gibt den ursprünglich 
 * uebergebenen Mutex wieder frei.
 */
Lock::~Lock() {
    pthread_mutex_unlock(pMtx_);
}
