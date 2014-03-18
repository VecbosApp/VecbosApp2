#!/usr/bin/python
import sys
import os
from optparse import OptionParser

parser = OptionParser()
(options, args) = parser.parse_args()

inputfdata="/Users/emanuele/Work/data/egamma/jan22/hzzTree_dataZee_2012_Jan22Rereco.root"
inputfmc="/Users/emanuele/Work/data/egamma/jan22/hzzTree_zee.root"
outputpref="2012_Jan22_"

from ROOT import gROOT
gROOT.LoadMacro('FitZMassScaleAndResolution.C+')
from ROOT import FitZMassScaleAndResolution

for ptb in range(0,4):
    print "pt bin = ",ptb
    for etab in range(0,2):
        print "   eta bin = ",etab
        for r9b in range(0,2):
            print "        r9 bin = ",r9b
            outputfmc="mc"+outputpref+"PtBin"+str(ptb)+"_EtaBin"+str(etab)+"_R9Bin"+str(r9b)
            outputfdata="data"+outputpref+"PtBin"+str(ptb)+"_EtaBin"+str(etab)+"_R9Bin"+str(r9b)
            print "        fitting DATA..."
            FitZMassScaleAndResolution(inputfdata,outputfdata,ptb,etab,r9b,0)
            print "        output is in file "+outputfdata
            print "        fitting MC..."
            FitZMassScaleAndResolution(inputfmc,outputfmc,ptb,etab,r9b,1)
            print "        output is in file "+outputfmc


