


import csv
import sys
import re
import json
import pymysql.cursors


# Connect to the database
connection = pymysql.connect(host='localhost',
                             user='fitness',
                             password='fitness',
                             db='fitness',
                             charset='utf8mb4',
                             cursorclass=pymysql.cursors.DictCursor)



def InsertHRDataIntoDatabase(name,date,time,hrData):

    try:
        with connection.cursor() as cursor:
            # Create a new record
            sql = "INSERT INTO `workout` (`userID`, `time`, `hrProfile`) VALUES ('%s', STR_TO_DATE('%s','%%d-%%m-%%Y %%h:%%i:%%s'), '%s' )"
            #cursor.execute(sql, ('0', "20111218131717", hrData))
            sql = sql%('0', date+' '+time, hrData)
            print(sql)
            cursor.execute(sql)

            d="STR_TO_DATE('%s','%%d-%%m-%%Y %%h:%%m:s')"
            # INSERT INTO `workout` (`userID`, `time`, `hrProfile`) VALUES ('0', STR_TO_DATE('24-03-2018 09:04:34','%d-%m-%Y %h:%m:%s'), 'blaa' );
            # INSERT INTO `workout` (`userID`, `time`, `hrProfile`) VALUES ('0', STR_TO_DATE('24-03-2018 09:04:34','%d-%m-%Y %h:%m:%s'), 'blaa' );


        # connection is not autocommit by default. So you must commit to save
        # your changes.
        connection.commit()

    finally:
        connection.close()



with open(sys.argv[1], 'rb') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    rowCount    = 0
    hrData      = []
    name        = ''
    date        = ''
    time        = ''
    for row in spamreader:

        if rowCount == 1:
            name    = row[0]
            date    = row[2]
            time    = row[3]
            #24-03-2018
            #09:04:34
            print(name)
            print(date)
            print(time)

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

    if len(hrData) > 0:
        jsonHRData  = json.dumps(hrData)
        InsertHRDataIntoDatabase(name,date,time,jsonHRData)
        print(jsonHRData)


