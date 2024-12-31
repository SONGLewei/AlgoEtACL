import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.core.type.TypeReference;
import java.io.File;
import java.io.IOException;
import java.util.List;


public class Loader {
    private static final double SCALE = 80.0; // Échelle pour convertir latitude/longitude en coordonnées écran

    public static void loadGraphFromFile(Graphe graphe, String filePath) {
        ObjectMapper mapper = new ObjectMapper();
        try {
            List<City> cities = mapper.readValue(new File(filePath), new TypeReference<List<City>>() {});

            // Ajouter les villes comme nœuds
            for (City city : cities) {
                int x = (int) (0.9 * SCALE * (city.getLongitude() + 8)); // Ajustement pour un affichage lisible
                int y = (int) (SCALE * (52 - city.getLatitude())); // Inverser pour un affichage écran
                graphe.addNode(new Node(city.getVille(), x, y));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
