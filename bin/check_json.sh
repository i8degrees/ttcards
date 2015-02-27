#!/bin/sh

JSHINT_BIN=$(which jshint)

if [[ -x $JSHINT_BIN ]]; then
  jshint ../Resources/config_game.json
  jshint ../Resources/config_assets-low-res.json
  jshint ../Resources/config_assets-hi-res.json
else
  echo "${0} ERROR: Could not find jshint executable"
  exit 1
fi
