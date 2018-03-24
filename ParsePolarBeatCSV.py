


import csv
import sys
import re
import json


with open(sys.argv[1], 'rb') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    rowCount    = 0
    hrData      = []
    for row in spamreader:
        if rowCount > 2:
            #print(row[1])
            #print(row[2])
            timeTriple  = re.compile('([0-9]+):([0-9]+):([0-9]+)').findall(row[1])[0]
            #print(timeTriple)
            timestamp   = int(timeTriple[0])*3600 + int(timeTriple[1])*60 + int(timeTriple[2])
            hr          = int(row[2])
            hrData.append([timestamp,hr])
            #print(hr)
            #print(timestamp)
        rowCount    = rowCount+1

    jsonHRData  = json.dumps(hrData)
    print(jsonHRData)


