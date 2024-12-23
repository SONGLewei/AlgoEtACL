# Library folder
IDIR = include
# Obj folder
ODIR = obj
# Source folder
SDIR = src
# Binary folder
BDIR = bin

# ++
# ! vide si language c
# ! ++ si language c++
cpp = ++
# C/c++
# ! c si language c
# ! c++ si language c++
language = c$(cpp)
# le compilateur :
#  ! g si language c,
#  ! g++ si language c++,
CC = g$(cpp)
# les options de compilation
CFLAGS = -g -std=$(language)17 -I$(IDIR)

# le programme final
PROG = out

_DEP = AElement.h GElement.h PElement.h Arete.h Sommet.h Graphe.h Ville.h
DEP = $(patsubst %,$(IDIR)/%,$(_DEP))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Toutes les étiquettes qui ne sont pas des fichiers sont déclarées ici
.PHONY: run all remake clean delete

all : $(PROG)

run : all
	./$(PROG)

# La cible est l'exécutable pt_sgt qui dépent des objets $(OBJ)
#		$@ désigne la cible $(PROG)
#		$^ désigne les dépendances $(OBJ)
$(PROG) : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# La cible est un objet (se trouvant dans le répertoire obj)
# Les dépendances sont :
# (+) $(DEP) et
# (+) le fichier source : $(SDIR)/%.c où
#				% désigne le nom de la cible sans le suffixe .o
#			qui se situe dans le répertoire $(ODIR)
$(ODIR)/%.o : $(SDIR)/%.cpp $(DEP)
	$(CC) $(CFLAGS) -c -o $@ $<

# les cibles de nettoyage
clean :
	rm -f $(ODIR)/*.o

delete : clean
	rm $(PROG)