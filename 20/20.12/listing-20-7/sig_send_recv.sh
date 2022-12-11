sig_receiver 5 &
sig_sender $(pidof sig_receiver) 1000 10 2
sleep 5
sig_receiver &
sig_sender $(pidof sig_receiver) 1000 10 2