compilador: lex.yy.c new_parser.tab.c new_ast.cpp
	g++ lex.yy.c new_parser.tab.c new_ast.cpp -lfl -o compilador

all: new_lexical_analyzer.lex new_parser.y new_ast.cpp
	bison -d new_parser.y
	lex new_lexical_analyzer.lex
	g++ lex.yy.c new_parser.tab.c new_ast.cpp -lfl -o compilador

zip: lex.yy.c new_parser.tab.c new_parser.tab.h new_ast.cpp new_ast.hpp Makefile
	zip -r compilador.zip lex.yy.c new_parser.tab.c new_parser.tab.h new_ast.cpp new_ast.hpp codegen.cpp Makefile

clean:
	$(RM) lex.yy.c
	$(RM) new_parser.tab.c
	$(RM) new_parser.tab.h
	$(RM) compilador
	$(RM) compilador.zip