#!/bin/sh
#
# https://github.com/emcrisostomo/fswatch

# File or directory path
CP_BIN=/bin/cp        # BSD
# CP_BIN=$(which cp)  # GNU

# source path to watch for file mods
WATCH_PATH=${HOME}/Projects/ttcards.git/Resources/config.json

# destination path to copy modified file to
DEST_PATH=${HOME}/Documents/ttcards/config.json

echo "${0}: Watching ${WATCH_PATH} for file modifications"

WATCHING=true
fswatch -0 "${WATCH_PATH}" | while [ $WATCHING == true ]
do
  read -d "" EVENT
  # echo "${event}"

  if [[ ${EVENT} != "" ]]; then
    ${CP_BIN} -v ${WATCH_PATH} ${DEST_PATH}
  fi

done
