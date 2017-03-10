CC=g++
CC_FLAGS=-Wall -Werror -ansi -pedantic
EXEC=rshell

SRCDIR=src
BINDIR=bin
LIBDIR=$(BINDIR)/libs

SOURCES:=$(wildcard $(SRCDIR)/*.cpp) \
    $(wildcard $(SRCDIR)/tasks/*.cpp) \
    $(wildcard $(SRCDIR)/utils/*.cpp)
OBJECTS:=$(SOURCES:$(SRCDIR)/%.cpp=$(LIBDIR)/%.o)
DEPENDS:=$(OBJECTS:.o=.d)

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $(BINDIR)/$@ $^

-include $(DEPENDS)

$(LIBDIR)/%.o : $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -c $< -o $@
	$(CC) $(CC_FLAGS) $< -MM -MT $@ > $(@:.o=.d)

$(LIBDIR)/tasks/%.o : $(SRCDIR)/tasks/%.cpp
	mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -c $< -o $@
	$(CC) $(CC_FLAGS) $< -MM -MT $@ > $(@:.o=.d)

$(LIBDIR)/utils/%.o : $(SRCDIR)/utils/%.cpp
	mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -c $< -o $@
	$(CC) $(CC_FLAGS) $< -MM -MT $@ > $(@:.o=.d)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXEC)

.PHONY: cleandep
cleandep:
	rm -f $(DEPENDS)