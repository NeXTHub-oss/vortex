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
 

#include "consistency_mgr.hpp"

namespace Atlas {

CSMgr *CSMgr::Instance_{nullptr};

///
/// @brief Main entry point for a single round of consistent state gen
/// @param lsp Pointer to the first thread specific header
/// @param log_v Pointer to the versions of consistent states
/// @param is_in_recovery Whether invoked online or during recovery
///
/// Build a graph of completed failure atomic sections (FASE) with
/// durability edges among them. All log entries of a FASE are in a
/// consistent state if all log entries they transitively happen-after
/// are also in the same consistent state. Failure-atomically removing
/// these log entries advances the persistent consistent state.
///    
void CSMgr::doConsistentUpdate(LogStructure *lsp,
                               Helper::LogVersions *log_v,
                               bool is_in_recovery)
{
#ifdef _FORCE_FAIL
    fail_program();
#endif
    IsInRecovery_ = is_in_recovery;
    
    // TODO incorporate consistency analysis profiling if required

    buildInitialGraph(lsp);
    
    if (IsParentDone_) return;

    // If there is any pending entry from the initial graph created
    // above, examine whether it can be resolved.
    resolvePendingList();
    if (areUserThreadsDone()) {
        IsParentDone_ = true;
        return;
    }

    // If there is still any unresolved log entry, it cannot belong to
    // a consistent state.
    removeUnresolvedNodes();
    if (areUserThreadsDone()) {
        IsParentDone_ = true;
        return;
    }

    // TODO: If we are in recovery phase and the resolved graph is
    // empty, we should increase the candidate log entries chosen and
    // then try again. 
    if (IsInRecovery_ && !get_num_graph_vertices()) return;

    // Create versions of consistent states
    createVersions(log_v);
    if (areUserThreadsDone()) {
        IsParentDone_ = true;
        return;
    }

    // Remove the log entries failure-atomically.
    destroyLogs(log_v);

    // Clean up FASEs created for this round.
    destroyFASections();
}

} // namespace Atlas
