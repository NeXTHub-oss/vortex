/*
 * Copyright (c) 2024, ITGSS Corporation. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * Contact with ITGSS, 651 N Broad St, Suite 201, in the
 * city of Middletown, zip code 19709, and county of New Castle, state of Delaware.
 * or visit www.it-gss.com if you need additional information or have any
 * questions.
 *
 */
 

#ifndef STATS_HPP
#define STATS_HPP

#include <cstdlib>
#include <cassert>

#include <stdint.h>
#include <pthread.h>

namespace Atlas {

class Stats {
    static Stats *Instance_;
public:
    
    static Stats& createInstance() {
        assert(!Instance_);
        Instance_ = new Stats();
        return *Instance_;
    }

    static void deleteInstance() {
        assert(Instance_);
        delete Instance_;
        Instance_ = nullptr;
    }

    static Stats& getInstance() {
        assert(Instance_);
        return *Instance_;
    }

    void acquireLock()
        { int status = pthread_mutex_lock(&Lock_); assert(!status); }
    void releaseLock()
        { int status = pthread_mutex_unlock(&Lock_); assert(!status); }

    void incrementCriticalSectionCount()
        { ++TL_CriticalSectionCount; }
    void incrementNestedCriticalSectionCount()
        { ++TL_NestedCriticalSectionCount; }
    void incrementLoggedStoreCount()
        { ++TL_LoggedStoreCount; }
    void incrementCriticalLoggedStoreCount()
        { ++TL_CriticalLoggedStoreCount; }
    void incrementUnloggedStoreCount()
        { ++TL_UnloggedStoreCount; }
    void incrementLogElisionFailCount()
        { ++TL_LogElisionFailCount; }
    void incrementUnloggedCriticalStoreCount()
        { ++TL_UnloggedCriticalStoreCount; }
    void incrementLogMemUse(size_t sz)
        { TL_LogMemUse += sz; }

    void print();
    
private:
    pthread_mutex_t Lock_;
    
    // Computed as the number of lock acquires
    thread_local static uint64_t TL_CriticalSectionCount;

    // Given a lock acquire operation, if there is a lock already held 
    thread_local static uint64_t TL_NestedCriticalSectionCount;

    // Total number of writes logged (memset/memcpy, etc. counted as 1)
    thread_local static uint64_t TL_LoggedStoreCount;

    // Total number of writes encountered within critical sections
    thread_local static uint64_t TL_CriticalLoggedStoreCount;

    // Total number of writes not logged
    thread_local static uint64_t TL_UnloggedStoreCount;

    // Total number of writes for which log elision failed
    thread_local static uint64_t TL_LogElisionFailCount;

    // Total number of writes not logged within critical sections
    thread_local static uint64_t TL_UnloggedCriticalStoreCount;

    // Total memory used by the program log
    thread_local static uint64_t TL_LogMemUse;

    // Total number of CPU cache flushes for logging
    thread_local static uint64_t TL_NumLogFlushes;
};

} // namespace Atlas
    
#endif
