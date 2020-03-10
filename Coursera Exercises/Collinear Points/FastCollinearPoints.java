import java.util.Arrays;

public class FastCollinearPoints {

    private final LineSegment[] segments;


    /** Finds all line segments containing 4 or more points.
     * @param points - tab with points at our plane.
     */
    public FastCollinearPoints(Point[] points) {

        checkPoints(points);

        LineSegment[] segmentsTmp = new LineSegment[0];
        Point[] pointDuplicates = Arrays.copyOf(points, points.length);

        // Sorting duplicates in order of the slope.
        for (int i = 0; i < points.length; i++) {
            // For each element we do a slope sort.
            Arrays.sort(pointDuplicates, points[i].slopeOrder());

            int begin = 1; // Points to the beginning of section.
            int end = 2;   // Points to the beginning od next section.

            while (end < points.length) {

                if (points[i].slopeTo(pointDuplicates[begin]) != points[i].slopeTo(pointDuplicates[end]) ||
                        (points[i].slopeTo(pointDuplicates[begin]) == points[i].slopeTo(pointDuplicates[end]) && end + 1 == points.length)) {

                    // Corner case
                    if (points[i].slopeTo(pointDuplicates[begin]) == points[i].slopeTo(pointDuplicates[end])) {
                        end++;
                    }


                    // Case where we had at least 4 collinear points.
                    if (end - begin + 1 >= 4) {
                        // Sorting points by coordinate.
                        // At the beginning there is the smallest point.

                        Arrays.sort(pointDuplicates, begin, end, Point::compareTo);

                        // We have to check if we already didnt save that line segment.
                        // We are adding only if we are the smallest element in the segment
                        if (points[i].compareTo(pointDuplicates[begin]) < 0) {
                            LineSegment newSegment = new LineSegment(points[i], pointDuplicates[end - 1]);
                            segmentsTmp = Arrays.copyOf(segmentsTmp, segmentsTmp.length + 1);
                            segmentsTmp[segmentsTmp.length - 1] = newSegment;
                        }

                    }

                    begin = end;

                }
                // If we are still collinear we increment end.
                end++;

            }

        }

        segments = segmentsTmp;
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

        Point[] tmp = Arrays.copyOf(points, points.length);
        Arrays.sort(tmp, Point::compareTo);

        for (int i = 1; i < tmp.length; i++) {
            if (tmp[i - 1].slopeTo(tmp[i]) == Double.NEGATIVE_INFINITY) {
                throw new IllegalArgumentException();
            }
        }

    }


    /** The number of line segments. */
    public int numberOfSegments() {
        return segments.length;
    }



    /** The line segments. */
    public LineSegment[] segments() {
        return Arrays.copyOf(segments, segments.length);
    }
}
