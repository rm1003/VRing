CC = gcc
CFLAGS = -g -I../../includes \
         -Wno-implicit-function-declaration \
         -Wno-implicit-int \
         -Wno-old-style-definition
LDFLAGS = -lm
RM = rm -f

SRC_DIR  = ../../src
OBJ_COMUM = $(SRC_DIR)/smpl.o $(SRC_DIR)/rand.o

TASKS = task0 task1 task2 task3 task4

all: $(TASKS)

$(TASKS): %: %.o $(OBJ_COMUM)
	$(CC) -o $@ $^ $(LDFLAGS) 

%.o: %.c ../../includes/smpl.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/smpl.o: $(SRC_DIR)/smpl.c ../../includes/smpl.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/rand.o: $(SRC_DIR)/rand.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(TASKS) *.o $(OBJ_COMUM) relat saida

