#!/bin/bash

rm xml2pdu.jar
rm pdu2xml.jar

pushd gurux.dlms.java/development
mvn clean
mvn compile
mvn package
popd
mvn install:install-file -Dfile=./gurux.dlms.java/development/target/gurux.dlms-2.1.11.jar  -DpomFile=gurux.dlms.java/development/pom.xml


pushd gurux.dlms.java/pdu2xml
mvn clean
mvn package
popd
mv ./gurux.dlms.java/pdu2xml/target/pdu2xml-0.0.1-SNAPSHOT.jar pdu2xml.jar

pushd gurux.dlms.java/xml2pdu
mvn clean
mvn package
popd
mv ./gurux.dlms.java/xml2pdu/target/xml2pdu-0.0.1-SNAPSHOT.jar xml2pdu.jar


java -jar ./pdu2xml.jar c4018100090c303030303030303130303031
java -jar ./xml2pdu.jar Test.xml

