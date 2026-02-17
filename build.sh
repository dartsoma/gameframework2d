#!/bin/bash
echo "Making File..."
make -C src 2> crash.log
[ -s crash.log ] || ./gf2d

