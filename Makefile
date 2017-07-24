compilador: new_lexical_analyzer.lex new_parser.y new_ast.cpp
	bison -d new_parser.y
	lex new_lexical_analyzer.lex
	g++ lex.yy.c new_parser.tab.c new_ast.cpp -lfl -o compilador


clean:
	$(RM) lex.yy.c
	$(RM) new_parser.tab.c
	$(RM) new_parser.tab.h
	$(RM) compilador