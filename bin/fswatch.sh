#!/bin/bash
#
# Usage example:
#
# First, be sure to update the shell script path in org.ttcards.fswatch.plist
# and anything in this script that needs matching to your local environment.
#
# Be sure this script has executable perms:
# chmod +x ~/Projects/ttcards.git/bin/fswatch.sh

# Create symbolic link from the launchd plist to your user account's
# LaunchAgents folder:
# ln -sT ~/Projects/ttcards.git/bin/org.ttcards.fswatch.plist ~/Library/LaunchAgents/org.ttcards.fswatch.plist

# Register launchd script with launchd:
# launchctl load -w ~/Library/LaunchAgents/org.ttcards.fswatch.plist
#
# Verify process IDs:
# launchctl list|grep org.ttcards.fswatch

# fswatch brew package; https://github.com/emcrisostomo/fswatch
FSWATCH_BIN=/usr/local/bin/fswatch

# For whatever bloody reason, launchd absolutely refuses to allow me to access
# anything under /bin ...
# CP_BIN=/bin/cp        # System-distributed (BSD under OSX)

# GNU cp from coreutils homebrew package
CP_BIN="/usr/local/opt/coreutils/libexec/gnubin/cp"

# source path to watch for file mods
BASE_WATCH_PATCH=${HOME}/Projects/ttcards.git/Resources
CONFIG_GAME_WATCH_PATH=${BASE_WATCH_PATCH}/config_game.json
CONFIG_GAME_ASSETS_LOW_RES_WATCH_PATH=${BASE_WATCH_PATCH}/config_assets-low-res.json
CONFIG_GAME_ASSETS_HI_RES_WATCH_PATH=${BASE_WATCH_PATCH}/config_assets-hi-res.json

# destination path to copy modified file to
CONFIG_DIR_BASE=${HOME}/Library/Application\ Support/TTcards
CONFIG_GAME_DEST_PATH=${CONFIG_DIR_BASE}/config_game.json

# FIXME: These paths do not work ... I don't know why
CONFIG_GAME_ASSETS_LOW_RES_DEST_PATH=${CONFIG_DIR_BASE}/config_assets-low-res.json
CONFIG_GAME_ASSETS_HI_RES_DEST_PATH=${CONFIG_DIR_BASE}/config_assets-hi-res.json

# Enable debug output logging to a file path
# LOGFILE=$HOME/Library/Logs/org.ttcards.fswatch.log

if [[ ${LOGFILE} != "" ]]; then
  # env sanity checks
  echo "${0} DEBUG\n" > ${LOGFILE}
  ${FSWATCH_BIN} --version >> ${LOGFILE}
  which read >> ${LOGFILE}
  ${CP_BIN} --version >> ${LOGFILE}
fi

echo "${0}: Watching ${CONFIG_GAME_WATCH_PATH} for file modifications"
echo "${0}: Watching ${CONFIG_GAME_ASSETS_LOW_RES_WATCH_PATH} for file modifications"
echo "${0}: Watching ${CONFIG_GAME_ASSETS_HI_RES_WATCH_PATH} for file modifications"

WATCHING=true
${FSWATCH_BIN} -0 "${CONFIG_GAME_WATCH_PATH}" | while [ $WATCHING == true ]
do
  read -d "" EVENT

  if [[ ${EVENT} != "" ]]; then

    # Output command results to debug log if LOGFILE is set (see above)
    if [[ ${LOGFILE} == "" ]]; then
      ${CP_BIN} -v "${CONFIG_GAME_WATCH_PATH}" "${CONFIG_GAME_DEST_PATH}"
    else
      ${CP_BIN} -v "${CONFIG_GAME_WATCH_PATH}" "${CONFIG_GAME_DEST_PATH}" >> ${LOGFILE}
    fi
  fi
done

WATCHING=true
${FSWATCH_BIN} -0 "${CONFIG_GAME_ASSETS_LOW_RES_WATCH_PATH}" | while [ $WATCHING == true ]
do
  read -d "" EVENT

  if [[ ${EVENT} != "" ]]; then

    # Output command results to debug log if LOGFILE is set (see above)
    if [[ ${LOGFILE} == "" ]]; then
      ${CP_BIN} -v "${CONFIG_GAME_ASSETS_LOW_RES_WATCH_PATH}" "${CONFIG_GAME_ASSETS_LOW_RES_DEST_PATH}"
    else
      ${CP_BIN} -v "${CONFIG_GAME_ASSETS_LOW_RES_WATCH_PATH}" "${CONFIG_GAME_ASSETS_LOW_RES_DEST_PATH}" >> ${LOGFILE}
    fi
  fi
done

WATCHING=true
${FSWATCH_BIN} -0 "${CONFIG_GAME_ASSETS_HI_RES_WATCH_PATH}" | while [ $WATCHING == true ]
do
  read -d "" EVENT

  if [[ ${EVENT} != "" ]]; then

    # Output command results to debug log if LOGFILE is set (see above)
    if [[ ${LOGFILE} == "" ]]; then
      ${CP_BIN} -v "${CONFIG_GAME_ASSETS_HI_RES_WATCH_PATH}" "${CONFIG_GAME_ASSETS_HI_RES_DEST_PATH}"
    else
      ${CP_BIN} -v "${CONFIG_GAME_ASSETS_HI_RES_WATCH_PATH}" "${CONFIG_GAME_ASSETS_HI_RES_DEST_PATH}" >> ${LOGFILE}
    fi
  fi
done
