#!/bin/bash


rm -rf C
rm Patch.sh
rm -rf StagingArea
mkdir StagingArea
cp -R A C


python3 ../../MakeFileSystemPatch.py $PWD/A $PWD/B $PWD/StagingArea


pushd StagingArea
chmod a+x Patch.sh
./Patch.sh $PWD/../C
popd
