#!/bin/bash


rm -rf C
rm Patch.sh
rm -rf StagingArea
mkdir StagingArea
cp -R A C


python3 ../../MakeFileSystemPatch.py $PWD/A $PWD/B $PWD/StagingArea


pushd StagingArea
./Patch.sh $PWD/C
popd
