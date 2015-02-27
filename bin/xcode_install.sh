#!/bin/sh

if [[ $? -eq 0 ]]; then
  xcodebuild -configuration Debug -target install
else
  exit 1
fi
