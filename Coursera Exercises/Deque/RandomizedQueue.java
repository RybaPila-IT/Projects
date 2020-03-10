import java.util.Iterator;
import java.util.NoSuchElementException;
import edu.princeton.cs.algs4.StdRandom;

public class RandomizedQueue<Item> implements Iterable<Item> {

    /** Items inside of the queue. */
    private Item[] items;
    /** Index of last free spot in the array. */
    private int index;


    /** Construct an empty randomized queue. */
    public RandomizedQueue() {
        items = (Item[]) new Object[2];
        index = 0;
    }

    /** Is the randomized queue empty?
      * @return true if queue is empty false otherwise.
     */
    public boolean isEmpty() {
        return index == 0;
    }

    /** @return the number of items on the randomized queue. */
    public int size() {
        return index;
    }

    /** Add the item.
     * @param item - item which will be added to the queue.
     */
    public void enqueue(Item item) {
        // Checking if item isn`t null
        if (item == null) {
            throw new IllegalArgumentException();
        }

        // Case where we have to resize the array
        if (index == items.length) {
            Item[] newItems = (Item[]) new Object[index * 2];

            for (int i = 0; i < index; i++) {
                newItems[i] = items[i];
            }

            items = newItems;
        }
        // Adding element into the array
        items[index++] = item;
    }

    /** Remove and return a random item.
     * @return Random item from the queue.
     */
    public Item dequeue() {

        if (isEmpty()) {
            throw new NoSuchElementException();
        }

        int position = StdRandom.uniform(index);
        Item i = items[position];
        items[position] = items[--index];
        items[index] = null;

        // Checking if we can shrink the array
        if (index <= items.length / 4 && items.length / 2 > 0) {

            Item[] newItems = (Item[]) new Object[items.length / 2];

            for (int j = 0; j < index; j++) {
                newItems[j] = items[j];
            }

            items = newItems;
        }

        return i;
    }

    /** Return a random item (but do not remove it).
     * @return a random item from the queue.
     */
    public Item sample() {

        if (isEmpty()) {
            throw new NoSuchElementException();
            }

        return items[StdRandom.uniform(index)];
    }

    /** Return an independent iterator over items in random order.
     * @return iterator to the queue.
     */
    public Iterator<Item> iterator() {
        return new RandomizedQueueIterator();
    }

    private final class RandomizedQueueIterator implements Iterator<Item> {

        /** Tab of representing visitation of the elements. */
        private boolean[] visited;
        /** Elements left to be visited. */
        private int left;

        private RandomizedQueueIterator() {
            visited = new boolean[index];
            left = index;
        }

        @Override
        public boolean hasNext() {
            return left != 0;
        }

        @Override
        public Item next() {

            if (left == 0) {
                throw new NoSuchElementException();
            }

            int place;

            do {
                place = StdRandom.uniform(visited.length);
            } while (visited[place]);

            visited[place] = true;
            left--;

            return items[place];

        }

        @Override
        public void remove() {
            throw new UnsupportedOperationException();
        }

    }

    /** Unit testing (required).
     * @param args - tab of program arguments.
     */
    public static void main(String[] args) {

        RandomizedQueue<Integer> randomInt = new RandomizedQueue<>();

        assert randomInt.isEmpty();

        // Small unit test for one item.
        randomInt.enqueue(1);

        assert !randomInt.isEmpty();
        assert randomInt.size() == 1;
        assert randomInt.dequeue() == 1;
        assert randomInt.isEmpty();
        assert randomInt.size() == 0;

        // Bigger test.
        for (int i = 0; i < 32; i++) {
            randomInt.enqueue(i);
        }

        assert randomInt.size() == 32;

        for (int i = 0; i < 24; i++) {
            assert randomInt.dequeue() < 32;
        }

        assert randomInt.size() == 8;

        for (int a: randomInt) {
            assert a <= 32;
        }

        while (!randomInt.isEmpty()) {
            assert randomInt.dequeue() < 32;
        }


        for (int i = 0; i < 64; i++) {
            randomInt.enqueue(i);
        }

        assert randomInt.size() == 64;

        while (randomInt.sample() != 62) {
            assert randomInt.sample() < 64;
        }


    }

}
