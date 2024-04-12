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
 

#ifndef HAPPENS_BEFORE_HPP
#define HAPPENS_BEFORE_HPP

#include <atomic>
#include <map>

#include "log_structure.hpp"

namespace Atlas {
    
typedef std::map<void*,uint64_t> MapOfLockInfo;

// This structure is currently used in a write-once manner. It contains
// the core information within an entry in a hash table. If any of the
// components needs to be changed, the update is done in a copy-on-write
// manner.
struct ImmutableInfo
{
    explicit ImmutableInfo(LogEntry *le, MapOfLockInfo *linfo, bool is_del) 
        : LogAddr{le},
        LockInfoPtr{linfo},
        IsDeleted{is_del} {}
    ImmutableInfo() = delete;
    ImmutableInfo(const ImmutableInfo&) = delete;
    ImmutableInfo(ImmutableInfo&&) = delete;
    ImmutableInfo& operator=(const ImmutableInfo&) = delete;
    ImmutableInfo& operator=(ImmutableInfo&&) = delete;

    // Here LogAddr_ is the address of the log entry of the last release
    // operation of the corresponding synchronization object.
    LogEntry *LogAddr;

    // The following map contains the set of locks (and their counts)
    // that *this* lock *depends* on. This *dependence* relation is
    // established at the point *this* lock is released. At any point
    // of time, there can be at most one writer since only one thread
    // can release *this* lock. But there can be multiple readers who
    // may be examining this map (e.g. rw-locks).
    MapOfLockInfo *LockInfoPtr;

    // The following field indicates whether this entry is obsolete
    bool IsDeleted;
};

struct LastReleaseInfo
{
    explicit LastReleaseInfo(ImmutableInfo *ii)
        : Immutable{ii},
        Next{nullptr} {}
    LastReleaseInfo() = delete;
    LastReleaseInfo(const LastReleaseInfo&) = delete;
    LastReleaseInfo(LastReleaseInfo&&) = delete;
    LastReleaseInfo& operator=(const LastReleaseInfo&) = delete;
    LastReleaseInfo& operator=(LastReleaseInfo&&) = delete;
    
    // This is updated with a read-modify-write operation since there may
    // be multiple concurrent writers. A worker thread may be a writer. The
    // helper thread may try to delete this element making it a writer too.
    std::atomic<ImmutableInfo*> Immutable;
    
    // The Next_ field typically will be atomic but in this case, it
    // turns out that this field is updated before it is published. It is
    // also write-once.
    LastReleaseInfo *Next;
};

} // namespace Atlas

#endif
