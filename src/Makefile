SRC=hashtable.c queue.c server.c worker.c package.c
BASE=$(basename $(SRC))
OBJ=$(addsuffix .o,$(BASE))

TAR=$(TAR1) $(TAR2)

TAR1:=libshooter.a
TAR2:=libshooter.so
CC=gcc
AR=ar
FLG=-DDEBUG

TAR:$(TAR1) $(TAR2)
	@echo compile $(TAR1) $(TAR2) finished

$(TAR1):$(OBJ)
	$(AR) -r $(TAR1) $^

$(TAR2):$(OBJ)
	$(CC) $(FLG) -shared -fPIC -o $(TAR2) $^ -ljemalloc

$(OBJ):$(SRC)
	$(CC) $(FLG) -fPIC -c -g $(SRC)
clean:
	--rm $(OBJ) $(TAR)

