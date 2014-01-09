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

# Egamma macro executables
CompareROC: $(INCLUDEDIR)/EgammaTools/macro/CompareROC.C \
	$(OUTLIB)ToolsFiguresOfMeritEvaluator.o
	$(CXX) $(CXXFLAGS) -I$(INCLUDEDIR) -ldl -o CompareROC $(OUTLIB)/*.o $(GLIBS) $(LDFLAGS) $ $<

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
$(OUTLIB)DataFormatsGenParticle.o: $(INCLUDEDIR)/DataFormats/src/GenParticle.cc \
	$(OUTLIB)DataFormatsCandidate.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsGenParticle.o $<
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
$(OUTLIB)DataFormatsJet.o: $(INCLUDEDIR)/DataFormats/src/Jet.cc \
	$(OUTLIB)DataFormatsCandidate.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsJet.o $<
$(OUTLIB)DataFormatsPFJet.o: $(INCLUDEDIR)/DataFormats/src/PFJet.cc \
	$(OUTLIB)DataFormatsJet.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsPFJet.o $<

$(OUTLIB)DataFormatsEvent.o: $(INCLUDEDIR)/DataFormats/src/Event.cc \
	$(OUTLIB)DataFormatsEventHeader.o \
	$(OUTLIB)DataFormatsVertex.o \
	$(OUTLIB)DataFormatsGenParticle.o \
	$(OUTLIB)DataFormatsElectron.o \
	$(OUTLIB)DataFormatsMuon.o \
	$(OUTLIB)DataFormatsPFMET.o \
	$(OUTLIB)DataFormatsPFJet.o \
	$(OUTLIB)DataFormatsCompositeCandidate.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)DataFormatsEvent.o $<

# JSON parser tools
$(OUTLIB)JsonFilter.o: $(INCLUDEDIR)JSON/src/JsonFilter.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)JsonFilter.o $<

# Analysis Tools libs
$(OUTLIB)ToolsCollectionPtrCleaner.o: $(INCLUDEDIR)/Tools/src/CollectionPtrCleaner.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ToolsCollectionPtrCleaner.o $<
$(OUTLIB)ToolsVertexSelector.o: $(INCLUDEDIR)/Tools/src/VertexSelector.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ToolsVertexSelector.o $<
$(OUTLIB)ToolsCollectionSelector.o: $(INCLUDEDIR)/Tools/src/CollectionSelector.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ToolsCollectionSelector.o $<
$(OUTLIB)ToolsCandidateKinematicFilter.o: $(INCLUDEDIR)/Tools/src/CandidateKinematicFilter.cc \
	$(OUTLIB)ToolsCollectionSelector.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ToolsCandidateKinematicFilter.o $<
$(OUTLIB)ToolsHLTFilter.o: $(INCLUDEDIR)/Tools/src/HLTFilter.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ToolsHLTFilter.o $<
$(OUTLIB)ToolsCandidateSorter.o: $(INCLUDEDIR)/Tools/src/CandidateSorter.cc \
	$(OUTLIB)DataFormatsCandidate.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ToolsCandidateSorter.o $<
$(OUTLIB)ToolsGenParticleCandidateMatch.o: $(INCLUDEDIR)/Tools/src/GenParticleCandidateMatch.cc \
	$(OUTLIB)DataFormatsCandidate.o \
	$(OUTLIB)DataFormatsGenParticle.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ToolsGenParticleCandidateMatch.o $<
$(OUTLIB)ToolsCandidateCombiner.o: $(INCLUDEDIR)/Tools/src/CandidateCombiner.cc \
	$(OUTLIB)DataFormatsCandidate.o \
	$(OUTLIB)DataFormatsCompositeCandidate.o \
	$(OUTLIB)ToolsCandidateSorter.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ToolsCandidateCombiner.o $<
$(OUTLIB)ToolsFiguresOfMeritEvaluator.o: $(INCLUDEDIR)/Tools/src/FiguresOfMeritEvaluator.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ToolsFiguresOfMeritEvaluator.o $<

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

# Output trees
$(OUTLIB)OutputTreesElectronIDTree.o: $(INCLUDEDIR)/OutputTrees/src/ElectronIDTree.cc \
	$(OUTLIB)DataFormatsElectron.o \
	$(OUTLIB)DataFormatsVertex.o \
	$(OUTLIB)EgammaToolsElectronIDAlgo.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)OutputTreesElectronIDTree.o $<

# Analysis generic libs
$(OUTLIB)AnalysisVecbosEventContent.o: $(INCLUDEDIR)/Analysis/src/VecbosEventContent.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)AnalysisVecbosEventContent.o $<
$(OUTLIB)AnalysisAnalysisBase.o: $(INCLUDEDIR)/Analysis/src/AnalysisBase.cc \
	$(OUTLIB)ToolsCollectionPtrCleaner.o \
	$(OUTLIB)ToolsHLTFilter.o \
	$(OUTLIB)JsonFilter.o \
	$(OUTLIB)AnalysisVecbosEventContent.o \
	$(OUTLIB)ToolsVertexSelector.o \
	$(OUTLIB)EgammaToolsElectronEffectiveArea.o \
	$(OUTLIB)ToolsGenParticleCandidateMatch.o \
	$(OUTLIB)ToolsCandidateKinematicFilter.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)AnalysisAnalysisBase.o $<

# Analyzer libs
$(OUTLIB)AnalysisDYToEESelection.o: $(INCLUDEDIR)/Analysis/src/DYToEESelection.cc \
	$(OUTLIB)EgammaToolsElectronIDSelector.o \
	$(OUTLIB)OutputTreesElectronIDTree.o \
	$(OUTLIB)ToolsCandidateCombiner.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)AnalysisDYToEESelection.o $<


clean:
	rm -f lib/*o
	rm -f VecbosApp
