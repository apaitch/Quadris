OBJECT=Block.o Board.o Level.o CommandTrie.o QuadrisGame.o main.o

main: ${OBJECT}
	g++ -g ${OBJECT} -o main

main.o: main.cc

Block.o:Block.h Block.cc

Board.o:Board.h Board.cc

Level.o:Level.h PRNG.h Level.cc

CommandTrie.o:CommandTrie.h CommandTrie.cc

QuadrisGame.o:Block.h Board.h Level.h CommandTrie.h QuadrisGame.h QuadrisGame.cc

clean:
	rm ${OBJECT}