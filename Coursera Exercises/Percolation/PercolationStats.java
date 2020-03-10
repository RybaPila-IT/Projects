import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;
import edu.princeton.cs.algs4.StdOut;

public class PercolationStats {

    /** Parameter used in computing deviations. */
    private static final double T = 1.96;
    /** Here are stored results of percolations. */
    private double[] results;


    /** perform independent trials on an n-by-n grid.
     *
     * @param n - size of grid to perform symulation on.
     * @param trials - number of repetition of experiment.
     */
    public PercolationStats(int n, int trials) {

        if (n <= 0 || trials <= 0) {
            throw new IllegalArgumentException();
        }

        results = new double[trials];

    }

    /** sample mean of percolation threshold.
     * @return mean.
     */
    public double mean() {
        return StdStats.mean(results);
    }

    /** sample standard deviation of percolation threshold.
     * @return standard deviation.
     * */
    public double stddev() {
        return StdStats.stddev(results);
    }

    /** low endpoint of 95% confidence interval.
     * @return low endpoint.
     * */
    public double confidenceLo() {
        return StdStats.mean(results) - (T * StdStats.stddev(results)) / Math.sqrt(results.length);
    }

    /** high endpoint of 95% confidence interval.
     * @return high endpoint.
     * */
    public double confidenceHi() {
        return StdStats.mean(results) + (T * StdStats.stddev(results)) / Math.sqrt(results.length);
    }

    /** test client (see below).
     * @param args - tab of input arguments for program.
     */
    public static void main(String[] args) {

        int n = Integer.parseInt(args[0]);
        int trials = Integer.parseInt(args[1]);
        PercolationStats stats = new PercolationStats(n, trials);

        for (int i = 0; i < trials; i++) {

            Percolation p = new Percolation(n);

            while (!p.percolates()) {

                int row = StdRandom.uniform(n) + 1;
                int col = StdRandom.uniform(n) + 1;

                p.open(row, col);
            }

            stats.results[i] = (double) p.numberOfOpenSites() / (n * n);
        }

        StdOut.println("mean                    = " + StdStats.mean(stats.results));
        StdOut.println("stddev                  = " + StdStats.stddev(stats.results));
        StdOut.println("95% confidence interval = [" + stats.confidenceLo() + ", " + stats.confidenceHi() + "]");
    }


}
