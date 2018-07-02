TARGET=nanopascal
C_SRCFILES=main.cpp nanopascal_lexer.cpp nanopascal_parser.cpp nanopascal_nodes.cpp
OBJ_FILES=${C_SRCFILES:.cpp=.o}
.PHONY: clean run
.SUFFIXES:

$(TARGET): $(OBJ_FILES)
	g++ -g -o $@ $(OBJ_FILES)

%.o: %.cpp
	g++ -std=c++14 -g -c -o $@ $<

run: $(TARGET)
	./$(TARGET) ../examples/HelloWorld.pas

clean:
	rm -f *.o
	rm -f $(TARGET)
