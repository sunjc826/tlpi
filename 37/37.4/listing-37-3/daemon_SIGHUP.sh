set -v
rm /tmp/ds.conf /tmp/ds.log /tmp/old_ds.log
echo -n START > /tmp/ds.conf
daemon_SIGHUP
sleep 10
cat /tmp/ds.log
echo -n CHANGED > /tmp/ds.conf 
date +'%F %X'; mv /tmp/ds.log /tmp/old_ds.log
sleep 10
date +'%F %X'; killall -SIGHUP daemon_SIGHUP
sleep 20
ls /tmp/*ds.log
cat /tmp/old_ds.log
cat /tmp/ds.log
killall -SIGTERM daemon_SIGHUP
set +v