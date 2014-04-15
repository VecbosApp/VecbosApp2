#!/bin/bash -f
# usage: ./createListsEOS.sh /store/group/phys_higgs/crovelli/Data2012B_hww

while getopts "s:t:d:h" opt; do
  case $opt in
      s)
      site=$OPTARG
      ;;
      t)
      t2dir=$OPTARG
      ;;
      d)
      maindir=$OPTARG
      ;;
      h)
      echo "USAGE: createLists.sh -s <site (UCSD,Caltech,CERN,Rome)> -t <T2 mount directory (the one containing one folder/dataset)> -d <dir where to put lists> "
      echo example: createListsEOS.sh -s UCSD -t /hadoop/cms/store/user/emanuele/VECBOS_2_53X_V3 -d lists
      exit 0
      ;;
  esac
done

if [ "$site" == 'UCSD' ] || [ "$site" == 'Caltech' ]; then
    echo "listing $t2dir"
    ls -l $t2dir | awk '{print $9}' > datasets.txt
    ls -l $t2dir | awk '{print "'"$t2dir"'" "/" $9}' | xargs -i echo "ls -l " {} " | grep -v \" 0 \" | awk '{print \"{}/\" \$9}'" > commands.txt 
fi

if [ "$site" == 'Rome' ]; then 
    echo "listing /pnfs/roma1.infn.it/data/cms/$t2dir"
    ls -l $t2dir | awk '{print $9}' > datasets.txt
    ls -l $t2dir | awk '{print "'"$t2dir"'" "/" $9}' | xargs -i echo "ls -l " {} " | grep -v \" 0 \" | awk '{print \"{}/\" \$9}'" > commands.txt 
fi

if [ "$site" == 'CERN' ]; then 
    echo "listing $t2dir"
    eos.select ls -l $t2dir | awk '{print $9}' > datasets.txt
    eos.select ls -l $t2dir | awk '{print "'"$t2dir"'" "/" $9}' | xargs -i echo "eos.select ls -l " {} " | grep -v \" 0 \" | awk '{print \"{}/\" \$9}'" > commands.txt 
fi


N=0
while read LINE ; do
    N=$((N+1))
    echo "Processing $LINE"
    names[${N}]=$LINE
done < datasets.txt


N=0
while read LINE ; do
    N=$((N+1))
    namescommand[${N}]=$LINE
    namesnum=${#namescommand}
done < commands.txt

rm -f finalcommand.sh

if [ "$site" == 'UCSD' ]; then
    for ((i=1;i<$N+1;i++)); do
	echo ${namescommand[${i}]} " | grep default | awk '{print \"root://xrootd.t2.ucsd.edu/\" \$1}' >" $maindir"/"${names[${i}]}".list" >> finalcommand.sh
    done
fi

if [ "$site" == 'Caltech' ]; then
    for ((i=1;i<$N+1;i++)); do
        echo ${namescommand[${i}]} " | grep default | awk '{print \"root://xrootd.unl.edu/\" \$1}' >" $maindir"/"${names[${i}]}".list" >> finalcommand.sh
    done
fi


if [ "$site" == 'Rome' ]; then
    for ((i=1;i<$N+1;i++)); do
	echo ${namescommand[${i}]} " | grep default | awk '{print \"root://cmsrm-xrootd02.roma1.infn.it:7070//\" \$1}' >" $maindir"/"${names[${i}]}".list" >> finalcommand.sh
    done	
fi

if [ "$site" == 'CERN' ]; then
    for ((i=1;i<$N+1;i++)); do
	echo ${namescommand[${i}]} " | grep default | awk '{print \"root://eoscms/\" \$1}' >" $maindir"/"${names[${i}]}".list" >> finalcommand.sh
    done
fi

echo "NOW reading from castor. It may take time..."

source finalcommand.sh

rm -f datasets.txt
rm -f commands.txt
rm -f finalcommand.sh

echo "cleaning..."
if [ "$site" == 'Caltech' ]; then
    files=`ls $maindir/`
    for file in $files
      do
        sed -i 's|/mnt/tier2||g' $maindir/$file
      done
fi

echo "LISTS are done in dir $maindir."
