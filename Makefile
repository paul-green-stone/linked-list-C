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

# ================================================================ #

LIST			:= $(addprefix source/, list.c)

# ================================ #

all: $(SHARED)

$(SHARED): $(OBJECTS)
ifeq ($(UNAME_S),Linux)
	$(CC) -shared -o $@.so $^ $(LDFLAGS)
endif
ifeq ($(UNAME_S),Darwin)
	$(CC) -dynamiclib -o $@.dylib $^ $(LDFLAGS)
endif
ifeq ($(WINDOWS),msys)
    $(CC) --shared -o $@ $^ $(LDFLAGS)
endif

# List module
$(OBJDIR)/List.o: $(LIST) $(INCLUDE)
	$(CC) $(ALL_CFLAGS) $(CFLAGS) -o $@ $<

# ================================================================ #


$(shell mkdir -p $(OBJDIR))

.PHONY: clean

clean:
	rm -rf $(OBJDIR) $(STATIC) *.so *.dylib
