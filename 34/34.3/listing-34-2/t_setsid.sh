set -o verbose
ps -p $$ -o 'pid pgid sid command'
t_setsid
set +o verbose