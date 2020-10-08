cc= gcc
cflags= -Wall -O2 -g

src= ${wildcard *.c}
targets= ${patsubst %.c, %.o, $(src)}

APP= cli

$(APP): $(targets)
	$(cc) $^ -o $@ $(cflags)

.PHONY: clean all
clean:
	-rm -r $(targets)
