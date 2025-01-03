public class Node<T> {
    T _name;
    int _x, _y;

    public Node(T name, int x, int y) {
        this._name = name;
        this._x = x;
        this._y = y;
    }

    public int getDistance(Node other) {
        // Conversion des coordonnées x et y en latitude et longitude
        double thisLongitude = this._x / 100.0 - 5;
        double thisLatitude = -this._y / 100.0 + 50;
        double otherLongitude = other._x / 100.0 - 5;
        double otherLatitude = -other._y / 100.0 + 50;

        // Rayon moyen de la Terre en kilomètres
        final double EARTH_RADIUS = 6371.0;

        // Conversion des degrés en radians
        double lat1Rad = Math.toRadians(thisLatitude);
        double lon1Rad = Math.toRadians(thisLongitude);
        double lat2Rad = Math.toRadians(otherLatitude);
        double lon2Rad = Math.toRadians(otherLongitude);

        // Différences des coordonnées
        double deltaLat = lat2Rad - lat1Rad;
        double deltaLon = lon2Rad - lon1Rad;

        // Formule de haversine
        double a = Math.sin(deltaLat / 2) * Math.sin(deltaLat / 2) + Math.cos(lat1Rad) * Math.cos(lat2Rad) * Math.sin(deltaLon / 2) * Math.sin(deltaLon / 2);
        double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));

        // Calcul de la distance
        return (int) (EARTH_RADIUS * c);
    }
}