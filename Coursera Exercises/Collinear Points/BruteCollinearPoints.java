import java.util.Arrays;

public class BruteCollinearPoints {

    /** Tab with line segments found. */
    private LineSegment[] segments;

    /** Finds all line segments containing 4 points.
     *
     * @param points - tab of points from which we are creating line segments.
     */
    public BruteCollinearPoints(Point[] points) {

        segments = new LineSegment[0];

        checkPoints(points);

        for (int p = 0; p < points.length; p++) {
            // Tab in which are saved indexes of points in points array to which p is connected
            boolean[] connectedToP = new boolean[points.length];

            for (int q = p + 1; q < points.length; q++) {
                for (int r = q + 1; r < points.length; r++) {
                    // Faster breaking, no need to iterate if 3 points are not collinear.
                    if (points[p].slopeTo(points[q]) != points[p].slopeTo(points[r])) {
                        continue;
                    } else {

                        for (int s = r + 1; s < points.length; s++) {
                            // Case when we have 4 collinear points
                            if (points[p].slopeTo(points[q]) == points[p].slopeTo(points[s])) {

                                if (!connectedToP[s]) {

                                    Point[] collinearPoints = new Point[4];

                                    collinearPoints[0] = points[p];
                                    collinearPoints[1] = points[r];
                                    collinearPoints[2] = points[q];
                                    collinearPoints[3] = points[s];

                                    Arrays.sort(collinearPoints, 0, 4);

                                    addLineSegment(collinearPoints[0], collinearPoints[3]);
                                    connectedToP[s] = true;
                                }

                            }

                        }

                    }
                }
            }
        }

    }


    private void checkPoints(final Point[] points) {

        // Checking if points isn`t null
        if (points == null) {
            throw new IllegalArgumentException();
        }

        // Checking for duplicates and if any entry of the array isn`t null
        for (int i = 0; i < points.length; i++) {
            if (points[i] == null) {
                throw new IllegalArgumentException();
            }
        }

        for (int i = 0; i < points.length; i++) {
            for (int j = i + 1; j < points.length; j++) {
                if (points[i].slopeTo(points[j]) == Double.NEGATIVE_INFINITY) {
                    throw new IllegalArgumentException();
                }
            }
        }
    }

    private void addLineSegment(final Point p, final Point s) {

        segments = Arrays.copyOf(segments, segments.length + 1);
        segments[segments.length - 1] = new LineSegment(p, s);

    }

    /** The number of line segments.
     *
     * @return number of line segments.
     */
    public int numberOfSegments() {
        return segments.length;
    }

    /** The line segments.
     *
     * @return Line segments in brute force algorithm.
     */
    public LineSegment[] segments() {
        return Arrays.copyOf(segments, segments.length);
    }



}
