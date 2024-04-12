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

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FP (void)fprintf

static void bail(char *msg, int line) {
    FP(stderr, "%s:%d: %s: %s\n", __FILE__, line, msg, strerror(errno));
    exit(EXIT_FAILURE);
}
#define BAIL(msg) bail(msg, __LINE__)

static void trim_rightmost_path_component(char *p) {
    char *s = p + strlen(p) - 1;
    if ('/' == *s)
        *s-- = '\0';
    while (s > p && '/' != *s)
        s--;
    *++s = '\0';
}

static int fsync_paranoid(const char *name) {
    char rp[1+PATH_MAX], *file = (char *) malloc(sizeof(char)*(strlen(name)+1));
    strcpy(file, name);
    FP(stderr, "fsync to root '%s'\n", file);
    if (NULL == realpath(file, rp))              BAIL("realpath failed");
    FP(stderr, "     realpath '%s'\n", rp);
    do {
        int fd;
            FP(stderr, "    fsync-ing '%s'\n", rp);
            if (-1 == (fd = open(rp, O_RDONLY)))       BAIL("open failed");
            if (-1 == fsync(fd))                       BAIL("fsync failed");
            if (-1 == close(fd))                       BAIL("close failed");
            trim_rightmost_path_component(rp);
        } while (*rp);
    FP(stderr, "         done\n");
    free(file);
    return 0;
}

static int fsync_dir(const char *name) 
{
    char *file = (char*) malloc(sizeof(char)*(strlen(name)+1));
    strcpy(file, name);
    trim_rightmost_path_component(file);
    FP(stderr, "    fsync-ing '%s'\n", file);
    int fd;
    if (-1 == (fd = open(file, O_RDONLY)))       BAIL("open failed");
    if (-1 == fsync(fd))                       BAIL("fsync failed");
    if (-1 == close(fd))                       BAIL("close failed");
    free(file);
    return 0;
}
