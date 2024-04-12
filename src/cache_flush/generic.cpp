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

namespace Atlas {

void LogMgr::psyncWithAcquireBarrier(void *start_addr, size_t sz)
{
#ifdef _FORCE_FAIL
    fail_program();
#endif
    if (sz <= 0) return;

    char *last_addr = (char*)start_addr + sz - 1;

    char *cacheline_addr =
        (char*)(((uint64_t)start_addr) & PMallocUtil::get_cache_line_mask());
    char *last_cacheline_addr =
        (char*)(((uint64_t)last_addr) & PMallocUtil::get_cache_line_mask());

    full_fence();
    do {
        NVM_CLFLUSH(cacheline_addr);
        cacheline_addr += PMallocUtil::get_cache_line_size();
    }while (cacheline_addr < last_cacheline_addr+1);
}
    
void LogMgr::psync(void *start_addr, size_t sz)
{
    psyncWithAcquireBarrier(start_addr, sz);
    full_fence();
}

void LogMgr::flushAtEndOfFase()
{
#if defined(_FLUSH_LOCAL_COMMIT) && !defined(DISABLE_FLUSHES)
    assert(TL_FaseFlushPtr_);
    if (!TL_FaseFlushPtr_->empty()) {
        flushCacheLines(*TL_FaseFlushPtr_);
        TL_FaseFlushPtr_->clear();
    }
#elif defined(_USE_TABLE_FLUSH) && !defined(DISABLE_FLUSHES)
    syncDataFlush();
#endif
}
        
} // namespace Atlas
