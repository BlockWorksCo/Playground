#!/bin/bash



# Create some random data as a base64 string.
dd if=/dev/urandom of=blob bs=128 count=1
blob="$(base64 --wrap=0 blob)"

# Form the JSON request using the random BLOB.
echo \{\"TargetList\":\"asdasd\",\"BLOB\":\"${blob}\"\} > job.tmp

# Execute the request to distribute the data to the list of Nodes.
response=$(curl -s --header "Content-Type: application/json" --request POST --data `cat job.tmp` http://localhost:5000/API/DistributionJob)

# Show the response.
echo ${response}


