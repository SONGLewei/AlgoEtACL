import java.io.*;
import java.net.*;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.core.type.TypeReference;
import java.util.List;
import java.util.Map;

public class FileReceiver {
    public static void receiveJson(GraphFrame frame) {
        int port = 12345;

        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("Server is listening on 0.0.0.0:" + port);
            Socket socket = serverSocket.accept();
            System.out.println("Client connected");

            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            StringBuilder jsonContent = new StringBuilder();
            String line;
            while ((line = in.readLine()) != null) {
                jsonContent.append(line).append("\n");
            }

            System.out.println("Received JSON content:");
            // System.out.println(jsonContent);


            // Charger le graphe dans le GraphPanel
            if (frame != null) {
                loadGraphFromJson(frame.getGraphPanel().getGraphe(), jsonContent.toString());
                // frame.getGraphPanel().centerGraph();
                frame.getGraphPanel().repaint();
            }

            socket.close();
        }
        catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    private static void loadGraphFromJson(Graphe graphe, String jsonContent) {
        ObjectMapper mapper = new ObjectMapper();
        try {
            Map<String, Object> graphData = mapper.readValue(jsonContent, new TypeReference<Map<String, Object>>() {});

            // Charger les noeuds
            List<Map<String, Object>> noeuds = (List<Map<String, Object>>) graphData.get("noeuds");
            for (Map<String, Object> noeud : noeuds) {
                String ville = (String) noeud.get("ville");
                double latitude = (double) noeud.get("latitude");
                double longitude = (double) noeud.get("longitude");
                int x = (int) (100.0 * (longitude + 5)); // Ajustement pour un affichage lisible
                int y = (int) (100.0 * (50 - latitude)); // Inverser pour un affichage écran
                graphe.addNode(new Node(ville, x, y));
            }

            // Charger les arêtes
            List<Map<String, String>> aretes = (List<Map<String, String>>) graphData.get("aretes");
            for (Map<String, String> arete : aretes) {
                String de = arete.get("de");
                String vers = arete.get("vers");
                Node start = graphe.findNodeByName(de);
                Node end = graphe.findNodeByName(vers);
                if (start != null && end != null) {
                    graphe.addEdge(new Edge(start, end, 1)); // Poids par défaut
                }
            }

        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
}
