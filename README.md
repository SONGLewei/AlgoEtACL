# To-Do :

Ecrire la fonction main qui :
- Charger les fichiers JSON
- Crée le graphe complet à partir de ce fichier JSON
- Appliquer l'algorithme FPTAS pour trouver le trajet optimum
- Exporter le graphe optimal en JSON

Sur l'interface :
- Garantir l'indépendance de la librairie graphique
- Charger les JSONs optimaux au lieu des JSONs de base

# Utilisation du Graphe

Chaque fois que vous utilisez un graphe, il faut inclure `GrapheAll.h` dans votre fichier `.cpp`. C'est tout.

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

# Attention

Les caractères corrompus (mauvais encodage) dans le fichier JSON lors du chargement ne sont pas acceptés.