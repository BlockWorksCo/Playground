#!/bin/bash


pushd gurux.dlms.java/development
mvn clean
mvn compile
popd

pushd gurux.dlms.java/gurux.dlms.client.example.java
mvn clean
mvn package
popd

java -jar gurux.dlms.java/gurux.dlms.client.example.java/target/gurux.dlms.client.example.java-0.0.1-SNAPSHOT.jar
