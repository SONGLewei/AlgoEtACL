# Utilisation du Graphe

Chaque fois que vous utilisez un graphe, il faut inclure `BuildNewGraphe.h` dans votre fichier `.cpp`. C'est tout.

## Charger un fichier JSON en Graphe

Pour charger un fichier JSON en Graphe, définissez :

```cpp
JSONLoader loader;
auto graphe = loader.chargerGrapheFromJSON("ThePathOfYourJSON");
```

## Exporter le Graphe en un fichier JSON

Pour exporter le graphe en un fichier JSON, utilisez :

```cpp
GrapheExporter::exporter(graphe, "name.json");
```

## Compiler et Programmer

1. Aller sur CMakeLists.txt, chercher la partie:

    ```txt
    set(SOURCES
        src/PDTotal.cpp 
        ${ALGO_SRC}
        ${GRAPHE_SRC}
    )
    ```
2. Changer la source : src/PDTotal.cpp -> path/theFile.cpp

3. Save tout le projet, en utilisent le Cmake, cliquer build launch le programme par le CMake (Une bande bleue sur le VSCode)

4. Voir le resultat sur ton terminal

## Affichage

1. Tu aura trouver un fichier.json sur le doissier resultat si tout marche bien.

2. Lancer le server Java.

3. En utilisent : 
    ```txt
        .\sendFile.exe Graphe_complet.json 127.0.0.1 12345
    ```
    pour envoyer le fichier de json
    
4. Voir votre resultat sur le server de dessin de graphe.

# Attention

Les caractères corrompus (mauvais encodage) dans le fichier JSON lors du chargement ne sont pas acceptés.