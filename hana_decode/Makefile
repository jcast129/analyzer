# Makefile for "hana" (C++ Hall A Analyzer) Event Decoder.
# R. Michaels, rom@jlab.org, Mar 2015
#
# Test Executables:
# tstoo    --  tests of OO decoder
# tstfadc  --  tests of FADC 250 class
# tstfadcblk   tests of FADC 250 class in multiblock mode
# tstf1tdc --  tests of F1 TDC class
# tstskel  --  test of SkeltonModule
# tstcoda  --  test of abstract interface to THaCodaFile and THaEtClient.
# tstio    --  simple tests of CODA I/O from a file.
# tdecpr   --  test of decoder with self-explanatory printouts.
# tdecex   --  test of decoder, example for a use by a detector class.
# etclient --  test of ET connection for online data.
# prfact   --  standalone code to print the prescale factors and exit.
# epicsd   --  test of EPICS data
#
# To understand how to use decoding classes, look at the 'main'
# routines tstcoda_main.C, tstio_main.C, tdecpr_main.C, tdecex_main.C etc
# corresponding to the above executables.  Also see haGenDetTest.C
# which shows proposed usage in a 'generic' detector class.

# Note regarding software from DAQ group:
# EVIO headers and libraries must be installed in $EVIO_INCDIR and
# $EVIO_LIBDIR, respectively.

# name and version of this release
VERS = hana_decode_2.0

# If one defines STANDALONE, the library and test execs are compiled
# independently of the rest of the analyzer.  Normally this is commented out.
# export STANDALONE = 1

# MAINDIR is the source of headers for main code.
# Also need objects from MAINDIR for standalone test executables,
# but NOT for making library.
export MAINDIR = ../src

# Use flag ONLINE_ET below if compiling standalone online code (ET system).
# This flag gets passed by the main Makefile if not compiling standalone tests.
# CODA environment variable must be set.  Examples are
#   CODA = /adaqfs/coda/2.2        (in adaq cluster)
#   CODA = /data7/user/coda/2.2    (on haplix cluster)
# User's $LD_LIBRARY_PATH must include $CODA/$OSNAME/lib  (OSNAME = SunOS or Linux)
# Example:  OSNAME=Linux ; export OSNAME
# LD_LIBRARY_PATH=/app/root/lib:$CODA/$OSNAME/lib:.   ; export LD_LIBRARY_PATH
# export ONLINE_ET = 1

# Use this if profiling (note: it slows down the code)
# export PROFILE = 1

# export DEBUG = 1


ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTBIN      := $(shell root-config --bindir)
ROOTINC      := -I$(shell root-config --incdir)
CXX          := $(shell root-config --cxx)
CC           := $(shell root-config --cc)

INCLUDES =

ifndef STANDALONE
  INCLUDES += -I$(MAINDIR)
else
  MACHINE := $(shell uname -s)
  ARCH    := linux
  SOSUF   := so
  ifeq ($(MACHINE),Darwin)
     ARCH := macosx
     SOSUF := dylib
  endif

# Only define compiler options for STANDALONE, otherwise inherit them from
# top-level Makefile

  DEFINES = -DSTANDALONE
  ifeq ($(ARCH),linux)
     DEFINES      += -DLINUXVERS
     # Linux with egcs (>= RedHat 5.2)
     LD            = $(CXX)
     SOFLAGS       = -shared
     ifdef DEBUG
	CXXFLG      = -g -O0
	LDFLAGS     = -g -O0
     else
	CXXFLG      = -O2
	LDFLAGS     = -O2
	DEFINES    += -DNDEBUG
     endif
     CXXFLG        += -Wall -fPIC
     CXXEXTFLG      =
     DEFINES      += $(shell getconf LFS_CFLAGS)
  endif

  ifeq ($(ARCH),macosx)
     # EXPERIMENTAL: Mac OS X with Xcode 3.x/gcc 4.x
     DEFINES      += -DMACVERS
     LD            = $(CXX)
     SOFLAGS       = -shared -Wl,-undefined,dynamic_lookup
     SONAME        = -Wl,-install_name,@rpath/
     ifdef DEBUG
	CXXFLG      = -g -O0
	LDFLAGS     = -g -O0
     else
	CXXFLG      = -O2
	LDFLAGS     = -O2
	DEFINES    += -DNDEBUG
     endif
     CXXFLG        += -Wall -fPIC
     CXXEXTFLG      =
  endif

  ifdef WITH_DEBUG
     DEFINES     += -DWITH_DEBUG
  endif
endif

export OSNAME := $(shell uname)

ifeq ($(OSNAME),Linux)
   LIBS          =
   GLIBS         = -L/usr/X11R6/lib -lXpm -lX11
   ALL_LIBS      = $(ROOTLIBS)

# ONLIBS is needed for ET
   ET_AC_FLAGS = -D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS
   ET_CFLAGS = -02 -fPIC -I. $(ET_AC_FLAGS) -DLINUXVERS
# CODA environment variable must be set.  Examples are
#   CODA = /adaqfs/coda/2.2        (in adaq cluster)
#   CODA = /data7/user/coda/2.2    (on haplix cluster)
   LIBET = $(CODA)/Linux/lib/libet.$(SOSUF)
   ONLIBS = $(LIBET) -lieee -lpthread -ldl -lresolv

   ifdef PROFILE
      CXXFLG   += -pg
   endif

endif

ifeq ($(OSNAME),Darwin)
  ALL_LIBS = $(ROOTLIBS)
endif

# ... finished, what ET needs.
# ifdef ONLINE_ET
#   DC_DEFINES += -DONLINE_ET
#   ALL_LIBS   += $(ONLIBS)
# endif

# External EVIO support
EVIO_LIB  = -L$(EVIO_LIBDIR) -levio
INCLUDES += -I$(EVIO_INCDIR)

DEFINES      += $(DC_DEFINES)
CFLAGS        = $(CXXFLG) $(ROOTCFLAGS) $(INCLUDES) $(DEFINES)
CXXFLAGS      = $(CXXFLG) $(CXXEXTFLG) $(ROOTCFLAGS) $(INCLUDES) $(DEFINES)
LIBS         += $(ROOTLIBS) $(SYSLIBS)
GLIBS        += $(ROOTGLIBS) $(SYSLIBS)

#-----------------------------------------------------------------------
# DECODE_OBJS is the package for CODA I/O and decoder written by R. Michaels.
# This, together with libevio, is what other developers need.

SRC = THaUsrstrutils.C THaCrateMap.C THaCodaData.C \
      THaEpics.C THaFastBusWord.C THaCodaFile.C THaSlotData.C \
      THaEvData.C THaCodaDecoder.C \
      CodaDecoder.C Module.C VmeModule.C PipeliningModule.C FastbusModule.C  \
      Lecroy1877Module.C Lecroy1881Module.C Lecroy1875Module.C \
      Fadc250Module.C GenScaler.C Scaler560.C Scaler1151.C \
      Scaler3800.C Scaler3801.C F1TDCModule.C Caen1190Module.C \
      Caen775Module.C Caen792Module.C

ifndef STANDALONE
  SRC += SimDecoder.C
endif

PROGS = tstoo tstfadc tstfadcblk tstfadcblk tstf1tdc tst1190 tstio tdecpr tdecex prfact epicsd
# If you want to use the ET system at Jlab.
# ifdef ONLINE_ET
#   SRC += THaEtClient.C
#   PROGS += etclient
#   PROGS += tstcoda
# endif

RCHEAD := $(SRC:.C=.h) THaBenchmark.h
HEAD = $(RCHEAD) Decoder.h
# ifdef ONLINE_ET
#   HEAD += et.h
# endif
DEPS = $(patsubst %.c,%.d,$(SRC:.C=.d))
DECODE_OBJ = $(patsubst %.c,%.o,$(SRC:%.C=%.o)) THaDecDict.o

ifndef STANDALONE
  all:  $(LIBDIR)/libdc.$(SOSUF).$(VERSION)
else
  all: $(PROGS) libdc.a
endif

# Library for main analyzer

libdc.a: $(DECODE_OBJ)
	rm -f $@
	ar crs $@ $(filter %.o,$^)
ifndef STANDALONE
	cp -p $@ $(LIBDIR)
endif

static:	libdc.a

libdc.$(SOSUF): $(DECODE_OBJ)
	rm -f $@
ifeq ($(strip $(SONAME)),)
	$(LD) $(LDFLAGS) $(SOFLAGS) -o $@ $(filter %.o,$^)
else
	$(LD) $(LDFLAGS) $(SOFLAGS) $(SONAME)libdc.$(SOSUF).$(SOVERSION) -o $@ $(filter %.o,$^)
endif

ifndef STANDALONE
$(LIBDIR)/libdc.$(SOSUF).$(VERSION): libdc.$(SOSUF)
	rm -f $@ $(LIBDIR)/libdc.$(SOSUF).$(SOVERSION)
	cp -p $< $@
endif

# Test programs for standalone tests of decoder.
ifdef STANDALONE
  DECODE_LIB = libdc.a
  ALL_LIBS += $(EVIO_LIB)
endif

ifdef STANDALONE
tstoo: tstoo_main.o $(DECODE_LIB)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ $< $(DECODE_LIB) $(ALL_LIBS)

tstfadc: tstfadc_main.o $(DECODE_LIB)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ tstfadc_main.o $(DECODE_LIB) $(ALL_LIBS)

tstfadcblk: tstfadcblk_main.o $(DECODE_LIB)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ tstfadcblk_main.o $(DECODE_LIB) $(ALL_LIBS)

tstf1tdc: tstf1tdc_main.o $(DECODE_LIB)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ tstf1tdc_main.o $(DECODE_LIB) $(ALL_LIBS)

tst1190: tst1190_main.o $(DECODE_LIB)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ tst1190_main.o $(DECODE_LIB) $(ALL_LIBS)

tstio: tstio_main.o $(DECODE_LIB)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ tstio_main.o $(DECODE_LIB) $(ALL_LIBS)

tdecpr: tdecpr_main.o $(DECODE_LIB)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ tdecpr_main.o $(DECODE_LIB) $(ALL_LIBS)

tdecex: tdecex_main.o THaGenDetTest.o $(DECODE_LIB)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ tdecex_main.o THaGenDetTest.o $(DECODE_LIB) $(ALL_LIBS)

etclient: etclient_main.o $(DECODE_LIB)
	$(CXX) $(LDFLAGS) -o $@ etclient_main.o $(DECODE_LIB) $(ALL_LIBS)

prfact: prfact_main.o $(DECODE_LIB)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ prfact_main.o $(DECODE_LIB) $(ALL_LIBS)

epicsd: epics_main.o $(DECODE_LIB)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ epics_main.o $(DECODE_LIB) $(ALL_LIBS)

tstcoda: tstcoda_main.o $(DECODE_LIB)
	$(CXX) $(LDFLAGS) -o $@ tstcoda_main.o $(DECODE_LIB) $(ALL_LIBS)
endif

# Dictionary

THaDecDict.C: $(RCHEAD) haDecode_LinkDef.h
	@echo "Generating Decoder Dictionary..."
	$(ROOTBIN)/rootcint -f THaDecDict.C -c $(ROOTINC) $(INCLUDES) $(DEFINES) $^
ifndef STANDALONE
	if [ -r THaDecDict_rdict.pcm ]; then cp -p THaDecDict_rdict.pcm  $(LIBDIR); fi
endif

# create a tar file of ./$(VERS)/* (all code)

tarfile: clean version
	tar cvf $(VERS).tar ./hana_decode

version:
	mkdir $(VERS)
	cp *.c ./$(VERS)
	cp *.C ./$(VERS)
	cp *.h ./$(VERS)
	cp Makefile ./$(VERS)
	cp snippet.dat ./$(VERS)
	cp README* ./$(VERS)
	cp ChangeLog ./$(VERS)
	tar cvf $(VERS).tar ./$(VERS)

clean:
	rm -f *.{a,out,o,os} *.$(SOSUF).* core *~ THaDecDict* $(VERS).tar $(PROGS)

realclean:  clean
	rm -f *.d

install:	all
	mkdir -p $(ANALYZER)/src/hana_decode $(ANALYZER)/include
	cp -pu $(SRC) $(HEAD) haDecode_LinkDef.h Makefile $(ANALYZER)/src/hana_decode
	cp -pu $(HEAD) $(ANALYZER)/include

.PHONY: all static evio tarfile version clean realclean install

.SUFFIXES:

%.o:	%.C
ifeq ($(strip $(MAKEDEPEND)),)
	$(CXX) $(CXXFLAGS) -MMD -o $@ -c $<
	@mv -f $*.d $*.d.tmp
else
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	$(MAKEDEPEND) $(ROOTINC) $(INCLUDES) $(DEFINES) -c $< > $*.d.tmp
endif
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

-include $(DEPS)
