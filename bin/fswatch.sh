#!/bin/sh
#
# https://github.com/emcrisostomo/fswatch

# File or directory path
WATCH_PATH=${HOME}/Projects/ttcards.git/Resources/config.json
CP_BIN=/bin/cp        # BSD
# CP_BIN=$(which cp)  # GNU

# FIXME: There is currently no output from this script =(
fswatch -o ${WATCH_PATH} | while true; do
  ${CP_BIN} ${WATCH_PATH} ${HOME}/Documents/ttcards/config.json
done
