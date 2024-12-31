import javax.swing.*;

public class GraphicalServer {
    private static GraphFrame frameInstance;

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            frameInstance = new GraphFrame();
            frameInstance.setVisible(true);
        });

        // Attendre l'initialisation complète de frameInstance
        while (frameInstance == null) {
            try {
                Thread.sleep(50); // Attendre brièvement pour éviter une boucle intensive
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        // Lancer le FileReceiver dans un thread séparé
        new Thread(() -> FileReceiver.receiveJson(frameInstance)).start();
    }

    public static GraphFrame getGraphFrameInstance() {
        return frameInstance;
    }
}