#!/bin/sh
set -e
. ./headers.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECT && DEST_D="$SYSROOT" $MAKE install)
done
