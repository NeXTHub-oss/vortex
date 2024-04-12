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
 

#ifndef PREGION_MGR_UTIL_HPP
#define PREGION_MGR_UTIL_HPP

#include <map>
#include <utility>

#include "pregion_configs.hpp"

namespace Atlas {

class PRegionExtentMap {
public:
    typedef std::pair<intptr_t,intptr_t> IntPtrPair;
    class CmpIntPtr {
    public:
        bool operator()(
            const IntPtrPair & c1, const IntPtrPair & c2) const {
            return (c1.first < c2.first) &&
                (c1.second < c2.second);
        }
    };
    typedef std::map<IntPtrPair,uint32_t,CmpIntPtr> MapInterval;

    PRegionExtentMap() = default;
    PRegionExtentMap(const PRegionExtentMap& from) {
        MapInterval::const_iterator ci_end = from.Extents_.end();
        for (MapInterval::const_iterator ci =
                 from.Extents_.begin(); ci != ci_end; ++ ci)
            insertExtent(ci->first.first, ci->first.second, ci->second);
    }

    void insertExtent(intptr_t first, intptr_t last, uint32_t id)
        { Extents_[std::make_pair(first,last)] = id; }

    void deleteExtent(intptr_t first, intptr_t last, uint32_t id) {
        MapInterval::iterator ci = Extents_.find(
            std::make_pair(first,last));
        if (ci != Extents_.end()) Extents_.erase(ci);
    }

    uint32_t findExtent(intptr_t first, intptr_t last) const {
        MapInterval::const_iterator ci = Extents_.find(
            std::make_pair(first,last));
        if (ci != Extents_.end()) return ci->second;
        return kInvalidPRegion_;
    }
private:
    MapInterval Extents_;
};
    
} // namespace Atlas
            
#endif


