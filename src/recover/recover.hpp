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
 

#ifndef _RECOVER_H
#define _RECOVER_H

#include <map>

using namespace std;
using namespace Atlas;

//namespace Atlas {
    
// There is a one-to-one mapping between a release and an acquire log
// entry, unless the associated lock is a rwlock
typedef multimap<LogEntry*, pair<LogEntry*,int /* thread id */> > MapR2A;
typedef MapR2A::const_iterator R2AIter;
typedef map<int /* tid */, LogEntry* /* last log replayed */> Tid2Log;
typedef map<LogEntry*, bool> MapLog2Bool;
typedef map<uint32_t, bool> MapInt2Bool;
typedef map<LogEntry*, LogEntry*> MapLog2Log;

void R_Initialize(const char *name);
void R_Finalize(const char *name);
LogStructure *GetLogStructureHeader();
void CreateRelToAcqMappings(LogStructure*);
void AddToMap(LogEntry*,int);
void Recover();
void Recover(int);
void MarkReplayed(LogEntry*);
bool isAlreadyReplayed(LogEntry*);

//} // namespace Atlas

#endif
