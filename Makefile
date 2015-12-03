# dependendences
TCL_INCLUDE_DIR = /usr/include/tcl8.5
INCLUDE_DIR += -I$(TCL_INCLUDE_DIR)

# compiler settings
CPPFLAGS += -O3 -Wall $(INCLUDE_DIR)
STATICLIBFLAGS = rcs

# library settings
LIBNAME = phlib
STATICLIB = lib$(LIBNAME).a
OBJDIR = obj
DISTDIR = dist
SRC_DIR = src/phlib
SRCS = $(addprefix $(SRC_DIR)/, cmdline.cpp floatmatrix.cpp floatvector.cpp tclutils.cpp tracereader.cpp xmlparser.cpp xmlstream.cpp)
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))

# targets
all: static

static: $(DISTDIR)/$(STATICLIB)

$(OBJS): $(HDRS) $(DISTDIR) $(OBJDIR)

$(DISTDIR)/$(STATICLIB): $(OBJS)
	$(AR) $(STATICLIBFLAGS) $(DISTDIR)/$(STATICLIB) $(OBJS)

$(OBJDIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CPPFLAGS) -o $@ $<

$(OBJDIR):
	mkdir -p $@

$(DISTDIR):
	mkdir -p $@

clean:
	$(RM) -r $(OBJDIR) $(DISTDIR)

