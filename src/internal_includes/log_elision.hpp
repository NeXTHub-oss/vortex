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
 

#ifndef LOG_ELISION_HPP
#define LOG_ELISION_HPP

#include <atomic>

namespace Atlas {
    
struct LockReleaseCount
{
    explicit LockReleaseCount(void *addr, uint64_t count)
        : LockAddr{addr},
        Count{count},
        Next{nullptr} {}
    LockReleaseCount() = delete;
    LockReleaseCount(const LockReleaseCount&) = delete;
    LockReleaseCount(LockReleaseCount&&) = delete;
    LockReleaseCount& operator=(const LockReleaseCount&) = delete;
    LockReleaseCount& operator=(LockReleaseCount&&) = delete;
    
    void *LockAddr;
    std::atomic<uint64_t> Count;
    LockReleaseCount *Next;
};

} // namespace Atlas

#endif
