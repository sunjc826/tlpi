printf "%s\n" "This should be run in sudo mode" 
mkdir mountpoint
printf "%s\n" "mounting"
mount | grep tmpfs
mount -t tmpfs testtmp mountpoint
printf "%s\n" "mounted"
cat /proc/mounts | grep tmpfs
