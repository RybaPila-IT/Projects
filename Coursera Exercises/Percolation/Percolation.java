import edu.princeton.cs.algs4.WeightedQuickUnionUF;


public class Percolation {

    /** Size of n x n grid. */
    private final int dim;
    /** Number of open sites. */
    private int openedFields;
    /** Tab representing sites. */
    private boolean[][] sites;
    /** Tab representing roots of sites. */
    private final WeightedQuickUnionUF elements;

    /** Creates n-by-n grid, with all sites initially blocked.
     * @param n - size of the grid dimension.
     * */
    public Percolation(int n) {

        if (n <= 0) {
            throw new IllegalArgumentException();
        }

        // All sites are set to false which means they are blocked
        dim = n;
        openedFields = 0;
        sites = new boolean[n][n];
        elements = new WeightedQuickUnionUF(n * n + 2);

        for (int i = 1; i <= dim; i++) {
            elements.union(0, i);
        }

    }


    private boolean correctNode(final int node) {

        return !(node > dim * dim + 1 || node <= 0);
    }


    private boolean correctNeighbour(final int opened, final int toConnect) {

        if (!correctNode(toConnect)) {
            return false; // checking if we are still in the array
        }
        if (toConnect == dim * dim + 1 && (opened - 1) / dim == dim - 1) {
            return true;
        }
        if ((toConnect - 1 == opened && opened % dim == 0) || (toConnect + 1 == opened && opened % dim == 1)) {
            return false; // checking if we have correct neighbour
        }

        return true;
    }




    private void union(final int opened, final int toConnect) {

        if (correctNeighbour(opened, toConnect) && (toConnect == dim * dim + 1 || isOpen((toConnect - 1) / dim + 1, (toConnect - 1) % dim + 1))) {
            elements.union(opened, toConnect);
        }

    }

    /** opens the site (row, col) if it is not open already.
     * @param row - number of row to be opened.
     * @param col - number of column to bo open.
     * */
    public void open(int row, int col) {

        if (!isOpen(row, col)) {

            sites[row - 1][col - 1] = true;
            openedFields++;

            final int index = (row - 1) * dim + col;

            union(index, index + 1);
            union(index, index - 1);
            union(index, index + dim);
            union(index, index - dim);

            if ((index - 1) / dim == dim - 1) {
                union(index, dim * dim + 1);
            }

        }


    }

    /** is the site (row, col) open?
     * @param col - number of column.
     * @param row - number of row.
     * @return true of site is open false otherwise.
     * */
    public boolean isOpen(int row, int col) {

        if (row > dim || col > dim || row <= 0 || col <= 0) {
            throw new IllegalArgumentException();
        }

        return sites[row - 1][col - 1];
    }

    /** is the site (row, col) full?
     * @param col - number of column.
     * @param row - number of row.
     * @return true if site is closed false otherwise.
     * */
    public boolean isFull(int row, int col) {
        int index = (row - 1)* dim + col;

        if (((index - 1) / dim) == (dim - 1)) {
            /** Corner case. */
            if (index == 1) {
                return isOpen(row, col);
            } else if ((index - 2) / dim == dim - 1 && index / dim == dim - 1) {
                return isOpen(row, col) && (isFull(row, col - 1) || isFull(row, col + 1) || isFull(row - 1, col));
            } else if ((index - 2) / dim == dim - 1) {
                return isOpen(row, col) && (isFull(row, col - 1) || isFull(row - 1, col));
            } else if ( index / dim == dim - 1) {
                return isOpen(row, col) && (isFull(row, col + 1) || isFull(row - 1, col));
            } else {
                return isOpen(row, col) && isFull(row - 1, col);
            }

        } else {
            return isOpen(row, col) && elements.connected(0, index);
        }
    }

    /** returns the number of open sites.
     * @return number of open sites.
     * */
    public int numberOfOpenSites() {
        return openedFields;
    }

    /** does the system percolate?
     * @return true if system percolates, false otherwise.
     * */
    public boolean percolates() {
        return elements.connected(0, dim * dim + 1);
    }

}
