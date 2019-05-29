delete from Devices;
delete from Programas;
delete from Logs;
delete from Salidas;

delete from AspNetUsers;

update sqlite_sequence set seq=0 where name='Devices';
update sqlite_sequence set seq=0 where name='Programas';
update sqlite_sequence set seq=0 where name='Logs';
update sqlite_sequence set seq=0 where name='Salidas';
