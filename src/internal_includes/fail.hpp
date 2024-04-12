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
 

#ifndef FAIL
#define FAIL

#include <cstdlib>
#include <iostream>
#include <execinfo.h>
#include <atomic>
#define FAIL_MAX 100
static std::atomic<int> fail_chance(0);
inline void fail_program () {
    if (rand() % FAIL_MAX <= fail_chance.load()){
        void *array[10];
        size_t size;
        char **strings;
        size_t i;

        size = backtrace (array, 10);
        strings = backtrace_symbols (array, size);

        for (i = 0; i < size; i++)
            std::cout << strings[i] << "\n";
        delete (strings);
        std::exit(0);
    } else {
        ++fail_chance;
    }
}
#endif
