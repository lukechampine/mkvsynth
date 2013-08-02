del: delbrot/lex.yy.c delbrot/y.tab.c delbrot/delbrot.c delbrot/plugins.c
	@cc -lm -lpthread jarvis/bufferAllocation.c jarvis/spawn.c delbrot/lex.yy.c delbrot/y.tab.c delbrot/delbrot.c delbrot/plugins.c filters/gradientVideoGenerate.c filters/fakeFilter.c filters/fakeSource.c -o del

delbrot/lex.yy.c: delbrot/delbrot.l
	@cd delbrot && flex delbrot.l && cd ..

delbrot/y.tab.c delbrot/delbrot.c: delbrot/delbrot.y
	@cd delbrot && yacc -d delbrot.y && cd ..
