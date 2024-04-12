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
 

#ifndef LOG_CONFIGS_HPP
#define LOG_CONFIGS_HPP

#include <stdint.h>

namespace Atlas {

const uint64_t kHashTableSize = 1 << 10;
const uint64_t kHashTableMask = kHashTableSize - 1;
const uint32_t kShift = 3;
const uint32_t kWorkThreshold = 100;
const uint32_t kCircularBufferSize = 1024 * 16 - 1;
    
// At limit for using 4 bits
// Combined strncat and strcat, strcpy and strncpy
enum LogType {
    LE_dummy, LE_acquire, LE_rwlock_rdlock, LE_rwlock_wrlock,
    LE_begin_durable, LE_release, LE_rwlock_unlock, LE_end_durable,
    LE_str, LE_memset, LE_memcpy, LE_memmove,
    LE_strcpy, LE_strcat, LE_alloc, LE_free
};

} // namespace Atlas

#endif
