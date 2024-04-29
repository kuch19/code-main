#Declaration des variables
CC= gcc
CFLAGS= -Wall

#Commandes

all: generateur 

###

generateur: generateur.c
	$(CC) $(CFLAGS) $< -lm -o $@


#Nettoyage
clean: 
	rm -f *.o
	rm -f *.svg
	rm -f *~
	rm generateur 
