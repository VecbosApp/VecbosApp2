#! /usr/bin/env python
import os
import sys
import time
import commands
import optparse

def main():
#######################################
### usage  
#######################################
    usage = '''usage: %prog [opts] dataset isMC'''
    parser = optparse.OptionParser(usage=usage)
    
        
    parser.add_option('-q', '--queue',       action='store',     dest='queue',       help='run in batch in queue specified as option (default -q 8nh)', default='8nh')
    parser.add_option('-y', '--year' ,       action='store',     dest='year' ,       help='Year (2011,2012)'                                          , default=None, type='float')
    parser.add_option('-n', '--nfileperjob', action='store',     dest='nfileperjob', help='split the jobs with n files read/batch job'                , default=10,   type='int')
    parser.add_option('-p', '--prefix',      action='store',     dest='prefix',      help='the prefix to be added to the output'                      , default='')
    parser.add_option('-a', '--application', action='store',     dest='application', help='the executable to be run'                                  , default='VecbosApp')
    parser.add_option('-d', '--download',    action='store',     dest='download',    help='download the output on a local computer'                   , default='pccmsrm')
    parser.add_option('-c', '--create',      action='store_true',dest='create',      help='create only the jobs, do not submit them'                  , default=False)
    parser.add_option('-t', '--testnjob',    action='store',     dest='testn',       help='submit only the first n jobs'                              , default=5, type='int')
    (opt, args) = parser.parse_args()

    print "len(args) = ",len(args)
    if len(args) != 2:
        print usage
        sys.exit(1)
    dataset = args[0]
    isMC = int(args[1])
        
    ##########################################
    # if running with xrootd, customize here #
    ##########################################
    mydir = "/afs/cern.ch/work/e/emanuele/vecbos/VecbosApp2"
    myproxy = "x509up_u24421"
    ##########################################


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
        outputfile.write('cp -r '+pwd+"/"+'JSON/data $WORKDIR\n')
        if opt.year==2011:
            outputfile.write('export SCRAM_ARCH=slc5_amd64_gcc434\n')
            outputfile.write('cd /afs/cern.ch/user/e/emanuele/scratch0/higgs/CMSSW_4_2_8_patch7/\n')
        else:
            outputfile.write('export SCRAM_ARCH=slc5_amd64_gcc462\n')
            outputfile.write('cd /afs/cern.ch/work/e/emanuele/vecbos/CMSSW_5_3_12/\n')
        outputfile.write('cp '+pwd+'/'+myproxy+' /tmp/\n')
        outputfile.write('eval `scramv1 runtime -sh`\n')
        outputfile.write('cd $WORKDIR\n')
        outputfile.write(pwd+'/'+opt.application+' '+inputfilename+" "+output+"_"+str(ijob)+" "+str(isMC)+"\n")
        outputfile.write('ls *.root | xargs -i scp -o BatchMode=yes -o StrictHostKeyChecking=no {} pccmsrm24:'+diskoutputmain+'/{}\n') 
        outputfile.close
        os.system("echo bsub -q "+opt.queue+" -o "+opt.prefix+"/"+output+"/log/job_"+str(ijob)+".log source "+pwd+"/"+outputname)
        if(opt.create==False):
            os.system("bsub -q "+opt.queue+" -o "+opt.prefix+"/"+dataset+"/"+output+"/log/"+output+"_"+str(ijob)+".log source "+pwd+"/"+outputname+" -copyInput="+dataset+"_"+str(ijob))
        ijob = ijob+1
        # and now cope with the max number of accesses (3000, keep 2500 for the safe side)
        totfiles = opt.nfileperjob*ijob
        if(totfiles % 1000 == 0):
            time.sleep(500);
            print "sleeping 500 s during a dataset...";
        # time.sleep(1)
            continue

if __name__ == "__main__":
        main()

