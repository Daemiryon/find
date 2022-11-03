# Macros
exec = clang -Wall -Wextra -pedantic -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
comp = clang -c -Wall -Wextra -pedantic -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
toexe = -o
toobj = -o

# -----
# Compilation
# # fichier.o: dépendances (le .c et les .h)
# # 	$(comp) .c $(toobj) fichier.o
parser.o: src/parser.h src/parser.c
	$(comp) src/parser.c $(toobj) parser.o
# -----
# Exécutable
# # fichier.exe: dépendances .o
# # 	$(exec) .o $(toexe) fichier.exe
ftc: parser.o
	$(exec) parser.o $(toexe) ftc
# -----
# Exécution
# # nom_commande: dépendances fichier.exe
# # 	@./fichier.exe

# -----
# clean obj and exe repertories
clean:
	@rm -f obj/*.o ftc