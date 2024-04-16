FLAGS = -g -c -Wall
COMPILED = compiled

all: main.c tads/Cluster.c tads/Cluster.h tads/Point.c tads/Point.h tads/Distance.c tads/Distance.h tads/Group.c tads/Group.h
	@mkdir -p $(COMPILED)
	@gcc $(FLAGS) main.c -o $(COMPILED)/main.o
	@gcc $(FLAGS) tads/Cluster.c -o $(COMPILED)/Cluster.o
	@gcc $(FLAGS) tads/Point.c -o $(COMPILED)/Point.o
	@gcc $(FLAGS) tads/Group.c -o $(COMPILED)/Group.o
	@gcc $(FLAGS) tads/Distance.c -o $(COMPILED)/Distance.o
	@gcc -o main $(COMPILED)/main.o $(COMPILED)/Cluster.o $(COMPILED)/Point.o $(COMPILED)/Distance.o $(COMPILED)/Group.o -lm

run:
	./main in-exemplos/5.txt 10 saida.txt

run_spec:
	./main in-exemplos/spec.txt 3 saida.txt

run_unordered_spec:
	./main in-exemplos/unordered_spec.txt 3 saida.txt

run_30k:
	./main in-exemplos/30k_5dim.txt 10 saida.txt

valgrind_30k:
	valgrind --leak-check=full --show-leak-kinds=all ./main in-exemplos/30k_5dim.txt 10 saida.txt

valgrind_spec:
	valgrind --leak-check=full --show-leak-kinds=all ./main in-exemplos/spec.txt 3 saida.txt

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./main in-exemplos/5.txt 10 saida.txt

clean:
	@rm -f $(COMPILED)/*.o
	@rm -rf $(COMPILED)
	@rm -f main
