#!/bin/sh -eEx
#
# Copyright (c) 2001-2016. NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# See file LICENSE for terms.
#
# Run this from UCX build directory.
#

rev=$(git rev-parse --short HEAD)
make docs
mkdir -p ucx.wiki
cd ucx.wiki
git init .
git remote show origin &>/dev/null || git remote add origin https://github.com/openucx/ucx.wiki.git
git fetch --all
git checkout -t origin/master -f
git pull
cp -f ../docs/doxygen-doc/ucx.pdf ./
git commit ucx.pdf -m "update ucx.pdf for $rev"
git push
