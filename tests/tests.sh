#!/bin/sh

make
printf "\n"
printf "================================================================\n"
printf "TEST CASE 1\n"
printf "msort /etc/passwd -o mysort\n"
msort /etc/passwd -o mysort
printf "LC_ALL=C sort /etc/passwd -o linuxsort\n"
LC_ALL=C sort /etc/passwd -o linuxsort
printf "diff mysort linuxsort\n"
diff mysort linuxsort
printf "\n"

printf "================================================================\n"
printf "TEST CASE 2\n"
printf "msort /etc/passwd -dr -o mysort\n"
msort /etc/passwd -dr -o mysort
printf "LC_ALL=C sort /etc/passwd -dr -o linuxsort\n"
LC_ALL=C sort /etc/passwd -dr -o linuxsort
printf "diff mysort linuxsort\n"
diff mysort linuxsort

printf "\n\n"

printf "Contents of test-data-1.csv :\n"
head -3 test-data-1.csv
printf "...\n"
tail -3 test-data-1.csv
printf "Size : "
ls -sh test-data-1.csv 
printf "Number of Lines : "
wc -l test-data-1.csv
printf "\n"

printf "================================================================\n"
printf "TEST CASE 3\n"
printf "msort test-data-1.csv -nur -k1,1 -t, -o mysort\n"
msort test-data-1.csv -nur -k1,1 -t, -o mysort
printf "LC_ALL=C sort test-data-1.csv -nur -k1,1 -t, -o linuxsort\n"
LC_ALL=C sort test-data-1.csv -nur -k1,1 -t, -o linuxsort
printf "diff mysort linuxsort\n"
diff mysort linuxsort
printf "\n"

printf "================================================================\n"
printf "TEST CASE 4\n"
printf "msort test-data-1.csv -nru -k3,3 -t, -o mysort\n"
msort test-data-1.csv -nru -k3,3 -t, -o mysort
printf "LC_ALL=C sort test-data-1.csv -nru -k3,3 -t, -o linuxsort\n"
LC_ALL=C sort test-data-1.csv -nru -k3,3 -t, -o linuxsort
printf "diff mysort linuxsort\n"
diff mysort linuxsort
printf "\n"

printf "================================================================\n"
printf "TEST CASE 5\n"
printf "msort test-data-1.csv -k4,4 -fiur -t, -o mysort\n"
msort test-data-1.csv -k4,4 -fiur -t, -o mysort
printf "LC_ALL=C sort test-data-1.csv -k4,4 -fiur -t, -o linuxsort\n"
LC_ALL=C sort test-data-1.csv -k4,4 -fiur -t, -o linuxsort
printf "diff mysort linuxsort\n"
diff mysort linuxsort
printf "\n"

printf "================================================================\n"
printf "TEST CASE 6\n"
printf "msort test-data-1.csv -k2.7,2.10 -urfi -t, -o mysort\n"
msort test-data-1.csv -k2.7,2.10 -urfi -t, -o mysort
printf "LC_ALL=C sort test-data-1.csv -k2.7,2.10 -urfi -t, -o linuxsort\n"
LC_ALL=C sort test-data-1.csv -k2.7,2.10 -urfi -t, -o linuxsort
printf "diff mysort linuxsort\n"
diff mysort linuxsort
printf "\n"

printf "================================================================\n"
printf "TEST CASE 7\n"
printf "msort test-data-1.csv -k1.2,2.1 -t, -o mysort\n"
msort test-data-1.csv -k1.2,2.1 -t, -o mysort
printf "LC_ALL=C sort test-data-1.csv -k1.2,2.1 -t, -o linuxsort\n"
LC_ALL=C sort test-data-1.csv -k1.2,2.1 -t, -o linuxsort
printf "diff mysort linuxsort\n"
diff mysort linuxsort
printf "\n\n"


printf "Contents of test-data-2 :\n"
head -3 test-data-2
printf "...\n"
tail -3 test-data-2
printf "Size : "
ls -sh test-data-2 
printf "Number of Lines : "
wc -l test-data-2
printf "\n"

printf "================================================================\n"
printf "TEST CASE 8\n"
printf "msort test-data-2 -df -o mysort\n"
msort test-data-2 -df -o mysort
printf "LC_ALL=C sort test-data-2 -df -o linuxsort\n"
LC_ALL=C sort test-data-2 -df -o linuxsort
printf "diff mysort linuxsort\n"
diff mysort linuxsort
printf "\n"

printf "================================================================\n"
printf "TEST CASE 9\n"
printf "msort test-data-2 -dur -o mysort\n"
msort test-data-2 -dur -o mysort
printf "LC_ALL=C sort test-data-2 -dur -o linuxsort\n"
LC_ALL=C sort test-data-2 -dur -o linuxsort
printf "diff mysort linuxsort\n"
diff mysort linuxsort
printf "\n"

make clean
rm linuxsort mysort
printf "rm linuxsort mysort\n"
printf "================================================================\n"