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
 

#include "atlas_alloc.h"
#include "atlas_alloc_cpp.hpp"
#include "pregion_mgr.hpp"

using namespace Atlas;

uint32_t NVM_FindOrCreateRegion(const char *name, int flags, int *is_created)
{
    return PRegionMgr::getInstance().findOrCreatePRegion(
        name, flags, is_created);
}
    
uint32_t NVM_FindRegion(const char *name, int flags)
{
    // TODO What is the behavior if the region is not found?
    return PRegionMgr::getInstance().findPRegion(name, flags);
}

uint32_t NVM_CreateRegion(const char *name, int flags)
{
    return PRegionMgr::getInstance().createPRegion(name, flags);
}

void NVM_CloseRegion(uint32_t rid)
{
    PRegionMgr::getInstance().closePRegion(rid);
}

void NVM_DeleteRegion(const char *name)
{
    PRegionMgr::getInstance().deletePRegion(name);
}

void *NVM_GetRegionRoot(uint32_t rid) 
{
    return PRegionMgr::getInstance().getPRegionRoot(rid);
}

void NVM_SetRegionRoot(uint32_t rid, void *new_root)
{
    PRegionMgr::getInstance().setPRegionRoot(rid, new_root);
}

void *nvm_alloc(size_t sz, uint32_t rid)
{
    bool does_need_cache_line_alignment = false;
    bool does_need_logging = true;
    return PRegionMgr::getInstance().allocMem(
        sz, rid, does_need_cache_line_alignment, does_need_logging);
}

void *nvm_calloc(size_t nmemb, size_t sz, uint32_t rid)
{
    return PRegionMgr::getInstance().callocMem(nmemb, sz, rid);
}

void *nvm_realloc(void *ptr, size_t sz, uint32_t rid)
{
    return PRegionMgr::getInstance().reallocMem(ptr, sz, rid);
}

void nvm_free(void *ptr)
{
    PRegionMgr::getInstance().freeMem(ptr);
}

void nvm_delete(void *ptr)
{
    PRegionMgr::getInstance().deleteMem(ptr);
}

// TODO: The following should probably go away, they are really not
// part of the API.
int NVM_IsInOpenPR(void *addr, size_t sz)
{
    // PRegion manager has to be present to proceed with logging
    if (!PRegionMgr::hasInstance()) return false;
#if defined(_ALL_PERSISTENT)
    return true;
#else
    return PRegionMgr::getInstance().isInOpenPRegion(addr, sz);
#endif    
}

int isOnDifferentCacheLine(void *p1, void *p2)
{
    return PMallocUtil::is_on_different_cache_line(p1, p2);
}

int isCacheLineAligned(void *p)
{
    return PMallocUtil::is_cache_line_aligned(p);
}

int NVM_IsInRegion(void *ptr, size_t sz)
{
    return PRegionMgr::getInstance().isInOpenPRegion(ptr, sz);
}

PRegion *NVM_GetRegion(uint32_t rid)
{
    return PRegionMgr::getInstance().getPRegion(rid);
}

void* operator new(size_t sz, Atlas::PRegion *rgn) noexcept
{
    assert(rgn);
    return nvm_alloc(sz, rgn->get_id());
}

void* operator new[](size_t sz, Atlas::PRegion *rgn) noexcept
{
    assert(rgn);
    return nvm_alloc(sz, rgn->get_id());
}

        
