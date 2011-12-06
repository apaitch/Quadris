CXX = g++
CXXFLAGS = -Wall -MMD -L/usr/X11R6/lib -lX11
EXEC = quadris 
OBJECTS = Ai.o Block.o Board.o Level.o CommandTrie.o QuadrisGame.o XWindow.o ScoreBoard.o main.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${DEPENDS}
