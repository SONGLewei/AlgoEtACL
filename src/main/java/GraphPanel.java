import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.geom.AffineTransform;
import java.util.ArrayList;
import java.util.List;
// import Node;
// import Edge;

public class GraphPanel extends JPanel {
    private final GraphFrame parentFrame;
    private final List<Node> nodes;
    private final List<Edge> edges;
    private double scale = 1.0; // Initial Scale
    private double offsetX = 0.0;
    private double offsetY = 0.0;
    private int dragStartX, dragStartY;

    public GraphPanel(GraphFrame parentFrame) {
        this.parentFrame = parentFrame;
        nodes = new ArrayList<>();
        edges = new ArrayList<>();

        // Gestion du déplacement avec clic gauche
        addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                dragStartX = e.getX();
                dragStartY = e.getY();
            }
        });

        addMouseMotionListener(new MouseAdapter() {
            @Override
            public void mouseDragged(MouseEvent e) {
                int deltaX = e.getX() - dragStartX;
                int deltaY = e.getY() - dragStartY;

                dragStartX = e.getX();
                dragStartY = e.getY();

                offsetX += deltaX / scale;
                offsetY += deltaY / scale;

                repaint();
            }
        });

        // Gestion du zoom avec la molette
        addMouseWheelListener(e -> {
            double oldScale = scale;
            if (e.getPreciseWheelRotation() < 0) {
                scale *= 1.1;
            }
            else {
                scale /= 1.1;
            }

            // Calcul de la position de la souris
            double mouseX = e.getX();
            double mouseY = e.getY();

            // Ajuster les offsets pour que le zoom se fasse au niveau de la souris
            offsetX = mouseX - (mouseX - offsetX) * (scale / oldScale);
            offsetY = mouseY - (mouseY - offsetY) * (scale / oldScale);

            parentFrame.updateZoomLabel(scale); // Met à jour l'étiquette du zoom
            repaint();
        });

        // Gestion du zoom avec les touches Numpad+ et Numpad-
        addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ADD) {
                    scale *= 1.1; // Zoom avant
                } else if (e.getKeyCode() == KeyEvent.VK_SUBTRACT) {
                    scale /= 1.1; // Zoom arrière
                }

                repaint();
            }
        });

        setFocusable(true);
    }

    public void resetZoom() {
        scale = 1.0;
        offsetX = 0;
        offsetY = 0;
        parentFrame.updateZoomLabel(scale);
        repaint();
    }

    public void addNode(String name, int x, int y) {
        nodes.add(new Node(name, x, y));
        repaint();
    }

    public void addEdge(String start, String end, int weight) {
        Node startNode = findNodeByName(start);
        Node endNode = findNodeByName(end);
        if (startNode != null && endNode != null) {
            edges.add(new Edge(startNode, endNode, weight));
            repaint();
        }
    }

    private Node findNodeByName(String name) {
        for (Node node : nodes) {
            if (node._name.equals(name)) {
                return node;
            }
        }
        return null;
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;

        // Calculer le centre de la zone visible
        int panelWidth = getWidth();
        int panelHeight = getHeight();
        int centerX = panelWidth / 2;
        int centerY = panelHeight / 2;

        // Centrer le dessin en fonction de l'echelle
        AffineTransform transform = g2d.getTransform();
        g2d.translate(centerX, centerY);
        g2d.scale(scale, scale);
        g2d.translate(-centerX + offsetX, -centerY +offsetY);

        // Dessiner les arêtes
        for (Edge edge : edges) {
            g2d.drawLine(edge._start._x, edge._start._y, edge._end._x, edge._end._y);
            // Afficher le poids au milieu de l'arête
            int midX = (edge._start._x + edge._end._x) / 2;
            int midY = (edge._start._y + edge._end._y) / 2;
            g2d.drawString(String.valueOf(edge._weight), midX, midY);
        }

        // Dessiner les noeuds
        for (Node node : nodes) {
            int scaledX = (int) (node._x * scale);
            int scaledY = (int) (node._y * scale);

            // Dessiner le cercle (Taille fixe peut importe le zoom)
            g2d.setColor(Color.BLUE);
            g2d.fillOval(scaledX - 10, scaledY - 10, 20, 20);

            // Dessiner le nom
            g2d.setColor(Color.BLACK);
            g2d.drawString(node._name, scaledX - 15, scaledY - 15);
        }

        // Afficher le niveau de zoom
        // g2d.setColor(Color.BLACK);
        // String zoomText = String.format("Zoom: %.2f%%", scale * 100);
        // g2d.drawString(zoomText, getWidth() - 100, getHeight() - 10);
    }
}