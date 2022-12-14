> pacct
sudo ./acct_on pacct
sleep 15 &
ulimit -c unlimited
cat
grep xxx badfile
echo $?
t_execve /bin/echo
t_fork
acct_view pacct
sudo ./acct_on /var/log/account/pacct