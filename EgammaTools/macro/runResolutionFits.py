#!/usr/bin/python
import sys
import os
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-r", "--resonance", dest="resonance", default="Z",
                  help="decide what resonance to fit (implemeted=Z,JPsi; default=Z)")
(options, args) = parser.parse_args()

inputfdata="/Users/emanuele/Work/data/egamma/jan22/hzzTree_dataZee_2012_Jan22Rereco.root"
if(options.resonance=="Z"):
    inputfmc="/Users/emanuele/Work/data/egamma/jan22/hzzTree_zee.root"
    outputpref="Z2012_"
elif(options.resonance=="JPsi"):
    inputfmc="/Users/emanuele/Work/data/egamma/jan22/jpsitoee_ptj5GeV_fast53X.root"
    outputpref="JPsi2012_"
else:
    "resonance "+options.resonance+" not implemented"
    sys.exit(0)

from ROOT import gROOT
gROOT.LoadMacro('FitZMassScaleAndResolution.C+')
if(options.resonance=="Z"): 
    from ROOT import FitZMassScaleAndResolution
    for ptb in range(0,4):
        print "pt bin = ",ptb
        for etab in range(0,2):
            print "   eta bin = ",etab
            for r9b in range(0,2):
                print "        r9 bin = ",r9b
                outputfmc="zmc"+outputpref+"PtBin"+str(ptb)+"_EtaBin"+str(etab)+"_R9Bin"+str(r9b)
                outputfdata="zdata"+outputpref+"PtBin"+str(ptb)+"_EtaBin"+str(etab)+"_R9Bin"+str(r9b)
                print "        fitting DATA..."
                FitZMassScaleAndResolution(inputfdata,outputfdata,ptb,etab,r9b,0)
                print "        output is in file "+outputfdata
                print "        fitting MC..."
                FitZMassScaleAndResolution(inputfmc,outputfmc,ptb,etab,r9b,1)
                print "        output is in file "+outputfmc

else: 
    from ROOT import FitJPsiMassScaleAndResolution
    for ptb in range(0,5):
        print "pt bin = ",ptb
        for etab in range(0,2):
            print "   eta bin = ",etab
            outputfmc="mc"+outputpref+"PtBin"+str(ptb)+"_EtaBin"+str(etab)
            outputfdata="data"+outputpref+"PtBin"+str(ptb)+"_EtaBin"+str(etab)
            print "        fitting DATA..."
            FitJPsiMassScaleAndResolution(inputfdata,outputfdata,ptb,etab,0)
            print "        output is in file "+outputfdata
            print "        fitting MC..."
            FitJPsiMassScaleAndResolution(inputfmc,outputfmc,ptb,etab,1)
            print "        output is in file "+outputfmc
