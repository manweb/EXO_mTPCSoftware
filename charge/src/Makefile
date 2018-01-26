# Begin of declarations ************************************************

# ---- Install directory -----------------------------------------------
INSTALLDIR = $(READMIDASSYS)

# ---- Define direcrories ----------------------------------------------
BUILDDIR = $(INSTALLDIR)/build/
LIBDIR = $(INSTALLDIR)/lib/
BINDIR = $(INSTALLDIR)/bin/

# ---- Compiler --------------------------------------------------------
CXX = g++

# ---- Compiler flags --------------------------------------------------
CXXFLAGS = -g -O2 -Wall -fPIC
SOFLAGS  = -O2 -shared -fPIC

# ---- ROOT ------------------------------------------------------------
ROOTLIBS  = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --libs)  -lXMLParser -lThread
ROOTGLIBS = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --glibs) -lXMLParser -lThread -lEve -lSpectrum
RPATH    += -Wl,-rpath,$(ROOTSYS)/lib
CXXFLAGS += -DHAVE_ROOT $(shell $(ROOTSYS)/bin/root-config --cflags)
LDFLAGS   = $(ROOTLDFLAGS)

# ---- Midas -----------------------------------------------------------
MIDASLIBS = $(MIDASSYS)/lib/libmidas.a -lutil
CXXFLAGS += -DHAVE_MIDAS -DOS_LINUX -Dextname -I$(MIDASSYS)/include

# ---- Optional TNetDirectory code -------------------------------------
CXXFLAGS += -DHAVE_LIBNETDIRECTORY

# ---- ROOTANA ---------------------------------------------------------
LIB_ROOTANA = $(ROOTANASYS)
INC_ROOTANA = $(ROOTANASYS)

# ---- Define sources --------------------------------------------------
SOURCES = Calibration.cxx EventDisplay.cxx ReadMidasExample.cxx mid2root.cxx dEdx.cxx
TARGETS = $(patsubst %.cxx, $(BINDIR)%, $(SOURCES))

# ---- Sources for libraries -------------------------------------------
DATASO = $(BUILDDIR)Data.o $(BUILDDIR)DataDict.o
EVTDISPSO = $(BUILDDIR)EventDisplay.o $(BUILDDIR)EventDisplayDict.o

# End of declarations **************************************************

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all: $(TARGETS)

$(LIBDIR)libData.so: $(DATASO)
	@echo "Creating library $@..."
	$(CXX) $(SOFLAGS) $(ROOTLDFLAGS) $^ -o $@ $(EXPLLINKLIBS)

$(LIBDIR)libEventDisplay.so: $(EVTDISPSO)
	@echo "Creating library $@..."
	$(CXX) $(SOFLAGS) $(ROOTLDFLAGS) $^ -o $@ $(EXPLLINKLIBS)

$(BINDIR)Calibration: $(LIBDIR)libData.so $(BUILDDIR)Calibration.o $(ROOTANASYS)/librootana.a
	@echo "Linking $@..."
	$(CXX) -o $@ $(CXXFLAGS) $^ $(MIDASLIBS) $(ROOTGLIBS) -lm -lz -lpthread $(RPATH)

$(BINDIR)EventDisplay: $(LIBDIR)libEventDisplay.so $(LIBDIR)libData.so $(BUILDDIR)EventDisplay.o $(ROOTANASYS)/librootana.a
	@echo "Linking $@..."
	$(CXX) -o $@ $(CXXFLAGS) $^ $(MIDASLIBS) $(ROOTGLIBS) -lm -lz -lpthread $(RPATH)

$(BINDIR)mid2root: $(LIBDIR)libData.so $(BUILDDIR)mid2root.o $(ROOTANASYS)/librootana.a
	@echo "Linking $@..."
	$(CXX) -o $@ $(CXXFLAGS) $^ $(MIDASLIBS) $(ROOTGLIBS) -lm -lz -lpthread $(RPATH)

$(BINDIR)ReadMidasExample: $(LIBDIR)libData.so $(BUILDDIR)ReadMidasExample.o $(ROOTANASYS)/librootana.a
	@echo "Linking $@..."
	$(CXX) -o $@ $(CXXFLAGS) $^ $(MIDASLIBS) $(ROOTGLIBS) -lm -lz -lpthread $(RPATH)

$(BINDIR)dEdx: $(LIBDIR)libData.so $(BUILDDIR)dEdx.o $(ROOTANASYS)/librootana.a
	@echo "Linking $@..."
	$(CXX) -o $@ $(CXXFLAGS) $^ $(MIDASLIBS) $(ROOTGLIBS) -lm -lz -lpthread $(RPATH)

clean:
	@echo "Cleaning up..."
	@rm -f core $(PROGRAMS) $(DATASO) $(EVTDISPSO) *Dict.* $(BUILDDIR)*.o $(LIBDIR)*.so *.o $(BINDIR)* *.gch
	@echo "...done!"

.SUFFIXES: .cxx

Data.o: Data.h
mid2root.o: Data.h
EventDisplay.o: Data.h
dEdx.o: dEdx.h

DataDict.cxx: Data.h DataLinkDef.h
	@echo "Generating dictionary $@..."
	rootcint -f $@ -c $^

EventDisplayDict.cxx: EventDisplay.h EventDisplayLinkDef.h
	@echo "Generating dictionary $@..."
	rootcint -f $@ -c $^

$(BUILDDIR)%.o: %.cxx
	$(CXX)  $(CXXFLAGS) -I$(INC_ROOTANA) -L$(LIB_ROOTANA) -c $^ -o $@
