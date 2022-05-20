CC = gcc
CFLAGS = 
LDFLAGS = 

INCLUDEDIR = ./include
INCLUDES = $(addprefix -I, $(INCLUDEDIR))

SOURCEDIR = ./source
SOURCES := $(shell find $(SOURCEDIR) -name "*\.c")

OBJDIR = ./objs
OBJS := $(subst $(SOURCEDIR), $(OBJDIR), $(SOURCES:.c=.o))

TARGET = a.exe

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)
	
$(OBJDIR)/%.o: $(SOURCEDIR)/%.c $(OBJDIR)
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

$(OBJDIR)/:
	mkdir $@
	
.PHONY: clean

clean:
	rm -rf $(OBJDIR) $(TARGET)