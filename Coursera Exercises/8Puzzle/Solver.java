import java.util.Comparator;
import java.util.Iterator;
import java.util.NoSuchElementException;
import edu.princeton.cs.algs4.MinPQ;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.In;


public class Solver {


    private final boolean isPossibleToSolve;
    private final int minimumSteps;
    private final BoardChangeList path;
    // Potrzebna jakaś struktura żeby odtwarzać drogę. Pomysł:
    // drzewo jak w poleceniu ale że ma wskazanie na ojca tylko.
    // jak mamy board typu goal to idziemy po rodzicach i rekurencyjnie
    // zwracamy np listę robiąc na niej flipa. Otrzymamy wtedy
    // te plansze w odpowiedniej kolejności.

    /** BoardChangeList. */
    private class BoardChangeList implements Iterable<Board> {

        private final Board board;
        private final int currentMoves;
        private final int manhattanDistance;
        private final BoardChangeList father;

        BoardChangeList(final Board b, final BoardChangeList f, final int cM) {
            board = b;
            father = f;
            currentMoves = cM;
            manhattanDistance = b.manhattan();
        }

        private BoardChangeList reverseOrder(final BoardChangeList tail) {

            BoardChangeList ourStep = new BoardChangeList(board, tail, currentMoves);


            if (father == null) {
                return ourStep;
            }

            return father.reverseOrder(ourStep);
        }


        @Override
        public Iterator<Board> iterator() {
            return new BoardChangeListIterator();
        }


        private class BoardChangeListIterator implements Iterator<Board> {

            private BoardChangeList step;


            BoardChangeListIterator() {
                step = path.reverseOrder(null);
            }


            @Override
            public boolean hasNext() {
                return step != null;
            }

            @Override
            public Board next() {

                if (!hasNext()) {
                    throw new NoSuchElementException();
                }

                BoardChangeList tmp = step;
                step = step.father;

                return tmp.board;
            }

            @Override
            public void remove() {
                throw new UnsupportedOperationException();
            }
        }


    }


    private class ManhattanComparator implements Comparator<BoardChangeList> {

        @Override
        public int compare(final BoardChangeList b1, final BoardChangeList b2) {
            return (b1.manhattanDistance + b1.currentMoves) - (b2.manhattanDistance + b2.currentMoves);
        }
    }

    private void searchStep(MinPQ<BoardChangeList> queue) {

        BoardChangeList minimal = queue.delMin();

        for (Board neighbour: minimal.board.neighbors()) {

            if (minimal.father == null || !neighbour.equals(minimal.father.board)) {
                queue.insert(new BoardChangeList(neighbour, minimal, minimal.currentMoves + 1));
            }
        }
    }


    // Constructor works fine.
    // find a solution to the initial board (using the A* algorithm)
    public Solver(Board initial) {

        if (initial == null) {
            throw new IllegalArgumentException();
        }

        MinPQ<BoardChangeList> movesWithBoard = new MinPQ<>(new ManhattanComparator());
        MinPQ<BoardChangeList> movesWithTwin = new MinPQ<>(new ManhattanComparator());

        movesWithBoard.insert(new BoardChangeList(initial, null, 0));
        movesWithTwin.insert(new BoardChangeList(initial.twin(), null, 0));

        while (!movesWithBoard.min().board.isGoal() && !movesWithTwin.min().board.isGoal()) {
            searchStep(movesWithBoard);
            searchStep(movesWithTwin);
        }

        if (movesWithBoard.min().board.isGoal()) {
            path = movesWithBoard.min();
            minimumSteps = path.currentMoves;
            isPossibleToSolve = true;
        } else {
            isPossibleToSolve = false;
            minimumSteps = -1;
            path = null;
        }


    }

    // is the initial board solvable? (see below)
    public boolean isSolvable() {
        return isPossibleToSolve;
    }

    // min number of moves to solve initial board
    public int moves() {
        return minimumSteps;
    }

    // sequence of boards in a shortest solution
    public Iterable<Board> solution() {
        return isSolvable() ? path.reverseOrder(null) : null;
    }

    public static void main(String[] args) {

        // create initial board from file
        In in = new In(args[0]);
        int n = in.readInt();
        int[][] tiles = new int[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                tiles[i][j] = in.readInt();
        Board initial = new Board(tiles);

        // solve the puzzle
        Solver solver = new Solver(initial);

        // print solution to standard output
        if (!solver.isSolvable())
            StdOut.println("No solution possible");
        else {
            StdOut.println("Minimum number of moves = " + solver.moves());
            for (Board board : solver.solution())
                StdOut.println(board);
        }
    }

}
