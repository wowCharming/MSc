# PTfS_01

ssh ptfs091h@emmy.rrze.uni-erlangen.de

vim main.cpp

g++ -std=c++11 -O3 main.cpp

./a.out


qsub -l nodes=1:ppn=40,walltime=01:00:00 -I

g++ -std=c++11 -O3 main.cpp timing.c && ./a.out
