PRG    = route_add
SRC    = main.c libnetlink.c

CFLAGS = -Wall
LFLAGS = -lm
CC     =  gcc $(CFLAGS)

$(PRG) : $(SRC)
	$(CC) -o $@ $^ $(LFLAGS)

clean : 
	rm -f $(PRG)