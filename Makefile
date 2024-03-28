all:
	gcc -o main main.c Cluster.c Point.c

clean:
	rm main
