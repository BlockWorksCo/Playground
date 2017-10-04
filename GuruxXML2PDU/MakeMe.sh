#!/bin/bash


pushd gurux.dlms.java/development
mvn clean
mvn compile
popd

pushd gurux.dlms.java/pdu2xml
mvn clean
mvn package
popd

mv gurux.dlms.java/pdu2xml/target/gurux.dlms.client.example.java-0.0.1-SNAPSHOT.jar pdu2xml.jar
java -jar pdu2xml.jar c4018100090c303030303030303130303031
