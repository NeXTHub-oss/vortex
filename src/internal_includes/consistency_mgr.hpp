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
 

#ifndef CONSISTENCY_MGR_HPP
#define CONSISTENCY_MGR_HPP

#include <cassert>
#include <utility>
#include <vector>
#include <map>
#include <string>

#include "helper.hpp"
#include "log_mgr.hpp"
#include "durability_graph.hpp"
#include "fase.hpp"

namespace Atlas {

// This class manages consistency of persistent data. It is either
// invoked by the helper thread during program execution or during
// recovery after a failure. Currently, there is at most one instance
// of this class.
class CSMgr {
    static CSMgr *Instance_;
public:
    typedef std::map<DGraph::VDesc, bool> MapNodes;

    // serial mode only
    static CSMgr& createInstance() {
        assert(!Instance_);
        Instance_ = new CSMgr();
        return *Instance_;
    }

    // serial mode only
    static void deleteInstance() {
        assert(Instance_);
        delete Instance_;
        Instance_ = nullptr;
    }

    // serial mode only
    static CSMgr& getInstance() {
        assert(Instance_);
        return *Instance_;
    }

    void doConsistentUpdate(LogStructure*,
                            Helper::LogVersions*, bool is_in_recovery);

    void set_existing_rel_map(Helper::MapLog2Int *m)
        { ExistingRelMap_ = m; }

    boost::graph_traits<DGraph::DirectedGraph>::vertices_size_type
    get_num_graph_vertices() const { return Graph_.get_num_vertices(); }

    void set_is_stable(DGraph::VDesc vertex, bool b)
        { Graph_.set_is_stable(vertex, b); }
    bool is_stable(DGraph::VDesc vertex) const
        { return Graph_.is_stable(vertex); }

private:
    
    typedef std::pair<LogEntry*, DGraph::VDesc> PendingPair;
    typedef std::vector<PendingPair> PendingList;
    
    typedef std::vector<LogStructure*> LSVec;
    typedef std::vector<Helper::LogVersions::iterator> LogIterVec;
    typedef std::map<intptr_t*, bool> Addr2Bool;
    typedef std::map<LogStructure*, FASection*> FaseMap;
    typedef std::vector<FASection*> FaseVec;

    bool IsParentDone_; // Is the parent user thread done?
    bool IsInRecovery_;
    DGraph Graph_;

    // A mapping from a release type log entry to its generation
    // number. This is currently used only during recovery. Populated
    // once and not changed later on at all.
    Helper::MapLog2Int *ExistingRelMap_;

    // A list of acquire type log entries whose targets are not yet found
    PendingList PendingList_;

    // Maps a log header to the first FASE in an analysis round
    FaseMap FirstFaseOfHeader_;

    // Vector of all FASEs created in an analysis round
    FaseVec AllFases_;

    SetOfInts *GlobalFlush_;

    explicit CSMgr() :
        IsParentDone_{false},
        IsInRecovery_{false},
        Graph_{}, 
        ExistingRelMap_{nullptr},
        PendingList_{},
        FirstFaseOfHeader_{},
        AllFases_{},
#if defined(_FLUSH_GLOBAL_COMMIT) && !defined(DISABLE_FLUSHES) && \
    !defined(_DISABLE_DATA_FLUSH)
        GlobalFlush_{new SetOfInts}
#else     
        GlobalFlush_{nullptr}
#endif    
        {}
    
    ~CSMgr()
        {
#if defined(_FLUSH_GLOBAL_COMMIT) && !defined(DISABLE_FLUSHES) && \
    !defined(_DISABLE_DATA_FLUSH)
            if (GlobalFlush_) {
                delete GlobalFlush_;
                GlobalFlush_ = nullptr;
            }
#endif    
        }
    CSMgr(const CSMgr&) = delete;
    CSMgr(CSMgr&&) = delete;
    CSMgr& operator=(const CSMgr&) = delete;
    CSMgr& operator=(CSMgr&&) = delete;

    bool isFirstFaseFound(LogStructure *ls) const
        { return FirstFaseOfHeader_.find(ls) != FirstFaseOfHeader_.end(); }
    void addFirstFase(LogStructure *ls, FASection *fase) {
        assert(FirstFaseOfHeader_.find(ls) == FirstFaseOfHeader_.end());
        FirstFaseOfHeader_.insert(std::make_pair(ls, fase));
    }
    void addFaseToVec(FASection *fase)
        { AllFases_.push_back(fase); }
    void addToPendingList(LogEntry *le, DGraph::VDesc nid) {
        PendingPair pp = std::make_pair(le, nid);
        PendingList_.push_back(pp);
    }
    FASection *getFirstFase(LogStructure *ls) {
        FaseMap::const_iterator ci = FirstFaseOfHeader_.find(ls);
        if (ci != FirstFaseOfHeader_.end()) return ci->second;
        else return NULL;
    }

    void buildInitialGraph(LogStructure *lsp);
    void addSyncEdges(const MapNodes&, LogEntry*, DGraph::VDesc);
    bool isFoundInExistingLog(LogEntry *le, uint64_t gen_num) const;
    void removeUnresolvedNodes();
    void createVersions(Helper::LogVersions *log_v);
    void resolvePendingList();
    void handleUnresolved(DGraph::VDesc nid, MapNodes *rm); 
    void destroyLogs(Helper::LogVersions*);
    void fixupNewEntries(LogStructure**, const LSVec&);
    void copyDeletedLogEntries(LogEntryVec*, const LogEntryVec&);
    void destroyLogEntries(const LogEntryVec&);
    void destroyLS(LogStructure*);
    FASection *buildFASection(LogEntry *le);
    void addLogStructure(LogEntry *le, LogStructure **header,
                         LogStructure **last_header);
    void collectLogs(Log2Bool *logs, FASection *fase);
    bool areLogicallySame(LogStructure *gh, LogStructure *cand_gh);
    uint32_t getNumNewEntries(LogStructure *new_e, LogStructure *old_e);
    void destroyFASections();

    void flushGlobalCommit(const LogEntryVec& logs);

    bool isInRecovery() const { return IsInRecovery_; }
    bool areUserThreadsDone() const
        { return LogMgr::getInstance().areUserThreadsDone(); }
    void traceGraph() {
#if defined(_NVM_TRACE) || defined(_NVM_VERBOSE_TRACE)
        Graph_.trace();
#endif    
    }
    template<class T> void traceHelper(T s) {
#if defined (_NVM_TRACE) || defined(_NVM_VERBOSE_TRACE)
        Helper::getInstance().trace(s);
#endif
    }
    
};

} // namespace Atlas

#endif
