all: hw1 hw2 hw3 hw4 hw5 hw6 hw7 broadcast sum sum2 barrier fox_totally_going_to_work cannon fox_data cannon_data

hw1: hw1.c
	mpicc -o hw1 hw1.c

hw2: hw2.c
	mpicc -o hw2 hw2.c

hw3: hw3.c
	mpicc -o hw3 hw3.c

hw4: hw4.c
	mpicc -o hw4 hw4.c

hw5: hw5.c
	mpicc -o hw5 hw5.c

hw6: hw6.c
	mpicc -o hw6 hw6.c

hw7: hw7.c
	mpicc -o hw7 hw7.c

broadcast: broadcast.c
	mpicc -o broadcast broadcast.c

sum: sum.c
	mpicc -o sum sum.c

sum2: sum2.c
	mpicc -o sum2 sum2.c

barrier: barrier.c
	mpicc -o barrier barrier.c

fox_totally_going_to_work: fox_totally_going_to_work.c
	mpicc -o fox_totally_going_to_work fox_totally_going_to_work.c -lm
	
cannon: cannon.c
	mpicc -o cannon cannon.c -lm

fox_data: fox_data.c
	mpicc -o fox_data fox_data.c -lm

cannon_data: cannon_data.c
	mpicc -o cannon_data cannon_data.c -lm
