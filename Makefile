# Macros
exec = clang -Wall
comp = clang -c -Wall 
toexe = -o
toobj = -o

# -----
# Exécutable
ftc: ftc.o parser.o check_param.o MegaMimes.o filters.o
	$(exec) ftc.o parser.o check_param.o MegaMimes.o filters.o $(toexe) ftc

# -----
# Compilation
parser.o: src/parser.h src/parser.c
	$(comp) src/parser.c $(toobj) parser.o


check_param.o: src/check_param.h src/parser.h src/check_param.c
	$(comp) src/check_param.c $(toobj) check_param.o

ftc.o: src/parser.h src/check_param.h src/ftc.c
	$(comp) src/ftc.c $(toobj) ftc.o

list.o: src/list.h src/list.c
	$(comp) src/list.c $(toobj) list.o

MegaMimes.o: src/MegaMimes.h src/MegaMimes.c
	$(comp) src/MegaMimes.c $(toobj) MegaMimes.o

filters.o: src/filters.h src/filters.c
	$(comp) src/filters.c $(toobj) filters.o

# -----
# Clean object and executable files
clean:
	@rm -f *.o ftc