import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.geom.AffineTransform;
import java.util.ArrayList;
import java.util.List;

public class GraphPanel<S,T> extends JPanel {
    private final GraphFrame parentFrame;
    private final Graphe<S,T> graphe;
    private double scale = 1.0; // Initial Scale
    private double offsetX = 0.0;
    private double offsetY = 0.0;
    private int dragStartX, dragStartY;

    public GraphPanel(GraphFrame parentFrame) {
        this.parentFrame = parentFrame;
        this.graphe = new Graphe();

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

        setFocusable(true);
    }

    public Graphe<S,T> getGraphe() {
        return graphe;
    }

    public void resetZoom() {
        scale = 1.0;
        offsetX = 0;
        offsetY = 0;
        parentFrame.updateZoomLabel(scale);
        repaint();
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
        AffineTransform originalTransform = g2d.getTransform();
        g2d.translate(centerX, centerY);
        g2d.scale(scale, scale);
        g2d.translate(-centerX + offsetX, -centerY +offsetY);

        // Dessiner les axes
        //g2d.setColor(Color.BLACK);
        //g2d.drawLine(-getWidth(), 0, getWidth(), 0);
        //g2d.drawLine(0, -getHeight(), 0, getHeight());


        // Dessiner le graphe
        graphe.paint(g2d, scale, offsetX, offsetY);

        g2d.setTransform(originalTransform);

        // Dessiner les arêtes
//        for (Edge edge : edges) {
//            int scaledStartX = (int) (edge._start._x * scale);
//            int scaledEndX = (int) (edge._end._x * scale);
//            int scaledStartY = (int) (edge._start._y * scale);
//            int scaledEndY = (int) (edge._end._y * scale);
//
//            g2d.drawLine(
//                (int) (scaledStartX),
//                (int) (scaledStartY),
//                (int) (scaledEndX),
//                (int) (scaledEndY)
//            );
//            // Afficher le poids au milieu de l'arête
//            int midX = (int) ((edge._start._x + edge._end._x) / 2.0 * scale);
//            int midY = (int) ((edge._start._y + edge._end._y) / 2.0 * scale);
//            g2d.drawString(String.valueOf(edge._weight), midX, midY);
//        }

        // Dessiner les noeuds
//        for (Node node : nodes) {
//            int scaledX = (int) (node._x * scale);
//            int scaledY = (int) (node._y * scale);
//
//            // Dessiner le cercle (Taille fixe peut importe le zoom)
//            g2d.setColor(Color.BLUE);
//            g2d.fillOval(scaledX - 10, scaledY - 10, 20, 20);
//
//            // Dessiner le nom
//            g2d.setColor(Color.BLACK);
//            g2d.drawString(node._name, scaledX - 15, scaledY - 15);
//        }
    }
}