ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs) -lTMVA

CXX           = g++
CXXFLAGS      = -g -fPIC -Wno-deprecated -O -ansi -D_GNU_SOURCE -g -O2
LD            = g++
LDFLAGS       = -g
SOFLAGS       = -shared


ARCH         := $(shell root-config --arch)
PLATFORM     := $(shell root-config --platform)


CXXFLAGS      += $(ROOTCFLAGS)
#CXX           += -I./
LIBS           = $(ROOTLIBS) 

NGLIBS         = $(ROOTGLIBS) 
NGLIBS        += -lMinuit 
GLIBS          = $(filter-out -lNew, $(NGLIBS))


INCLUDEDIR           = ./
OUTLIB               = ./lib/

.SUFFIXES: .cc,.C,.hh,.h
.PREFIXES: ./lib/

# the main executable
fakerate: $(INCLUDEDIR)/fakerate.cc \
	$(OUTLIB)egtree.o \
	$(OUTLIB)ElectronFakeRateEstimation.o \
	$(OUTLIB)EfficiencyEvaluator.o
	$(CXX) $(CXXFLAGS) -I$(INCLUDEDIR) -ldl -o fakerate $(OUTLIB)/*.o $(GLIBS) $(LDFLAGS) $ $<

# libs
$(OUTLIB)egtree.o: $(INCLUDEDIR)/egtree.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)egtree.o $<
$(OUTLIB)ElectronFakeRateEstimation.o: $(INCLUDEDIR)/ElectronFakeRateEstimation.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ElectronFakeRateEstimation.o $<
$(OUTLIB)EfficiencyEvaluator.o: $(INCLUDEDIR)/../../Math/src/EfficiencyEvaluator.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -I../../ -o $(OUTLIB)EfficiencyEvaluator.o $<

clean:
	rm -f lib/*o
	rm -f FakeRate
