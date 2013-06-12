#! /bin/sh

find . -name "*.fta" -exec dos2unix {} {} \;
find . -name "*.ped" -exec dos2unix {} {} \;
find . -name "*.mcs" -exec dos2unix {} {} \;
