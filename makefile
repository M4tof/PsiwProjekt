all: bin/dystrybutornia bin/m1

bin/dystrybutornia: ./src/dystrybutornia.c | bin
	gcc ./src/dystrybutornia.c -o bin/dystrybutornia

bin/m1: ./src/magazyn.c | bin
	gcc ./src/magazyn.c -o bin/m1

bin:
	mkdir -p bin

clean:
	rm -rf bin
