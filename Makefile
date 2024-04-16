FLAGS = -g -c -Wall
COMPILED = compiled

all: main.c tads/Cluster.c tads/Cluster.h tads/Point.c tads/Point.h tads/Distance.c tads/Distance.h tads/Group.c tads/Group.h
	@mkdir -p $(COMPILED)
	@gcc $(FLAGS) main.c -o $(COMPILED)/main.o
	@gcc $(FLAGS) tads/Cluster.c -o $(COMPILED)/Cluster.o
	@gcc $(FLAGS) tads/Point.c -o $(COMPILED)/Point.o
	@gcc $(FLAGS) tads/Group.c -o $(COMPILED)/Group.o
	@gcc $(FLAGS) tads/Distance.c -o $(COMPILED)/Distance.o
	@gcc -o trab1 $(COMPILED)/main.o $(COMPILED)/Cluster.o $(COMPILED)/Point.o $(COMPILED)/Distance.o $(COMPILED)/Group.o -lm

run:
	./trab1 in-exemplos/5.txt 10 out_exemplos/out5a.txt

run_spec:
	./trab1 in-exemplos/spec.txt 3 saida.txt

run_unordered_spec:
	./trab1 in-exemplos/unordered_spec.txt 3 saida.txt

run_30k:
	./trab1 in-exemplos/30k_5dim.txt 10 saida.txt

valgrind_30k:
	valgrind --leak-check=full --show-leak-kinds=all ./trab1 in-exemplos/30k_5dim.txt 10 saida.txt

valgrind_spec:
	valgrind --leak-check=full --show-leak-kinds=all ./trab1 in-exemplos/spec.txt 3 saida.txt

valgrind1:
	valgrind --leak-check=full --show-leak-kinds=all ./trab1 in-exemplos/1.txt 2 out_exemplos/out1a.txt

valgrind2:
	valgrind --leak-check=full --show-leak-kinds=all ./trab1 in-exemplos/2.txt 4 out_exemplos/out2a.txt

valgrind3:
	valgrind --leak-check=full --show-leak-kinds=all ./trab1 in-exemplos/3.txt 5 out_exemplos/out3a.txt

valgrind4:
	valgrind --leak-check=full --show-leak-kinds=all ./trab1 in-exemplos/4.txt 5 out_exemplos/out4a.txt

valgrind5:
	valgrind --leak-check=full --show-leak-kinds=all ./trab1 in-exemplos/5.txt 10 out_exemplos/out5a.txt

run_10k_5dim:
	./trab1 10k_5dim.txt 100 saida.txt

clean:
	@rm -f $(COMPILED)/*.o
	@rm -rf $(COMPILED)
	@rm -f trab1
