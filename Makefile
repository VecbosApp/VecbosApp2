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
VecbosApp: $(INCLUDEDIR)/Analysis/src/VecbosApp.cc \
	$(OUTLIB)AnalysisVecbosEventContent.o \
	$(OUTLIB)AnalysisAnalysisBase.o \
	$(OUTLIB)DataFormatsEvent.o \
	$(OUTLIB)AnalysisDYToEESelection.o
	$(CXX) $(CXXFLAGS) -I$(INCLUDEDIR) -ldl -o VecbosApp $(OUTLIB)/*.o $(GLIBS) $(LDFLAGS) $ $<

# DataFormats libs
$(OUTLIB)DataFormatsEventHeader.o: $(INCLUDEDIR)/DataFormats/src/EventHeader.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsEventHeader.o $<
$(OUTLIB)DataFormatsVecbosEvent.o: $(INCLUDEDIR)/DataFormats/src/VecbosEvent.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsVecbosEvent.o $<
$(OUTLIB)DataFormatsCandidate.o: $(INCLUDEDIR)/DataFormats/src/Candidate.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsCandidate.o $<
$(OUTLIB)DataFormatsRecoCandidate.o: $(INCLUDEDIR)/DataFormats/src/RecoCandidate.cc \
	$(OUTLIB)DataFormatsCandidate.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsRecoCandidate.o $<
$(OUTLIB)DataFormatsCompositeCandidate.o: $(INCLUDEDIR)/DataFormats/src/CompositeCandidate.cc \
	$(OUTLIB)DataFormatsCandidate.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsCompositeCandidate.o $<
$(OUTLIB)DataFormatsVertex.o: $(INCLUDEDIR)/DataFormats/src/Vertex.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsVertex.o $<
$(OUTLIB)DataFormatsTrack.o: $(INCLUDEDIR)/DataFormats/src/Track.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsTrack.o $<
$(OUTLIB)DataFormatsSuperCluster.o: $(INCLUDEDIR)/DataFormats/src/SuperCluster.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsSuperCluster.o $<
$(OUTLIB)DataFormatsElectron.o: $(INCLUDEDIR)/DataFormats/src/Electron.cc \
	$(OUTLIB)DataFormatsTrack.o \
	$(OUTLIB)DataFormatsSuperCluster.o \
	$(OUTLIB)DataFormatsCandidate.o \
	$(OUTLIB)DataFormatsRecoCandidate.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsElectron.o $<
$(OUTLIB)DataFormatsMuon.o: $(INCLUDEDIR)/DataFormats/src/Muon.cc \
	$(OUTLIB)DataFormatsTrack.o \
	$(OUTLIB)DataFormatsCandidate.o \
	$(OUTLIB)DataFormatsRecoCandidate.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsMuon.o $<
$(OUTLIB)DataFormatsMET.o: $(INCLUDEDIR)/DataFormats/src/MET.cc \
	$(OUTLIB)DataFormatsCandidate.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsMET.o $<
$(OUTLIB)DataFormatsPFMET.o: $(INCLUDEDIR)/DataFormats/src/PFMET.cc \
	$(OUTLIB)DataFormatsMET.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsPFMET.o $<
$(OUTLIB)DataFormatsEvent.o: $(INCLUDEDIR)/DataFormats/src/Event.cc \
	$(OUTLIB)DataFormatsEventHeader.o \
	$(OUTLIB)DataFormatsVertex.o \
	$(OUTLIB)DataFormatsElectron.o \
	$(OUTLIB)DataFormatsMuon.o \
	$(OUTLIB)DataFormatsPFMET.o \
	$(OUTLIB)DataFormatsCompositeCandidate.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsEvent.o $<

# Analysis Tools libs
$(OUTLIB)ToolsVertexSelector.o: $(INCLUDEDIR)/Tools/src/VertexSelector.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ToolsVertexSelector.o $<
$(OUTLIB)ToolsCollectionSelector.o: $(INCLUDEDIR)/Tools/src/CollectionSelector.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ToolsCollectionSelector.o $<

# Egamma Tools libs
$(OUTLIB)EgammaToolsElectronEffectiveArea.o: $(INCLUDEDIR)/EgammaTools/src/ElectronEffectiveArea.cc \
	$(OUTLIB)DataFormatsElectron.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)EgammaToolsElectronEffectiveArea.o $<
$(OUTLIB)EgammaToolsElectronIDAlgo.o: $(INCLUDEDIR)/EgammaTools/src/ElectronIDAlgo.cc \
	$(OUTLIB)DataFormatsElectron.o \
	$(OUTLIB)ToolsVertexSelector.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)EgammaToolsElectronIDAlgo.o $<
$(OUTLIB)EgammaToolsElectronIDSelector.o: $(INCLUDEDIR)/EgammaTools/src/ElectronIDSelector.cc \
	$(OUTLIB)DataFormatsElectron.o \
	$(OUTLIB)ToolsCollectionSelector.o \
	$(OUTLIB)EgammaToolsElectronIDAlgo.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)EgammaToolsElectronIDSelector.o $<


# Analysis generic libs
$(OUTLIB)AnalysisVecbosEventContent.o: $(INCLUDEDIR)/Analysis/src/VecbosEventContent.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)AnalysisVecbosEventContent.o $<
$(OUTLIB)AnalysisAnalysisBase.o: $(INCLUDEDIR)/Analysis/src/AnalysisBase.cc \
	$(OUTLIB)AnalysisVecbosEventContent.o \
	$(OUTLIB)ToolsVertexSelector.o \
	$(OUTLIB)EgammaToolsElectronEffectiveArea.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)AnalysisAnalysisBase.o $<


# Analyzer libs
$(OUTLIB)AnalysisDYToEESelection.o: $(INCLUDEDIR)/Analysis/src/DYToEESelection.cc \
	$(OUTLIB)EgammaToolsElectronIDSelector.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)AnalysisDYToEESelection.o $<


clean:
	rm -f lib/*o
	rm -f VecbosApp
