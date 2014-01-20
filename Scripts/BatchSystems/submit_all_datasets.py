#! /usr/bin/env python

import os
import sys
import time
import commands
import optparse
import datetime

def main():
    parser = optparse.OptionParser(usage="submit_all_datasets.py --data --prefix=zll1fake")
    now = datetime.datetime.now()
    defaultoutputdir='vecbosjob_'+str(now.year)+str(now.month)+str(now.day)+"_"+str(now.hour)+str(now.minute)+str(now.second)
    parser.add_option('-d', '--data',  action='store_true', dest='data',  help='run on data (8 TeV)'   , default=True)
    parser.add_option('-p', '--prefix',      action='store',     dest='prefix',      help='the prefix to be added to the output'                      , default=defaultoutputdir)
    (opt, args) = parser.parse_args()

    if opt.data==True:
        print "Submitting DoubleElectron now...\n"
        os.system("Scripts/BatchSystems/submit_batch.py -n 4 -p "+opt.prefix+" -d pccmsrm DoubleElectron_Run2012A-22Jan2013 0")
        os.system("Scripts/BatchSystems/submit_batch.py -n 4 -p "+opt.prefix+" -d pccmsrm DoubleElectron_Run2012B-22Jan2013 0")
        os.system("Scripts/BatchSystems/submit_batch.py -n 4 -p "+opt.prefix+" -d pccmsrm DoubleElectron_Run2012C-22Jan2013 0")
        os.system("Scripts/BatchSystems/submit_batch.py -n 4 -p "+opt.prefix+" -d pccmsrm DoubleElectron_Run2012D-22Jan2013 0")
    
        print "Submitting DoubleMuon now...\n"
        os.System("Scripts/BatchSystems/submit_batch.py -n 4 -p "+opt.prefix+" -d pccmsrm DoubleMu_Run2012A-22Jan2013 0")
        os.System("Scripts/BatchSystems/submit_batch.py -n 4 -p "+opt.prefix+" -d pccmsrm DoubleMuParked_Run2012B-22Jan2013 0")
        os.System("Scripts/BatchSystems/submit_batch.py -n 4 -p "+opt.prefix+" -d pccmsrm DoubleMuParked_Run2012C-22Jan2013 0")
        os.System("Scripts/BatchSystems/submit_batch.py -n 4 -p "+opt.prefix+" -d pccmsrm DoubleMuParked_Run2012D-22Jan2013 0")

if __name__ == "__main__":
        main()
