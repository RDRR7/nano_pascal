TARGET=nanopascal
C_SRCFILES=main.cpp nanopascal_lexer.cpp nanopascal_parser.cpp
OBJ_FILES=${C_SRCFILES:.cpp=.o}
.PHONY: clean run
.SUFFIXES:

$(TARGET): $(OBJ_FILES)
	g++ -g -o $@ $(OBJ_FILES)

%.o: %.cpp
	g++ -std=c++11 -g -c -o $@ $<

run: $(TARGET)
	./$(TARGET) ../examples/gcd.pas

clean:
	rm -f *.o
	rm -f $(TARGET)
