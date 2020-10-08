cc= gcc
cflags= -Wall -O2 -g -lsqlite3

src= ${wildcard *.c}
targets= ${patsubst %.c, %.o, $(src)}

APP= ser

$(APP): $(targets)
	$(cc) $^ -o $@ $(cflags)

.PHONY: clean all
clean:
	-rm -r $(targets)
