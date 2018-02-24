
FROM ubuntu:16.04

WORKDIR "/project"


RUN apt-get update -q
RUN apt-get install -y nginx vim netcat
RUN apt-get install -y software-properties-common
RUN add-apt-repository ppa:certbot/certbot
RUN apt-get update -q
RUN apt-get install -y python-certbot-nginx 



EXPOSE 80

