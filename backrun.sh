# sudo vim  ~/.bashrc
# alias backrun='~/projects/backrun.sh'
# @ruanqc, 202511,pls moves to project levl
#ps aux |grep mpi
ps -e -o pid,user,etime,cmd |grep mpi
