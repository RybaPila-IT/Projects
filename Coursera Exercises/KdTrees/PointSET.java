import java.util.TreeSet;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;


public class PointSET {

    /** Points which are inside our point set. */
    private TreeSet<Point2D> points;

    /** Construct an empty set of points.
     */
    public PointSET() {
        points = new TreeSet<>();
    }

    /** Is the set empty?
     * @return true if set is empty. False otherwise.
     */
    public boolean isEmpty() {
        return points.isEmpty();
    }

    /** Number of points in the set.
     * @return Number of points in the set.
     */
    public int size() {
        return points.size();
    }

    /** Add the point to the set, if it is not already
     * in the set.
     * @param p - point which is going to be added to the set.
     */
    public void insert(Point2D p) {

        if (p == null) {
            throw new IllegalArgumentException();
        }

        points.add(p);
    }

    /** Does the set contain point p?
     * @param p - point which set may contain.
     * @return true if set contains the point. False otherwise.
     */
    public boolean contains(Point2D p) {

        if (p == null) {
            throw new IllegalArgumentException();
        }

        return points.contains(p);
    }

    /** Draw all points to standard draw.
     */
    public void draw() {

        for (Point2D p : points) {
            p.draw();
        }

    }

    /** All points that are inside the rectangle
     * or on the boundary.
     * @param rect - rectangle for which we want to find points inside.
     * @return Points which are inside the rectangle.
     */
    public Iterable<Point2D> range(RectHV rect) {

        if (rect == null) {
            throw new IllegalArgumentException();
        }

        TreeSet<Point2D> inside = new TreeSet<>();

        for (Point2D p : points) {

            if (rect.contains(p)) {
                inside.add(p);
            }

        }

        return inside;
    }

    /** A nearest neighbour in the set to point p.
     * Null if set is empty.
     * @param p - point for which we are looking the nearest neighbour.
     * @return Nearest neighbour of the point or null if set is empty.
     */
    public Point2D nearest(Point2D p) {

        if (p == null) {
            throw new IllegalArgumentException();
        }

        double distance = Double.POSITIVE_INFINITY;
        Point2D nearest = null;

        for (Point2D q : points) {

            if (p.distanceTo(q) < distance) {
                nearest = q;
                distance = p.distanceTo(q);
            }

        }

        return nearest;
    }

    /** Unit testing; optional.
     * @param args - tab of input strings.
     */
    public static void main(String[] args) {
        throw new UnsupportedOperationException();
    }

}
