#! /usr/bin/env python
# example: submit_batch.py -p test0 -d pccmsrm DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball 1

import os
import sys
import time
import commands
import optparse
import datetime

def writecondorscript(condorname,srcname,logname,outputfilename):
    condorfile = open(condorname,'w')
    condorfile.write('universe=vanilla\n')
    condorfile.write('executable='+srcname+'\n')
    condorfile.write('stream_output = False\n')
    condorfile.write('stream_error  = False\n')
    condorfile.write('WhenToTransferOutput = ON_EXIT\n')
    condorfile.write('transfer_input_files =\n')
    condorfile.write('transfer_Output_files ='+outputfilename+'\n')
    condorfile.write('log    = '+logname+'\n')
    condorfile.write('Notification = Never\n')
    condorfile.write('output = '+logname+'.$(Cluster).$(Process).out\n')
    condorfile.write('error  = '+logname+'.$(Cluster).$(Process).err\n')
    condorfile.write('queue\n')



def main():
#######################################
### usage  
#######################################
    usage = '''usage: %prog [opts] dataset isMC'''
    parser = optparse.OptionParser(usage=usage)
    now = datetime.datetime.now()
    defaultoutputdir='vecbosjob_'+str(now.year)+str(now.month)+str(now.day)+"_"+str(now.hour)+str(now.minute)+str(now.second)
        
    parser.add_option('-q', '--queue',       action='store',     dest='queue',       help='run in batch in queue specified as option (default -q 8nh)', default='8nh')
    parser.add_option('-y', '--year' ,       action='store',     dest='year' ,       help='Year (2011,2012)'                                          , default=2012, type='float')
    parser.add_option('-n', '--nfileperjob', action='store',     dest='nfileperjob', help='split the jobs with n files read/batch job'                , default=10,   type='int')
    parser.add_option('-p', '--prefix',      action='store',     dest='prefix',      help='the prefix to be added to the output'                      , default=defaultoutputdir)
    parser.add_option('-a', '--application', action='store',     dest='application', help='the executable to be run'                                  , default='VecbosApp')
    parser.add_option('-d', '--download',    action='store',     dest='download',    help='download the output on a local computer'                   , default='pccmsrm')
    parser.add_option('-c', '--create',      action='store_true',dest='create',      help='create only the jobs, do not submit them'                  , default=False)
    parser.add_option('-t', '--testnjobs',   action='store',     dest='testnjobs',   help='submit only the first n jobs'                              , default=1000000, type='int')
    parser.add_option('-s', '--site',        action='store',     dest='site',        help='site where to run the job: CERN(default), UCSD'            , default='CERN')
    (opt, args) = parser.parse_args()

    if len(args) != 2:
        print usage
        sys.exit(1)
    dataset = args[0]
    isMC = int(args[1])

    ##########################################
    # if running with xrootd, customize here #
    ##########################################
    if(opt.site=='CERN'): mydir = "/afs/cern.ch/work/e/emanuele/vecbos/VecbosApp2"
    if(opt.site=='UCSD'): mydir = "/home/users/emanuele/releases/CMSSW_5_3_12/src/VecbosApp2"
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

    print "the outputs will be in the directory: "+opt.prefix

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

    if (diskoutputdir != "none" and opt.site=='CERN' and opt.download=='pccmsrm'): 
        os.system("ssh -o BatchMode=yes -o StrictHostKeyChecking=no pccmsrm24 mkdir -p "+diskoutputmain)

    #look for the current directory
    #######################################
    pwd = os.environ['PWD']
    #######################################
    inputListfile=open(inputlist)
    inputfiles = inputListfile.readlines()
    ijob=0

    if(opt.site=='UCSD'):
        sourcefilename = "submitall.src"
        sourcefile = open(sourcefilename,"w")
        
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
        if(opt.site=='CERN'):
            outputfile.write('cp -r '+pwd+"/"+'JSON $WORKDIR\n')
            outputfile.write('cp -r '+pwd+"/"+'EgammaTools $WORKDIR\n')
            outputfile.write('cp -r '+pwd+"/"+'Analysis $WORKDIR\n') 
        if opt.year==2011:
            outputfile.write('export SCRAM_ARCH=slc5_amd64_gcc434\n')
            if(opt.site=='CERN'): outputfile.write('cd /afs/cern.ch/user/e/emanuele/scratch0/higgs/CMSSW_4_2_8_patch7/\n')
            if(opt.site=='UCSD'): outputfile.write('cd /home/users/emanuele/releases/CMSSW_5_3_12/\n')
        else:
            outputfile.write('export SCRAM_ARCH=slc5_amd64_gcc462\n')
            if(opt.site=='CERN'): outputfile.write('cd /afs/cern.ch/work/e/emanuele/vecbos/CMSSW_5_3_12/\n')
            if(opt.site=='UCSD'): outputfile.write('cd /home/users/emanuele/releases/CMSSW_5_3_12/\n')
        outputfile.write('cp '+pwd+'/'+myproxy+' /tmp/\n')
        outputfile.write('eval `scramv1 runtime -sh`\n')
        if(opt.site=='CERN'): outputfile.write('cd $WORKDIR\n')
        outputfile.write(pwd+'/'+opt.application+' '+inputfilename+" "+output+"_"+str(ijob)+" \n")
        if(opt.site=='CERN' and opt.download=='pccmsrm'): outputfile.write('ls *.root | xargs -i scp -o BatchMode=yes -o StrictHostKeyChecking=no {} pccmsrm24:'+diskoutputmain+'/{}\n') 
        outputfile.close
        logfile = opt.prefix+"/"+output+"/log/"+output+"_"+str(ijob)+".log"
        if(opt.site=='CERN') :
            os.system("echo bsub -q "+opt.queue+" -o "+logfile+" source "+pwd+"/"+outputname)
            if(opt.create==False):
                os.system("bsub -q "+opt.queue+" -o "+logfile+" source "+pwd+"/"+outputname)
        if(opt.site=='UCSD') :
            condorname = opt.prefix+"/"+output+"/src/submit_"+str(ijob)+".sub"
            print "created the submission script "+condorname
            writecondorscript(condorname,outputname,logfile,output+"_"+str(ijob)+".root")
            if(opt.create==False):
                #os.system("condor_submit "+condorname)
                sourcefile.write("condor_submit "+condorname+'\n')
        ijob = ijob+1
        if(ijob==opt.testnjobs): break
        # and now cope with the max number of accesses (3000, keep 2500 for the safe side)
        totfiles = opt.nfileperjob*ijob
        if(totfiles % 1000 == 0):
            time.sleep(500);
            print "sleeping 500 s during a dataset...";
        # time.sleep(1)
            continue

if __name__ == "__main__":
        main()

