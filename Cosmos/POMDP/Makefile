CLASSES= exPapier.o generateLHA.o generateSPN.o readPlast.o main.o 


all: swn

linear: generator
	./swn linear 5 5

swn: $(CLASSES)
	g++ -o swn $(CLASSES)
# 	g++ -o linear linear.cpp
#	g++ -o grid grid.cpp

clean:
	rm *.o
	rm swn


exec: generator
	./swn default 1


loop:
	./generator test2
	Cosmos generated.gspn generated.lha -d test --max-run 100000 --batch 10000
	tail -n 1 test > test2
