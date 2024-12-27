Chaque fois si vous utilisez un graphe
Il faut d'abord charger le GrapheAll.h sur votre fichier .cpp. C'est tout.

Pour transmettre un fichier de JSON à Graphe: Definir:
      JSONLoader loader;
      auto graphe = loader.chargerGrapheFromJSON("ThePathOfYourJSON")

L'inverse:
      GrapheExporter::exporter(graphe,"TheNameYouWant.json");

ATTENTION!!!
      Les caractères tronqués dans le fichier JSON lors du chargement ne sont pas acceptés, veuillez donc supprimer les caractères tronqués dans le code à l'avance.
