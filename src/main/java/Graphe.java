import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class Graphe<S,T> {
    private final List<Node<T>> nodes;
    private final List<Edge<S,T>> edges;

    public Graphe() {
        this.nodes = new ArrayList<>();
        this.edges = new ArrayList<>();
    }

    public void addNode(Node<T> node) {
        nodes.add(node);
    }

    public List<Node<T>> getNodes() {
        return nodes;
    }

    public void addEdge(Edge<S,T> edge) {
        edges.add(edge);
    }

    public List<Edge<S,T>> getEdges() {
        return edges;
    }

    public Node<T> findNodeByName(String name) {
        for (Node<T> node : nodes) {
            if (node._name.equals(name)) {
                return node;
            }
        }
        return null;
    }

    public void paint(Graphics2D g2d, double scale, double offsetX, double offsetY) {
        // Dessiner les arêtes

        for (Edge<S,T> edge : edges) {
            g2d.setColor(Color.BLACK);
            g2d.drawLine(
                (int) (edge._start._x * scale + offsetX),
                (int) (edge._start._y * scale + offsetY),
                (int) (edge._end._x * scale + offsetX),
                (int) (edge._end._y * scale + offsetY)
            );
            // Afficher le poids au milieu de l'arête
            int midX = (int) ((edge._start._x + edge._end._x) / 2.0 * scale + offsetX);
            int midY = (int) ((edge._start._y + edge._end._y) / 2.0 * scale + offsetY);
            g2d.drawString(String.valueOf(edge._weight), midX, midY);
        }

        // Dessiner les noeuds
        for (Node<T> node : nodes) {
            int scaledX = (int) (node._x * scale + offsetX);
            int scaledY = (int) (node._y * scale + offsetY);

            // Dessiner le cercle
            g2d.setColor(Color.BLUE);
            g2d.fillOval(scaledX - 10, scaledY - 10, 20, 20);

            // Dessiner le nom
            g2d.setColor(Color.BLACK);
            g2d.drawString(node._name.toString(), scaledX - 15, scaledY - 15);
        }
    }
}