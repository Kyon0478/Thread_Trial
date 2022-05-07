TAR = a.out
OBJ = 02_multithread.c
CC:=gcc

$(TAR):$(OBJ)
	$(CC) $(OBJ) -o $(TAR) -lpthread 

.PHONY
clean:
	rm -rf $(TAR)
