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
 

#include <stdio.h>
#include <stdlib.h>

#include "atlas_alloc.h"
#include "util.hpp"
#include "pregion_configs.hpp"
#include "pregion_mgr.hpp"

using namespace Atlas;

// The input should be the name of the executable whose log is to be deleted
int main(int argc, char **argv)
{
    char input[kMaxlen_];
    
    assert(argc == 2);

    PRegionMgr::createInstance();

    char *log_name = NVM_GetLogRegionName(argv[1]);
    PRegion *rgn = nullptr;
    if (!(rgn = PRegionMgr::getInstance().searchPRegion(log_name))) {
        fprintf(stderr, "Log for %s not found, nothing to do\n", argv[1]);
        return 0;
    }
    fprintf(stderr, "Are you sure you want to delete log for %s (yes/no)? ",
            argv[1]);

    while (true)
    {
        char *s = fgets(input, kMaxlen_, stdin);
        if (!strcmp(s, "yes\n"))
        {
            PRegionMgr::getInstance().deleteForcefullyPRegion(log_name);

            PRegionMgr::deleteInstance();

            fprintf(stderr, "Log for %s successfully deleted\n", argv[1]);
            break;
        }
        else if (!strcmp(s, "no\n")) break;
        else fprintf(stderr, "Please enter yes or no: ");
    }
    return 0;
}
    
