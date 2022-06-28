CC = gcc
CFLAGS = -I../include -I.
LDFLAGS =
TARGET = a.exe
SOURCEDIR = ../source
OBJDIR = .
SOURCE = except.c assert.c mem.c fmt.c text.c
SOURCES = $(addprefix $(SOURCEDIR)/, $(SOURCE)) test_text.c
OBJS = $(subst $(SOURCEDIR), $(OBJDIR), $(SOURCES:.c=.o))

$(TARGET): $(OBJS)
	CC -o $(TARGET) $(OBJS) $(LDFLAGS)
	
$(OBJDIR)/%.o: $(SOURCEDIR)/%.c
	CC -c $< -o $@ $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(OBJS) $(TARGET)