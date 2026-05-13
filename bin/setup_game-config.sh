#!/usr/bin/env bash
#
# 2016-03/25:jeff
#
# Post installation script for setup of the game configuration

# Setup environment for mkdir and friends
PATH=/bin:/sbin:/usr/bin

APP_NAME=TTcards

case "$(uname -s)" in
  Darwin)
    GAME_CONFIG_PREFIX="${HOME}/Library/Application Support"
  ;;

  Linux)
    GAME_CONFIG_PREFIX="${HOME}/.config" # XDG
  ;;

  Windows) # FIXME(jeff): I doubt that this is a valid definition!
    # TODO(jeff): Verify this path
    GAME_CONFIG_PREFIX="${HOME}/AppData/Local"
    exit -1
  ;;

  *)
    # ...Pray for a POSIX-compliant platform!
    GAME_CONFIG_PREFIX="${HOME}/.config"
  ;;
esac

mkdir "${GAME_CONFIG_PREFIX}"
mkdir "${GAME_CONFIG_PREFIX}/${APP_NAME}"
mkdir "${GAME_CONFIG_PREFIX}/${APP_NAME}/Cards"
mkdir "${GAME_CONFIG_PREFIX}/${APP_NAME}/SavedGames"
mkdir "${GAME_CONFIG_PREFIX}/${APP_NAME}/Screenshots"

# if [[ ! (-f "${GAME_CONFIG_PREFIX}/${APP_NAME}/cards.json") ]]; then
#   cp ../Resources/cards.json \
#     "${GAME_CONFIG_PREFIX}/${APP_NAME}/cards.json"
# fi

if [[ ! (-f "${GAME_CONFIG_PREFIX}/${APP_NAME}/config_game.json") ]]; then
  cp ../Resources/config_game.json \
    "${GAME_CONFIG_PREFIX}/${APP_NAME}/config_game.json"
fi

if [[ ! (-f "${GAME_CONFIG_PREFIX}/${APP_NAME}/config_assets-hi-res.json") ]]; then
  cp ../Resources/config_assets-hi-res.json \
    "${GAME_CONFIG_PREFIX}/${APP_NAME}/config_assets-hi-res.json"
fi

if [[ ! (-f "${GAME_CONFIG_PREFIX}/${APP_NAME}/config_assets-low-res.json") ]]; then
  cp ../Resources/config_assets-low-res.json \
    "${GAME_CONFIG_PREFIX}/${APP_NAME}/config_assets-low-res.json"
fi
