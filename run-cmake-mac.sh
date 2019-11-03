#!/bin/bash

set -ex

cmake -GXcode -BBuild
xcodebuild build -configuration Release -project Build/sfz-read.xcodeproj

echo "Running Built Result"

./Build/Release/sfz-read-main "/Library/Application Support/Plogue/Free Sounds" "Programs/Plogue/Toy Piano.sfz"
