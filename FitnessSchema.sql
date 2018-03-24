
drop database if exists fitness;
drop user if exists 'fitness'@'localhost';

CREATE USER 'fitness'@'localhost' IDENTIFIED BY 'fitness';

create database fitness;

GRANT ALL PRIVILEGES ON fitness.* TO 'fitness'@'localhost';



use fitness;

create table user (
    id              int NOT NULL AUTO_INCREMENT,
    name            text,
    emailAddress    text,

    PRIMARY KEY (id)
);


create table workout (
    userID      int,
    hrProfile   longtext,
    time        datetime,
);

