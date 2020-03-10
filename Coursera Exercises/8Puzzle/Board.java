import java.util.Arrays;
import java.util.Iterator;
import java.util.NoSuchElementException;
import edu.princeton.cs.algs4.StdOut;

public class Board {

    /** Tab representing fields of board. */
    private final int[][] fields;


    //TODO what im going to do is create boardIterator with tab
    // of size for all neighbours of our board and thats all
    private class BoardIterable  implements Iterable<Board> {

        private final BoardList head;

        private class BoardList {

            private Board board;
            private BoardList next;


            BoardList() {
                board = null;
                next = null;
            }

            BoardList(final int[][] fields, final int row0, final int col0, final int rowEx, final int colEx) {

                // We do the exchange
                fields[row0][col0] = fields[rowEx][colEx];
                fields[rowEx][colEx] = 0;

                board = new Board(fields);
                next = null;

                fields[rowEx][colEx] = fields[row0][col0];
                fields[row0][col0] = 0;

            }


            void appendToEnd(final int[][] fields, final int row0, final int col0, final int rowEx, final int colEx) {

                if (board == null) {

                    BoardList tmp = new BoardList(fields, row0, col0, rowEx, colEx);
                    board = tmp.board;

                } else {

                    BoardList tmp = this;

                    while (tmp.next != null) {
                        tmp = tmp.next;
                    }

                    tmp.next = new BoardList(fields, row0, col0, rowEx, colEx);

                }

            }

        }


        private class BoardIterator implements Iterator<Board> {
            /** Pointer to place where we are at the list. */
            private BoardList current;

            BoardIterator() {
                current = head;
            }

            @Override
            public boolean hasNext() {
                return current != null;
            }

            @Override
            public Board next() {

                if (!hasNext()) {
                    throw new NoSuchElementException();
                }

                Board board = current.board;

                if (board == null) {
                    throw new IllegalArgumentException("Board at BoardList can not be null");
                }

                current = current.next;

                return board;
            }

            @Override
            public void remove() {
                throw new UnsupportedOperationException();
            }

        }


        BoardIterable() {

            int[][] fieldsCopy = fields;
            head = new BoardList();

            for (int i = 0; i < fields.length; i++) {
                for (int j = 0; j < fields.length; j++) {
                    // That is the place without element so we can move it.
                    if (fields[i][j] == 0) {

                        if (i < fieldsCopy.length - 1) {
                            // Append bottom neighbour
                            head.appendToEnd(fieldsCopy, i, j, i + 1, j);
                        }

                        if (i > 0) {
                            // Append top neighbour
                            head.appendToEnd(fieldsCopy, i, j, i - 1, j);
                        }

                        if (j > 0) {
                            // Append left neighbour
                            head.appendToEnd(fieldsCopy, i, j, i, j - 1);
                        }

                        if (j < fields.length - 1) {
                            // Append right neighbour
                            head.appendToEnd(fieldsCopy, i, j, i, j + 1);
                        }
                    }
                }
            }
        }

        @Override
        public Iterator<Board> iterator() {
            return new BoardIterator();
        }

    }



    /** Creates a board from an n-by-n array of tiles,
     *  where tiles[row][col] = tile at (row, col).
     *  @param tiles - tab representing fields of the board
     */
    public Board(int[][] tiles) {

        // Checking whether we did not receive null as parameter.
        if (tiles == null) {
            throw new IllegalArgumentException();
        }

        // Checking whether an array doesnt have a null entry.
        for (int[] tile : tiles) {

            if (tile == null) {
                throw new IllegalArgumentException();
            }

        }

        fields = new int[tiles.length][tiles.length];

        // Coping all enters of the array.
        for (int i = 0; i < tiles.length; i++) {
            fields[i] = Arrays.copyOf(tiles[i], tiles.length);
        }



    }


    /** String representation of this board.
     *
     * @return String representation of the board.
     */
    public String toString() {

        StringBuilder builder = new StringBuilder();
        builder.append(fields.length);

        for (int[] field : fields) {
            builder.append("\n");

            for (int j = 0; j < fields.length; j++) {
                builder.append(" ").append(field[j]);
            }

        }

        return builder.toString();

    }


    /** Board dimension n.
     * @return Board dimension n
     */
    public int dimension() {
        return fields.length;
    }


    /** Number of tiles out of place.
     * @return number of tiles out of place.
     */
    public int hamming() {

        int outOfPlace = 0;

        for (int i = 0; i < fields.length; i++) {
            for (int j = 0; j < fields.length; j++) {
                if (fields[i][j] != i * fields.length + j + 1 && fields[i][j] != 0) {
                    outOfPlace++;
                }
            }
        }

        return outOfPlace;
    }


    /** Sum of Manhattan distances between tiles and goal.
     * @return manhattan distance of the board.
     */
    public int manhattan() {

        int manhattanDistance = 0;

        for (int i = 0; i < fields.length; i++) {
            for (int j = 0; j < fields.length; j++) {
                if (fields[i][j] != i * fields.length + j + 1 && fields[i][j] != 0) {

                    // Computing were digit should be in the array.
                    int digitRow = (fields[i][j] - 1) / fields.length;
                    int digitCol = fields[i][j] - digitRow * fields.length - 1;

                    manhattanDistance += (Math.abs(digitRow - i) + Math.abs(digitCol - j));

                }
            }
        }

        return manhattanDistance;
    }

    /** Is this board the goal board?
     * @return true if board is goal bord.
     *         false otherwise.
     */
    public boolean isGoal() {
        return manhattan() == 0;
    }

    /** Does this board equal y?
     * @param y - object which we are comparing to.
     * @return true if our object is equal to y.
     */
    public boolean equals(Object y) {

        if (y == this) {
            return true;
        }

        if (y == null || getClass() != y.getClass()) {
            return false;
        }

        Board newY = (Board) y;

        return Arrays.deepEquals(fields, newY.fields);
    }

    /** All neighboring boards.
     * @return iterator over neighbours of the board.
     */
    public Iterable<Board> neighbors() {
        return new BoardIterable();
    }

    /** A board that is obtained by exchanging any pair of tiles.
     * @return board being twin of our board.
     */
    public Board twin() {

        if (fields.length <= 1) {
            throw new IllegalArgumentException("Array dimensions can not be less then 2");
        }

        int[][] copyArray = new int[fields.length][fields.length];

        for (int i = 0; i < fields.length; i++) {
            copyArray[i] = Arrays.copyOf(fields[i], fields.length);
        }

        int firstCol = 0;
        int secondCol = fields.length - 1;

        if (copyArray[0][0] == 0) {
            firstCol++;
        } else if (fields[fields.length - 1][fields.length - 1] == 0) {
            secondCol--;
        }

        copyArray[0][firstCol] = fields[fields.length - 1][secondCol];
        copyArray[fields.length - 1][secondCol] = fields[0][firstCol];

        return new Board(copyArray);
    }

    /** Unit testing (not graded).
     *
     * @param args tab of program arguments.
     */
    public static void main(String[] args) {

        int[][] a = new int[3][3];
        a[0] = new int[]{8, 1, 3};
        a[1] = new int[]{4, 0, 2};
        a[2] = new int[]{7, 6, 5};

        Board b = new Board(a);

        a[0][0] = 0;
        a[1][1] = 8;

        Board c = new Board(a);

        assert b.hamming() == 5;
        assert b.manhattan() == 10;


        Iterable<Board> iterable = b.neighbors();

        for (Board f: iterable) {
            StdOut.println(f);
        }

        assert b.equals(b);
        assert !b.equals(c);
        assert !b.isGoal();
        assert b.dimension() == 3;


        StdOut.println(c.twin());

    }

}
