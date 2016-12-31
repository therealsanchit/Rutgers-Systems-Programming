all : client server 

client : libnetfiles.c libnetfiles.h
		gcc libnetfiles.c libnetfiles.h -o client

server : netfileserver.c Queue.c
		gcc netfileserver.c Queue.c -pthread -o server
