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
 

#ifndef FASE_HPP
#define FASE_HPP

#include "log_mgr.hpp"

namespace Atlas {

// Represent a failure atomic section of code.    
struct FASection {
    explicit FASection(LogEntry *first, LogEntry *last) 
        : First{first},
        Last{last},
        Next{nullptr},
        IsDeleted{false} {}
    FASection() = delete;
    FASection(const FASection&) = delete;
    FASection(FASection&&) = delete;
    FASection& operator=(const FASection&) = delete;
    FASection& operator=(FASection&&) = delete;
    
    LogEntry *First;
    LogEntry *Last;
    FASection *Next;
    bool IsDeleted;
};

} // namespace Atlas

#endif
