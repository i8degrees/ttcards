#!/bin/sh

../bin/check_json.sh

if [[ $? -eq 0 ]]; then
  xcodebuild -configuration Debug -target ALL_BUILD
else
  echo "${0} ERROR: JSON validation failure has prevented building the project."
  exit 1
fi
