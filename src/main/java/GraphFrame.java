import javax.swing.*;
import java.awt.*;
// import GraphPanel;

public class GraphFrame extends JFrame {
    private final GraphPanel graphPanel;
    private final JLabel zoomLabel;

    public GraphFrame() {
        setTitle("Serveur de dessin de graphe");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(1900, 1000);

        graphPanel = new GraphPanel(this);
        add(graphPanel, BorderLayout.CENTER);

        // Afficher le niveau de zoom

        // g2d.setColor(Color.BLACK);
        // String zoomText = String.format("Zoom: %.2f%%", scale * 100);
        // g2d.drawString(zoomText, getWidth() - 100, getHeight() - 10);

        // Bouton pour rétablir le zoom
        JButton resetZoomButton = new JButton("Réinitialiser le zoom");
        resetZoomButton.addActionListener(e -> graphPanel.resetZoom());
        JPanel controlPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        controlPanel.add(resetZoomButton);
        add(controlPanel, BorderLayout.SOUTH);

        // Étiquette pour afficher le zoom
        zoomLabel = new JLabel("Zoom: 100.00%");
        zoomLabel.setHorizontalAlignment(SwingConstants.RIGHT);
        add(zoomLabel, BorderLayout.NORTH);

        // Exemple de graphe
        Loader.loadGraphFromFile(graphPanel, "resources/Auvergne-Rhône-Alpes.json");
        Loader.loadGraphFromFile(graphPanel, "resources/Bourgogne-Franche-Comte.json");
        Loader.loadGraphFromFile(graphPanel, "resources/Bretagne.json");
        Loader.loadGraphFromFile(graphPanel, "resources/Centre-Val_de_Loire.json");
        Loader.loadGraphFromFile(graphPanel, "resources/Corse.json");
        Loader.loadGraphFromFile(graphPanel, "resources/GrandEst.json");
        Loader.loadGraphFromFile(graphPanel, "resources/HautDeFrance.json");
        Loader.loadGraphFromFile(graphPanel, "resources/Ile-de-France.json");
        Loader.loadGraphFromFile(graphPanel, "resources/Normandie.json");
        Loader.loadGraphFromFile(graphPanel, "resources/NouvelleAquitaine.json");
        Loader.loadGraphFromFile(graphPanel, "resources/Occitanie.json");
        Loader.loadGraphFromFile(graphPanel, "resources/Pays_de_la_Loire.json");
        Loader.loadGraphFromFile(graphPanel, "resources/Provence_Alpes_Cote_Azur.json");
    }

    public void updateZoomLabel(double zoomLevel) {
        zoomLabel.setText(String.format("Zoom: %.2f%%", zoomLevel * 100));
    }
}