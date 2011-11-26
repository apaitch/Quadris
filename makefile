CXX = g++
CXXFLAGS = -Wall -MMD -g
EXEC = main
OBJECTS = Block.o Board.o Level.o CommandTrie.o QuadrisGame.o main.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${DEPENDS}
