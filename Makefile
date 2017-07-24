compilador: new_lexical_analyzer new_parser.y
	lex new_lexical_analyzer
	bison -d new_parser.y
	g++ lex.yy.c new_parser.tab.c -lfl -o compilador
