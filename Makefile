.SUFFIXES : .c .o

CC := gcc

TARGET := codeline
OBJECTS := codeline.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	  $(CC) -o $@ $(OBJECTS)

clean:
	  rm -rf $(OBJECTS) $(TARGET) core
