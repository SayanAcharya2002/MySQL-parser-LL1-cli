create table users (
    id varchar(2),
    name varchar(2),
    primary key ( name )
);


select * from users
where
id = 5
and
name in ("ABC","DEF") or
name in ("ABC","DEF","FGH")
or name like "___ABC";

insert into star (a,b)
values
("A","B","C");


insert into users (name,password)
values
("ABC","CDE");

select 
* from students where a = "b" or b = "c" and c > 5;

create table names(
    name varchar(2),
    password varchar(2),
    primary key ( name )
);


select name,password from users
where
password > 5 or name < 6 or
name in ("Aditya","Adnan");

insert into users
values
("ABC",5,6,6);

select name,password FRoM users
where name = "Aditya" and id = 5 or pwd = "abc";

create table allname(
    first varchar(2),
    second varchar(2),
    third int,
    primary key ( first )
);

select name,password,a from users
where name = "Aditya" or password = "C"
and a = "a";