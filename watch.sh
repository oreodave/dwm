#!/usr/bin/env sh

set -xeuo pipefail

find -type 'f' -name '*.[ch]' | entr -s "make && sudo make install && killall dwm"
