
FROM ubuntu:16.04

WORKDIR "/project"


RUN apt-get update -q
RUN apt-get install -y vim
RUN apt-get install -y software-properties-common
RUN add-apt-repository -y ppa:certbot/certbot
RUN apt-get update -q
RUN apt-get install -y bzip2 wget
RUN apt-get install -y dbus
RUN apt-get install -y openssh-server

RUN echo mail > /etc/hostname

RUN apt-get install -y debconf-utils
RUN echo "mysql-server mysql-server/root_password password 1234567890" | debconf-set-selections
RUN echo "mysql-server mysql-server/root_password_again password 1234567890" | debconf-set-selections
RUN DEBIAN_FRONTEND=noninteractive apt-get -yq install mysql-server
RUN mkdir -p /var/run/mysqld
RUN chown mysql /var/run/mysqld

ADD Install Install
ADD hosts hosts
ADD hostname /bin/hostname
RUN cp /project/hosts /etc/hosts && echo mail.blockworks.co > /etc/hostname && cat /etc/hosts && ./Install



EXPOSE 25
EXPOSE 587
EXPOSE 110
EXPOSE 995
EXPOSE 993
EXPOSE 143
EXPOSE 389
EXPOSE 636
EXPOSE 24242



#CMD service nginx start && tail -F /var/log/nginx/error.log

