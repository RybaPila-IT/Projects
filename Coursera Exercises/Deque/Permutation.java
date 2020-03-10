import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

public class Permutation {

    /** Main method described in specification.
     * @param args tab of input arguments for the program.
     */
    public static void main(String[] args) {

        int k = Integer.parseInt(args[0]);
        RandomizedQueue<String> randomString = new RandomizedQueue<>();

        while (!StdIn.isEmpty()) {
            randomString.enqueue(StdIn.readString());
        }

        for (String s : randomString) {

            if (k == 0) {
                break;
            } else {
                StdOut.println(s);
                k--;
            }

        }

    }

}
