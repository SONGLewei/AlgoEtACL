// import Node;

public class Edge<S,T> {
    Node<T> _start;
    Node<T> _end;
    int _weight;

    public Edge(Node<T> start, Node<T> end, int weight) {
        this._start = start;
        this._end = end;
        this._weight = weight;
    }
}