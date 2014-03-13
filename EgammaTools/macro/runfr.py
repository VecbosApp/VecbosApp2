#!/usr/bin/python
import sys
import os
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-r", "--recompile",
                  action="store_true", dest="recompile", default=False,
                  help="recompile the fake rate estimator")
parser.add_option("-d", "--drawonly",
                  action="store_true", dest="drawonly", default=False,
                  help="do not run. Only draw the results based on existing ROOT files")

(options, args) = parser.parse_args()

if(options.drawonly == False):
    if(options.recompile):
        print 'recompiling...'
        os.system('make clean')
        os.system('make fakerate')
        print 'done complining.'

    os.system('./fakerate zll1fake fr')
    os.system('hadd -f fakerates_nontrg.root fr-EleMisid*')
    os.system('rm -f fr-EleMisid*.root')

from ROOT import gROOT
gROOT.LoadMacro('drawFR.cc+')
from ROOT import drawIDCutWPs
drawIDCutWPs()
from ROOT import drawIDmvaWPs
drawIDmvaWPs()
