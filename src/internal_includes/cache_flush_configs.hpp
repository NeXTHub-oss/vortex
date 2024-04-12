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
 

#ifndef CACHE_FLUSH_CONFIGS_HPP
#define CACHE_FLUSH_CONFIGS_HPP

namespace Atlas {

const int32_t kFlushTableSize = 8;
const int32_t kFlushTableMask = kFlushTableSize - 1;
// TODO the following should be derived from cache line size
const uint32_t kFlushShift = 6; // log(cache line size)
    
} // namespace Atlas

#endif
