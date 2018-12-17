#!/bin/bash

# Execute the request to distribute the data to the list of Nodes.
response=$(curl -s --header "Content-Type: application/json" --request DELETE http://localhost:5000/API/DistributionJob/$1)

# Show the response.
echo ${response}


