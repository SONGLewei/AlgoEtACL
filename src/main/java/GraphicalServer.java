import javax.swing.*;
// import GraphFrame;

public class GraphicalServer {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            GraphFrame frame = new GraphFrame();
            frame.setVisible(true);
        });
    }
}