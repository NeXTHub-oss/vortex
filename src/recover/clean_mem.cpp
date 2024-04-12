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
 

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>

#include "atlas_alloc.h"
#include "util.hpp"
#include "pregion_configs.hpp"
#include "pregion_mgr.hpp"

using namespace Atlas;

int main(int argc, char **argv)
{
    char input[kMaxlen_];
    
    assert(argc < 3);

    if (argc == 1)
    {
        fprintf(stderr,
                "Are you sure you want to delete all regions (yes/no)? ");
        while (true)
        {
            char *s = fgets(input, kMaxlen_, stdin);
            if (!strcmp(s, "yes\n")) break;
            else if (!strcmp(s, "no\n")) return 0;
            else fprintf(stderr, "Please enter yes or no: ");
        }
    }
    else if (strcmp(argv[1], "-f"))
    {
        fprintf(stderr, "Unknown option: use clean_mem -f\n");
        return 0;
    }

    PRegionMgr::createInstance();

    PRegionMgr::getInstance().deleteForcefullyAllPRegions();

    PRegionMgr::deleteInstance();
    
    char *rgn_tab = NVM_GetRegionTablePath();
    unlink(rgn_tab);
    free(rgn_tab);
    fprintf(stderr, "All region data/metadata removed successfully\n");
    return 0;
}

