# program executable name
TARGET = ass_a
# compiler flags
CFLAGS = -g -Wall
# linker flags
LDFLAGS = -lcomedi -pthread -lrt -lm
# list of sources
SOURCES = $(shell find -name "*.c")
# default rule, to compile everything
all: $(TARGET)
# define object files
OBJECTS = $(SOURCES:.c=.o)
# link programs
$(TARGET): $(OBJECTS)
	gcc $(LDFLAGS) -o $@ $^
# compile
%.o : %.c
	gcc $(CFLAGS) -o $@ $<
# cleaning
clean:
	rm -f $(TARGET) $(OBJECTS)
