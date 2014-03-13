#!/usr/bin/python
import os
import sys
import optparse

# example: ./lcgCpFile.py /store/mc/Summer12_DR53X/DYToEE_M_20_TuneZ2star_8TeV_pythia6/GEN-SIM-RAW/PU25bx25_START53_V19D-v1/20000/FCD89901-29CD-E211-806E-00304867BFB2.root T1_US_FNAL_Buffer /cmsrm/pc24_2/emanuele/data/DYToEE_M_20_TuneZ2star_8TeV_pythia6_GEN-SIM-RAW+PU25bx25_START53_V19D-v1.root
usage = '''usage: %prog [opts] LFN T2 fileDest'''
parser = optparse.OptionParser(usage=usage)
(opt, args) = parser.parse_args()

file = args[0]
site = args[1]
fileDestination = args[2]

# get the PFN from the LFN
getstr='http://cmsweb.cern.ch/phedex/datasvc/xml/prod/lfn2pfn?node='+site+'&protocol=srmv2&lfn='+file
pfn=os.popen("wget -O- --no-check-certificate \""+getstr+"\"| sed -e \"s/.*pfn='\([^']*\).*/\\1\\n/\"").read()
print "physical file name is: "+pfn+"\n"

# get the file with lcg-cp
command="lcg-cp -b --vo cms -D srmv2 -U srmv2 -v '"+pfn+"' file:"+fileDestination
os.system(command)
