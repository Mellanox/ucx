#
# Copyright (c) 2020. NVIDIA CORPORATION & AFFILIATES. All rights reserved.
#


AC_CHECK_DECLS([inotify_init, inotify_add_watch, IN_ATTRIB],
               [AC_DEFINE([HAVE_INOTIFY], 1, [Enable inotify support])],
               [],
               [[#include <sys/inotify.h>]])

AS_IF([test "x$fuse3_happy" = "xyes"], [ucs_modules="${ucs_modules}:fuse"])
AC_CONFIG_FILES([src/ucs/vfs/fuse/Makefile])
