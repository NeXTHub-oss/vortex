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
 

#ifndef PREGION_CONFIGS_HPP
#define PREGION_CONFIGS_HPP

namespace Atlas {

typedef uint32_t region_id_t;

const uint32_t kDCacheLineSize_ = 64;
const uint32_t kMaxlen_ = kDCacheLineSize_;
    
const uint64_t kByte_ = 1024;
#ifdef _NVDIMM_PROLIANT
    const uint64_t kPRegionSize_ = 1 * kByte_ * kByte_ * kByte_; /* 1GB */
#else
    const uint64_t kPRegionSize_ = 4 * kByte_ * kByte_ * kByte_; /* 4GB */
#endif    
const uint32_t kMaxNumPRegions_ = 100;
const uint32_t kNumArenas_ = 64;
const uint32_t kArenaSize_ = kPRegionSize_ / kNumArenas_;
const uint32_t kMaxFreeCategory_ = 128;
const uint32_t kInvalidPRegion_ = kMaxNumPRegions_;
const uint32_t kMaxBits_ = 48;
const uint64_t kPRegionsBase_ = 
    (((uint64_t)1 << (kMaxBits_ - 1)) - (kPRegionSize_ * kMaxNumPRegions_))/2;

} // namespace Atlas

#endif
