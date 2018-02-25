
FROM ubuntu:16.04

WORKDIR "/project"


RUN apt-get update -q
RUN apt-get install -y vim
RUN apt-get install -y software-properties-common
RUN add-apt-repository -y ppa:certbot/certbot
RUN apt-get update -q
RUN apt-get install -y bzip2 wget

# https://askubuntu.com/questions/79257/how-do-i-install-mysql-without-a-password-prompt
RUN apt-get install -y debconf-utils
RUN echo "mysql-server mysql-server/root_password password 1234567890" | debconf-set-selections
RUN echo "mysql-server mysql-server/root_password_again password 1234567890" | debconf-set-selections
RUN DEBIAN_FRONTEND=noninteractive apt-get -yq install mysql-server
RUN mkdir -p /var/run/mysqld
RUN chown mysql /var/run/mysqld

ADD Install Install
ADD hosts hosts
#RUN cp hosts /etc/hosts
RUN ./Install



EXPOSE 443


#CMD service nginx start && tail -F /var/log/nginx/error.log

