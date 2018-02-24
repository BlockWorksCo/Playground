
FROM ubuntu:16.04

WORKDIR "/project"

ADD letsencrypt/ /etc/letsencrypt/
ADD default /etc/nginx/sites-available/default


RUN apt-get update -q
RUN apt-get install -y vim
RUN apt-get install -q -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" nginx
RUN apt-get install -y software-properties-common
RUN add-apt-repository -y ppa:certbot/certbot
RUN apt-get update -q
RUN apt-get install -y python-certbot-nginx 



EXPOSE 443

