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
 

#include "log_mgr.hpp"

void AsyncDataCacheFlush(void *p);
void AsyncMemOpDataCacheFlush(void *dst, size_t sz);
void SyncDataCacheFlush();

namespace Atlas {

#if 0 // unused
void LogMgr::asyncLogFlush(void *p)
{
    // Since this is a log entry, we don't need to check whether it is
    // persistent or not. It must be persistent.
    intptr_t *entry = TL_LogFlushTab_ +
        (((intptr_t)p >> kFlushShift) & kFlushTableMask);
    intptr_t cache_line = (intptr_t)p & PMallocUtil::get_cache_line_mask();

    if (*entry != cache_line) {
        if (*entry) {
            full_fence();
            NVM_CLFLUSH(*entry);
        }
        *entry = cache_line;
    }
}

void LogMgr::syncLogFlush()
{
    int i;
    full_fence();
    for (i=0; i<kFlushTableSize; ++i) {
        intptr_t *entry = TL_LogFlushTab_ + i;
        if (*entry) {
            NVM_CLFLUSH(*entry);
            *entry = 0;
        }
    }
    full_fence();
}
#endif
    
#if defined(_USE_TABLE_FLUSH)
void LogMgr::asyncDataFlush(void *p)
{
#ifdef _FORCE_FAIL
    fail_program();
#endif
    if (!NVM_IsInOpenPR(p, 1)) return;

    intptr_t *entry = TL_DataFlushTab_ +
        (((intptr_t)p >> kFlushShift) & kFlushTableMask);
    intptr_t cache_line = (intptr_t)p & PMallocUtil::get_cache_line_mask();

    if (*entry != cache_line) {
        if (*entry) {
            full_fence();
            NVM_CLFLUSH(*entry);
        }
        *entry = cache_line;
    }
}

void LogMgr::asyncMemOpDataFlush(void *dst, size_t sz)
{
#ifdef _FORCE_FAIL
    fail_program();
#endif
    if (!NVM_IsInOpenPR(dst, 1)) return;

    if (sz <= 0) return;
    
    char *last_addr = (char*)dst + sz - 1;
    char *cacheline_addr =
        (char*)(((uint64_t)dst) & PMallocUtil::get_cache_line_mask());
    char *last_cacheline_addr =
        (char*)(((uint64_t)last_addr) & PMallocUtil::get_cache_line_mask());

    intptr_t *entry;
    full_fence();
    do {
        entry = TL_DataFlushTab_ +
            (((intptr_t)cacheline_addr >> kFlushShift) & kFlushTableMask);
        if (*entry != (intptr_t)cacheline_addr) {
            if (*entry) NVM_CLFLUSH(*entry);
            *entry = (intptr_t)cacheline_addr;
        }
        cacheline_addr += PMallocUtil::get_cache_line_size();
    }while (cacheline_addr < last_cacheline_addr+1);
}

void LogMgr::syncDataFlush()
{
#ifdef _FORCE_FAIL
    fail_program();
#endif
    int i;
    full_fence();
    for (i=0; i<kFlushTableSize; ++i) {
        intptr_t *entry = TL_DataFlushTab_ + i;
        if (*entry) {
            NVM_CLFLUSH(*entry);
            *entry = 0;
        }
    }
    full_fence();
}
#endif

} // namespace Atlas
