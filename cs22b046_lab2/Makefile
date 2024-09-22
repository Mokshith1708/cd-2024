lexer: lex.yy.c
	gcc -o lexer lex.yy.c -ll
lex.yy.c: $(fname).l
	lex ${fname}.l

clean: 
	rm -f lexer lex.yy.c
