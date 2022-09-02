#!/bin/sh

targetFolder='/Users/Shared/Gig Performer/ThirdParty'

if ! test -d "$targetFolder"; then
   mkdir -p "$targetFolder"
fi


#Get rid of all the DLLs in there so we can do a clean test
#rm -rf "$targetFolder"/*


if ! test -d build; then
   mkdir build
fi
cd build
cmake ..
cmake --build . --config Release
mv *.dylib "$targetFolder"