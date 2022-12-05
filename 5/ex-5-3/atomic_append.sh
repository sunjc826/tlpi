> test.txt
atomic_append test.txt 100000 & atomic_append test.txt 100000
ls -l test.txt
> test.txt
atomic_append test.txt 100000 x & atomic_append test.txt 100000 x
ls -l test.txt
