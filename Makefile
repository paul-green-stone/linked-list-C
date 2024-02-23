OBJDIR			:= objects
OBJECTS 		:= $(addprefix $(OBJDIR)/, List.o)

INCLUDE			:= include/sll.h

CC				:= gcc
CFLAGS 			:= -g -c

AR 				:= ar
ARFLAGS 		:= -r -c

ALL_CFLAGS 		:= -Wall -Wextra -pedantic-errors -fPIC -O2

SHARED			:= libsll

UNAME_S			:= $(shell uname -s)

INSTALL_DIR		:= /usr/local

# ================================================================ #

LIST			:= $(addprefix source/, list.c)

# ================================ #

all: $(SHARED)

$(SHARED): $(OBJECTS)
ifeq ($(UNAME_S),Linux)
	$(CC) -shared -o $@.so $^
else ifeq ($(UNAME_S),Darwin)
	$(CC) -dynamiclib -o $@.dylib $^
else ifeq ($(WINDOWS),Windows_NT)
    $(CC) -shared -o $@.dll $^
endif

# List module
$(OBJDIR)/List.o: $(LIST) $(INCLUDE)
	$(CC) $(ALL_CFLAGS) $(CFLAGS) -o $@ $<

install:
ifeq ($(UNAME_S),Linux)
	cp $(SHARED).so $(INSTALL_DIR)/lib
	cp -r ./include ./sll
	mv ./sll $(INSTALL_DIR)/include
else ifeq ($(UNAME_S),Darwin)
	cp $(SHARED).dylib $(INSTALL_DIR)/lib
	cp -r ./include ./sll
	mv ./sll $(INSTALL_DIR)/include
endif

# ================================================================ #


$(shell mkdir -p $(OBJDIR))

.PHONY: clean

clean:
	rm -rf $(OBJDIR) $(STATIC) *.so *.dylib *.dll
