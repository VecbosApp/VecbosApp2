#! /usr/bin/env python
import os
import sys
import time
import commands
import optparse
# set parameters to use cmst3 batch 
#######################################
### usage  
#######################################
# if len(sys.argv) != 3:
#     print "usage cmst3_submit_manyfilesperjob.py dataset isMC"
#     sys.exit(1)
dataset = sys.argv[1]
isMC = int(sys.argv[2])

usage = '''usage: %prog [opts] dataset isMC'''
parser = optparse.OptionParser(usage)
parser.add_option('-q', '--queue',       dest='queue', help='run in batch in queue specified as option (default -q 8nh)', default='8nh')
parser.add_option('-y', '--year' ,       dest='year' , help='Year (2011,2012)'                                          , default=None, type='float')
parser.add_option('-n', '--nfileperjob', dest='nfileperjob', help='split the jobs with n files read/batch job'    , default=10,   type='int')
parser.add_option('-p', '--prefix',      dest='prefix', help='the prefix to be added to the output'                    , default='')
parser.add_option('-a', '--application', dest='application', help='the executable to be run'                      , default='VecbosApp')
parser.add_option('-d', '--download',    dest='download',    help='download the output on a local computer'       , default='pccmsrm')

(opt, args) = parser.parse_args()

if isMC != 0:
    if opt.year == 2011:
        inputlist = "Samples/44X/VECBOS_2_44X_V1/MC/"+dataset+".list"
    else:
        inputlist = "Samples/53X/VECBOS_2_53X_V3/MC/"+dataset+".list"
else:
    if opt.year == 2011:
        inputlist = "Samples/44X/VECBOS_2_44X_V1/DATA/"+dataset+".list"
    else:
        inputlist = "Samples/53X/VECBOS_2_53X_V3/Data/"+dataset+".list"

output = dataset

mydir = "/afs/cern.ch/work/e/emanuele/vecbos/VecbosApp2"
myproxy = "x509up_u24421"
os.system("voms-proxy-init -voms cms")
if os.path.isfile("/tmp/"+myproxy):
    print "Proxy file found in /tmp."
else:
    print "Couldn't find the correct proxy in /tmp."
    exit()
os.system("cp /tmp/"+myproxy+" "+mydir)

if opt.download=='pccmsrm':
    if opt.year==2011:
        diskoutputdir = "/cmsrm/pc24_2/emanuele/data/Vecbos4.4.X/"
    else:
        diskoutputdir = "/cmsrm/pc24_2/emanuele/data/Vecbos5.3.X/"
    diskoutputmain = diskoutputdir+"/"+opt.prefix+"/"+output

os.system("mkdir -p "+opt.prefix+"/"+output)
os.system("mkdir -p "+opt.prefix+"/"+output+"/log/")
os.system("mkdir -p "+opt.prefix+"/"+output+"/input/")
os.system("mkdir -p "+opt.prefix+"/"+output+"/src/")
outputroot = diskoutputmain+"/root/"

if diskoutputdir != "none": 
    os.system("ssh -o BatchMode=yes -o StrictHostKeyChecking=no pccmsrm24 mkdir -p "+diskoutputmain)

#look for the current directory
#######################################
pwd = os.environ['PWD']
#######################################
inputListfile=open(inputlist)
inputfiles = inputListfile.readlines()
ijob=0

while (len(inputfiles) > 0):
    inputfilename = pwd+"/"+opt.prefix+"/"+output+"/input/input_"+str(ijob)+".list"
    inputfile = open(inputfilename,'w')
    for line in range(min(opt.nfileperjob,len(inputfiles))):
        ntpfile = inputfiles.pop()
        if ntpfile != '':
            inputfile.write(ntpfile)
            

    inputfile.close()

    # prepare the script to run
    outputname = opt.prefix+"/"+output+"/src/submit_"+str(ijob)+".src"
    outputfile = open(outputname,'w')
    outputfile.write('#!/bin/bash\n')
    # outputfile.write('export STAGE_HOST=castorcms\n')
    # outputfile.write('export STAGE_SVCCLASS=cmst3\n')
    # outputfile.write('cd '+pwd)
    outputfile.write('cp -r '+pwd+"/"+'JSON/data $WORKDIR\n')
    if opt.year==2011:
        outputfile.write('export SCRAM_ARCH=slc5_amd64_gcc434\n')
        outputfile.write('cd /afs/cern.ch/user/e/emanuele/scratch0/higgs/CMSSW_4_2_8_patch7/\n')
    else:
        outputfile.write('export SCRAM_ARCH=slc5_amd64_gcc462\n')
        outputfile.write('cd /afs/cern.ch/work/e/emanuele/vecbos/CMSSW_5_3_12/\n')
    outputfile.write('eval `scramv1 runtime -sh`\n')
    outputfile.write('cd $WORKDIR\n')
    outputfile.write(pwd+'/'+opt.application+' '+inputfilename+" "+output+"_"+str(ijob)+" "+str(isMC)"\n")

    outputfile.write('ls *.root | xargs -i scp -o BatchMode=yes -o StrictHostKeyChecking=no {} pccmsrm24:'+diskoutputmain+'/{}\n') 
    outputfile.close
    os.system("echo bsub -q "+opt.queue+" -o "+opt.prefix+"/"+output+"/log/job_"+str(ijob)+".log source "+pwd+"/"+outputname)
#    os.system("bsub -q "+opt.queue+" -o "+opt.prefix+"/"+dataset+"/"+output+"/log/"+output+"_"+str(ijob)+".log source "+pwd+"/"+outputname+" -copyInput="+dataset+"_"+str(ijob))
    ijob = ijob+1
    # and now cope with the max number of accesses (3000, keep 2500 for the safe side)
    totfiles = opt.nfileperjob*ijob
    if(totfiles % 1000 == 0):
        time.sleep(500);
        print "sleeping 500 s during a dataset...";
    #time.sleep(1)
    continue
