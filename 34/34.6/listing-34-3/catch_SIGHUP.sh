# There are a few ways to get the correct output
# 1. Click the "X" button, whether it's the terminal emulator window frame, or the terminal emulator tab, both are ok 
# 2. manually use another terminal window to send kill -SIGHUP <shell pid from echo $$>

# This will not work
# - Use the "exit" bash builtin

echo $$
catch_SIGHUP x > diffgroup.log 2>&1 &
catch_SIGHUP > samegroup.log 2>&1

# One other weird thing
# When running one of these in foreground mode
# None of the options work
