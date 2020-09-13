conversor: conversor.l conversor.y conversor.h
	bison -d conversor.y
	flex -o conversor.lex.c conversor.l
	gcc -o $@ conversor.tab.c conversor.lex.c conversor.c