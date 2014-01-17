import FWCore.ParameterSet.Config as cms

process = cms.Process("RUN")

readFiles = cms.untracked.vstring()
process.source = cms.Source("PoolSource",
                            noEventSort = cms.untracked.bool(True),
                            #Run: 1 Event: 80333
                            duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                            fileNames = readFiles
                            )
readFiles.extend([
#  53X data file
#    '/store/data/Run2012D/DoubleElectron/AOD/22Jan2013-v1/10027/7A1610EB-0990-E211-BC9F-002354EF3BE6.root',
#  53X MC file
    '/store/mc/Summer12_DR53X/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/GEN-SIM-RECO/HLT8E33_PU_S10_START53_V7I-v1/30000/FCC5A42A-1A7A-E211-B82D-0025901D4B08.root'
])
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1))

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
# 53X data
# process.GlobalTag.globaltag = 'FT53_V21A_AN6::All' 
# 53X MC
process.GlobalTag.globaltag = 'START53_V27::All'

process.readAK5PF = cms.EDAnalyzer('JetCorrectorDBReader', 
                                   payloadName    = cms.untracked.string('AK5PF'), # this is the communication to the databas
                                   printScreen    = cms.untracked.bool(False),
                                   createTextFile = cms.untracked.bool(True),
                                   globalTag      = cms.untracked.string('START53_V27') # this is used ONLY for the name of the printed txt files. You can use any name that you like, but it is recommended to use the GT name that you retrieved the files from.
                                   )
process.p = cms.Path(process.readAK5PF)
