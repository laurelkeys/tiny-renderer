SYSCONF_LINK = g++
CPPFLAGS     = -g -MD -Wall -Wextra -Wcast-align -Wno-unused-parameter -std=c++17
LDFLAGS      = -Wall
LIBS         = -lm

DESTDIR = ./
TARGET  = main

OBJECTS := $(patsubst %.cc,%.o,$(wildcard *.cc))
DEPS=$(OBJECTS:.o=.d)

all: $(DESTDIR)$(TARGET)

$(DESTDIR)$(TARGET): $(OBJECTS)
	$(SYSCONF_LINK) $(LDFLAGS) -o $(DESTDIR)$(TARGET) $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cc
	$(SYSCONF_LINK) $(CPPFLAGS) -c $< -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
	-rm -f *.tga
	-rm -f $(DEPS)

-include $(DEPS)