#!/bin/bash



dd if=/dev/urandom of=blob bs=128 count=1
blob="$(base64 --wrap=0 blob)"

echo \{\"TargetList\":\"asdasd\",\"BLOB\":\"${blob}\"\} > job.tmp
response=$(curl -s --header "Content-Type: application/json" --request POST --data `cat job.tmp` http://localhost:5000/API/DistributionJob)

echo ${response}


