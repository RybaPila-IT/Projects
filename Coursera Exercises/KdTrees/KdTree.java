import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.StdDraw;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;

public class KdTree {

    /** Root of the Kd Tree. */
    private Node root;
    /** Size of the Kd tree. */
    private int size;

    private class Node {

        /** Point stored at the node. */
        private final Point2D point;
        /** Reference to the left son of the node. */
        private Node leftSon;
        /** Reference to the right son of the node. */
        private Node rightSon;

        /** Constructs a node with specified parameters.
         *  Assumption: p is not null, checked in previous functions.
         * @param p - point which is going to be stored at the node.
         */
        Node(final Point2D p) {

            point = p;
            leftSon = null;
            rightSon = null;
        }

    }

    private void drawNode(final Node n, final byte b, final double fatherX, final double fatherY) {

        if (n != null) {

            byte next;

            // Drawing vertical red lines.
            if (b == 0) {

                StdDraw.setPenColor(StdDraw.RED);

                n.point.drawTo(new Point2D(n.point.x(), fatherY));

                if (fatherY > n.point.y()) {
                    n.point.drawTo(new Point2D(n.point.x(), 0));
                } else {
                    n.point.drawTo(new Point2D(n.point.x(), 1));
                }

                next = 1;
            // Drawing horizontal blue lines.
            } else {

                StdDraw.setPenColor(StdDraw.BLUE);

                n.point.drawTo(new Point2D(fatherX, n.point.y()));

                if (fatherX > n.point.x()) {
                    n.point.drawTo(new Point2D(0, n.point.y()));
                } else {
                    n.point.drawTo(new Point2D(1, n.point.y()));
                }

                next = 0;
            }

            // Drawing points as small circles.
            StdDraw.setPenColor(StdDraw.BLACK);
            StdDraw.filledCircle(n.point.x(), n.point.y(), 0.01);

            drawNode(n.leftSon, next, n.point.x(), n.point.y());
            drawNode(n.rightSon, next, n.point.x(), n.point.y());


        }

    }



    private Node addNewNode(final Point2D p, final Node n, final byte b) {

        if (n == null) {
            return new Node(p);
        }

        Comparator<Point2D> comparator;
        byte next;

        // Case when we compare points by x-coordinate
        // is when b == 0, when by y-coordinate b == 1.
        if (b == 0) {
            comparator = Point2D.X_ORDER;
            next = 1;
        } else {
            comparator = Point2D.Y_ORDER;
            next = 0;
        }

        if (comparator.compare(n.point, p) > 0) {
            n.leftSon = addNewNode(p, n.leftSon, next);
        } else {
            n.rightSon = addNewNode(p, n.rightSon, next);
        }

        return n;
    }


    private void addContainedPoints(List<Point2D> points, Node n, byte b, RectHV rect) {

        if (n != null) {

            // If rectangle contains point stored in node n
            // then we add it to the points set.
            if (rect.contains(n.point)) {
                points.add(n.point);
            }

            // Comparing by x-coordinate
            if (b == 0) {

                if (rect.xmin() < n.point.x()) {
                    addContainedPoints(points, n.leftSon, (byte) 1, rect);
                }

                if (rect.xmax() >= n.point.x()) {
                    addContainedPoints(points, n.rightSon, (byte) 1, rect);
                }
            // Else comparing by y-coordinate.
            } else {

                if (rect.ymin() < n.point.y()) {
                    addContainedPoints(points, n.leftSon, (byte) 0, rect);
                }

                if (rect.ymax() >= n.point.y()) {
                    addContainedPoints(points, n.rightSon, (byte) 0, rect);
                }

            }

        }

    }


    private Point2D champion(Node n, Point2D p, double distance, Point2D winner, byte b) {

        if (n != null) {

            // Update a winner and a distance if we have found closer point.
            if (n.point.distanceTo(p) < distance) {
                winner = n.point;
                distance = n.point.distanceTo(p);
            }

            // Comparing by x-coordinate.
            if (b == 0) {

                if (n.point.x() <= p.x()) {

                    Point2D possible1 = champion(n.rightSon, p, distance, winner, (byte) 1);

                    if (possible1.distanceTo(p) > p.x() - n.point.x()) {
                        return champion(n.leftSon, p, possible1.distanceTo(p), possible1, (byte) 1);
                    }

                    return possible1;

                } else {

                    Point2D possible1 = champion(n.leftSon, p, distance, winner, (byte) 1);

                    if (possible1.distanceTo(p) > n.point.x() - p.x()) {
                        return champion(n.rightSon, p, possible1.distanceTo(p), possible1, (byte) 1);
                    }

                    return possible1;

                }
            // Comparing by y-coordinate.
            } else {


                if (n.point.y() <= p.y()) {

                    Point2D possible1 = champion(n.rightSon, p, distance, winner, (byte) 0);

                    // Checking if there is a possibility that there is
                    // champion in second subtree.
                    if (possible1.distanceTo(p) > p.y() - n.point.y()) {
                        return champion(n.leftSon, p, possible1.distanceTo(p), possible1, (byte) 0);
                    }

                    return possible1;

                } else  {

                    Point2D possible1 = champion(n.leftSon, p, distance, winner, (byte) 0);

                    if (possible1.distanceTo(p) > n.point.y() - p.y()) {
                        return champion(n.rightSon, p, possible1.distanceTo(p), possible1, (byte) 0);
                    }

                    return possible1;

                }

            }

        }

        return winner;
    }



    /** Construct an empty set of points. */
    public KdTree() {
        root = null;
        size = 0;
    }

    /** Is the set empty?
     * @return True if set is empty. False otherwise.
     */
    public boolean isEmpty() {
        return root == null;
    }

    /** Number of points in the set.
     * @return Number of points in the set.
     */
    public int size() {
        return size;
    }

    /** Add the point to the set,
     *  if it is not already in the set.
     * @param p - point which is going to be added.
     */
    public void insert(Point2D p) {

        if (p == null) {
            throw new IllegalArgumentException();
        }

        if (!contains(p)) {
            root = addNewNode(p, root, (byte) 0);
            size++;
        }

    }

    /** Does the set contain point p?
     * @param p - point which set may contain.
     * @return True if set contains point p. False otherwise.
     */
    public boolean contains(Point2D p) {

        if (p == null) {
            throw new IllegalArgumentException();
        }

        Node tmp = root;
        byte b = 0;

        Comparator<Point2D> comparator;

        while (tmp != null) {

            if (tmp.point.compareTo(p) == 0) {
                return true;
            }

            if (b == 0) {
                comparator = Point2D.X_ORDER;
                b = 1;
            } else {
                comparator = Point2D.Y_ORDER;
                b = 0;
            }

            if (comparator.compare(tmp.point, p) > 0) {
                tmp = tmp.leftSon;
            } else {
                tmp = tmp.rightSon;
            }


        }

        return false;

    }

    /** Draw all points to standard draw.
     */
    public void draw() {

        StdDraw.setXscale(-0.1, 1.1);
        StdDraw.setYscale(-0.1, 1.1);

        RectHV frame = new RectHV(0, 0, 1, 1);
        frame.draw();

        drawNode(root, (byte) 0, 0, 1);
    }

    /** All points that are inside the rectangle,
     *  or on the boundary.
     * @param rect - rectangle in which we are looking for points.
     * @return Points contained by rectangle rect.
     */
    public Iterable<Point2D> range(RectHV rect) {

        if (rect == null) {
            throw new IllegalArgumentException();
        }

        LinkedList<Point2D> points = new LinkedList<>();

        addContainedPoints(points, root, (byte) 0, rect);

        return points;
    }


    /** A nearest neighbour in the set to point p.
     *  Null if the set id empty.
     * @param p - point to which we measure the distance.
     * @return Point with shortest distance to p.
     */
    public Point2D nearest(Point2D p) {

        if (p == null) {
            throw new IllegalArgumentException();
        }

        return champion(root, p, Double.POSITIVE_INFINITY, null, (byte) 0);
    }


    /** Unit testing of the methods.
     * @param args - tab with input string arguments.
     */
    public static void main(String[] args) {
        throw new UnsupportedOperationException();
    }
}
