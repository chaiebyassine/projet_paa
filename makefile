# Nom de l'exécutable
TARGET = main.exe

# Compilateur et options
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Isrc

# Fichiers source
SRC = main.cpp \
      src/model/base/position.cpp \
      src/model/plateau/case.cpp \
      src/model/plateau/plateau.cpp \
      src/model/piece/piece.cpp \
      src/model/piece/pion.cpp \
      src/model/piece/tour.cpp \
      src/model/piece/cavalier.cpp \
      src/model/piece/fou.cpp \
      src/model/piece/reine.cpp \
      src/model/piece/roi.cpp \
      src/model/joueur/joueur.cpp \
      src/model/joueur/joueurHumain.cpp \
      src/model/joueur/joueurIA.cpp \
      src/model/jeu.cpp \
      src/command/commandedeplacement.cpp \
      src/command/commandecapture.cpp \
      src/controller/controleurjeu.cpp \
      src/view/vueSFML.cpp

# Librairies SFML
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Règle principale
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
