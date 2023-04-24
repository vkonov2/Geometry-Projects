
# Makefile settings
APPNAME = Ellipse.out
EXT = .cpp

# Addition Packages
SRC_PACKAGES = BOOST EIGEN GSL

# BOOST library
ROOT_BOOST = ../../Libs/boost_1_79_0

# Linear-algebra library
ROOT_EIGEN = ../../Libs/eigen-3.4.0/Eigen

# GNU Scientific library
ROOT_GSL = ~

EXTRA_INCLUDE_DIR = $(foreach loop, $(SRC_PACKAGES), -I$(ROOT_$(loop)))

SRCDIR = src
GMTDIR = ../Src/Geometry
DRWDIR = ../Src/Draw
CRVDIR = ../Src/CurveGenerator
EIGDIR = ../Src/Eigen
DSRDIR = ../Src/Distribution
UTLDIR = ../Src

HDRDIR = src/hdr

OBJDIRMAIN = obj/main
OBJDIRGMT = obj/gmt
OBJDIRDRW = obj/drw
OBJDIRCRV = obj/crv
OBJDIREIG = obj/eig
OBJDIRUTL = obj/utl
OBJDIRDSR = obj/dsr

DEP = dep

SRCMAIN = $(wildcard $(SRCDIR)/*$(EXT))
SRCGMT = $(filter-out $(GMTDIR)/Polyhedr$(EXT),$(wildcard $(GMTDIR)/*$(EXT)))
SRCDRW = $(filter-out $(DRWDIR)/DrawPolyhedron$(EXT),$(wildcard $(DRWDIR)/*$(EXT)))
SRCCRV = $(wildcard $(CRVDIR)/*$(EXT))
SRCEIG = $(wildcard $(EIGDIR)/*$(EXT))
SRCUTL = $(wildcard $(UTLDIR)/*$(EXT))
SRCDSR = $(wildcard $(DSRDIR)/*$(EXT))

HDRMAIN = $(wildcard $(HDRDIR)/*.h)
HDRGMT = $(wildcard $(GMTDIR)/*.h)
HDRDRW = $(wildcard $(DRWDIR)/*.h)
HDRCRV = $(wildcard $(CRVDIR)/*.h)
HDREIG = $(wildcard $(EIGDIR)/*.h)
HDRUTL = $(wildcard $(UTLDIR)/*.h)
HDRDSR = $(wildcard $(DSRDIR)/*.h)

OBJMAIN = $(SRCMAIN:$(SRCDIR)/%$(EXT)=$(OBJDIRMAIN)/%.o)
OBJGMT = $(SRCGMT:$(GMTDIR)/%$(EXT)=$(OBJDIRGMT)/%.o)
OBJDRW = $(SRCDRW:$(DRWDIR)/%$(EXT)=$(OBJDIRDRW)/%.o)
OBJCRV = $(SRCCRV:$(CRVDIR)/%$(EXT)=$(OBJDIRCRV)/%.o)
OBJEIG = $(SRCEIG:$(EIGDIR)/%$(EXT)=$(OBJDIREIG)/%.o)
OBJUTL = $(SRCUTL:$(UTLDIR)/%$(EXT)=$(OBJDIRUTL)/%.o)
OBJDSR = $(SRCDSR:$(DSRDIR)/%$(EXT)=$(OBJDIRDSR)/%.o)

DEPMAIN = $(OBJMAIN:$(OBJDIRMAIN)/%.o=$(DEP)/%.d)
DEPGMT = $(OBJGMT:$(OBJDIRGMT)/%.o=$(DEP)/%.d)
DEPDRW = $(OBJDRW:$(OBJDIRDRW)/%.o=$(DEP)/%.d)
DEPCRV = $(OBJCRV:$(OBJDIRCRV)/%.o=$(DEP)/%.d)
DEPEIG = $(OBJEIG:$(OBJDIREIG)/%.o=$(DEP)/%.d)
DEPUTL = $(OBJUTL:$(OBJDIRUTL)/%.o=$(DEP)/%.d)
DEPDSR = $(OBJDSR:$(OBJDIRDSR)/%.o=$(DEP)/%.d)

RM = rm
DELOBJ = $(OBJMAIN) $(OBJGMT) $(OBJDRW) $(OBJCRV) $(OBJEIG) $(OBJUTL) $(OBJDSR)

# Compiler settings
CXX = clang++
CC = $(CXX)
CXXFLAGS = -std=c++2a
# CXXFLAGS = -std=c++2a -Wall -Wextra -Wcast-align -Wfloat-equal -Wformat=2 -Wundef -Wno-missing-field-initializers -Wno-old-style-cast -Wno-sign-compare -Wno-sign-conversion -Wcast-align -Wwrite-strings -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-write-strings -Wreturn-type -Werror=return-type -Werror=uninitialized -Wunreachable-code -Wno-keyword-macro -Wno-reserved-id-macro -Wno-unused-const-variable -pedantic -g
CPPFLAGS = -Wno-unused-value -Isrc 
DEPFLAGS = -MM -MT
LDFLAGS = -lgsl -lcblas -lm
MAKEFLAGS = --jobs=2
GNUMAKEFLAGS = --output-sync

all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJMAIN) $(OBJGMT) $(OBJDRW) $(OBJCRV) $(OBJEIG) $(OBJUTL) $(OBJDSR)
	clear
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(OBJMAIN) $(OBJGMT) $(OBJDRW) $(OBJCRV) $(OBJEIG) $(OBJUTL) $(OBJDSR) -o $(APPNAME)


# Creates the dependecy rules
$(DEP)/%.d: $(SRCDIR)/%$(EXT)
	@$(CC) $(CFLAGS) $< $(DEPFLAGS) $(DEP:/%.d=$(OBJDIRMAIN)/%.o) >$@

$(DEP)/%.d: $(GMTDIR)/%$(EXT)
	@$(CC) $(CFLAGS) $< $(DEPFLAGS) $(DEP:/%.d=$(OBJDIRGMT)/%.o) >$@

$(DEP)/%.d: $(DRWDIR)/%$(EXT)
	@$(CC) $(CFLAGS) $< $(DEPFLAGS) $(DEP:/%.d=$(OBJDIRDRW)/%.o) >$@

$(DEP)/%.d: $(CRVDIR)/%$(EXT)
	@$(CC) $(CFLAGS) $< $(DEPFLAGS) $(DEP:/%.d=$(OBJDIRCRV)/%.o) >$@

$(DEP)/%.d: $(EIGDIR)/%$(EXT)
	@$(CC) $(CFLAGS) $< $(DEPFLAGS) $(DEP:/%.d=$(OBJDIREIG)/%.o) >$@

$(DEP)/%.d: $(UTLDIR)/%$(EXT)
	@$(CC) $(CFLAGS) $< $(DEPFLAGS) $(DEP:/%.d=$(OBJDIRUTL)/%.o) >$@

$(DEP)/%.d: $(DSRDIR)/%$(EXT)
	@$(CC) $(CFLAGS) $< $(DEPFLAGS) $(DEP:/%.d=$(OBJDIRDSR)/%.o) >$@

# Includes all .h files
# -include $(HDRMAIN)
# $(GMTDIR) $(DRWDIR) $(CRVDIR) $(EIGDIR) $(DSRDIR) $(UTLDIR)

$(OBJDIRMAIN)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<  

$(OBJDIRGMT)/%.o: $(GMTDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $< 

$(OBJDIRDRW)/%.o: $(DRWDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $< 

$(OBJDIRCRV)/%.o: $(CRVDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<  

$(OBJDIREIG)/%.o: $(EIGDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<  

$(OBJDIRUTL)/%.o: $(UTLDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<  

$(OBJDIRDSR)/%.o: $(DSRDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<  

# make print-VAR to terminal
print-%  : ; @echo $* = $($*)

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) -f $(DEPMAIN) $(DEPGMT) $(DEPDRW) $(DEPCRV) $(DEPEIG) $(DEPUTL) $(DEPDSR)

# Cleans complete project
.PHONY: clean
clean:
	$(RM) -f $(DELOBJ) $(DEPMAIN) $(DEPGMT) $(DEPDRW) $(DEPCRV) $(DEPEIG) $(DEPUTL) $(DEPDSR) $(APPNAME)