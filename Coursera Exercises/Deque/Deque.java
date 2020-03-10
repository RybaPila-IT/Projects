import java.util.Iterator;
import java.util.NoSuchElementException;

public class Deque<Item> implements Iterable<Item> {

    /** Reference to the beginning of the queue. */
    private Node first;

    /** Reference to the end of the queue. */
    private Node last;

    /** Size of the queue. */
    private int size;

    /** Subclass representing element in our queue. */
    private final class Node {

        /** Value stored in the element. */
        private final Item value;

        /** Reference to the next element. If there is no next element then
         *  Node is null terminated. */
        private Node next;

        /** Reference to the previous element. If there is
         * no previous element then Node is null terminated.
         * <Item> - Generic.
         */
        private Node previous;

        /** Constructor of the queue element.
         * @param item value for the queue element.
         */
        private Node(final Item item) {

            value = item;
            next = null;
            previous = null;

        }

    }

    /** Constructs an empty deque. */
    public Deque() {

        first = null;
        last = null;
        size = 0;

    }

    /** @return true if queue is empty, false otherwise. */
    public boolean isEmpty() {
        return size == 0;
    }

    /** @return The number of items on the deque. */
    public int size() {
        return size;
    }

    /** Add the item to the front of the queue.
     * @param item element to ba added to the list.
     */
    public void addFirst(Item item) {

        if (item == null) {
            throw new IllegalArgumentException();
        }

        Node newFirst = new Node(item);
        newFirst.previous = first;

        if (first != null) {
            first.next = newFirst;
        } else {
            last = newFirst;
        }

        first = newFirst;
        size++;

    }

    /** Add the item to the back of the queue.
     * @param item element to be added to the list.
     */
    public void addLast(Item item) {

        if (item == null) {
            throw new IllegalArgumentException();
        }

        Node newLast = new Node(item);

        if (last != null) {
            last.previous = newLast;
        } else {
            first = newLast;
        }

        newLast.next = last;
        last = newLast;
        size++;

    }

    /** Remove and return The item from the front.
     * @return Item from the front of the list.
     */
    public Item removeFirst() {

        if (isEmpty()) {
            throw new NoSuchElementException();
        }

        Item i = first.value;
        first = first.previous;

        if (first != null) {
            first.next = null;
        } else {
            last = null; // Case where we have an empty queue
        }

        size--;

        return i;

    }

    /** Remove and return The item from the back.
     * @return element form the back of the list.
     */
    public Item removeLast() {

        if (isEmpty()) {
            throw new NoSuchElementException();
        }

        Item i = last.value;
        last = last.next;

        if (last != null) {
            last.previous = null;
        } else {
            first = null;
        }

        size--;

        return  i;

    }

    /** return an iterator over items in order from front to back. */
    @Override
    public Iterator<Item> iterator() {
        return new DequeIterator();
    }


    private final class DequeIterator implements Iterator<Item> {

        /** Current position of the iterator. */
        private Node current;

        private DequeIterator() {
            current = first;
        }

        @Override
        public boolean hasNext() {
            return current != null;
        }

        @Override
        public Item next() {

            if (!hasNext()) {
                throw new NoSuchElementException();
            }

            Item i = current.value;
            current = current.previous;
            return i;
        }

        @Override
        public void remove() {
            throw new UnsupportedOperationException();
        }


    }


    /** Unit testing (required).
     * @param args tab with program arguments.
     */
    public static void main(String[] args) {

        // Creating a Deque and constructor test.
        Deque<Integer> intDeque = new Deque<>();

        assert intDeque.isEmpty();

        // Small test for inserting.
        intDeque.addFirst(13);
        intDeque.addFirst(14);
        intDeque.addLast(15);

        assert !intDeque.isEmpty();
        assert intDeque.size() == 3;

        assert intDeque.removeFirst() == 14;
        assert intDeque.removeLast() == 15;
        assert intDeque.removeLast() == 13;

        assert intDeque.isEmpty();
        assert intDeque.size() == 0;

        // Bigger test for inserting.
        for (int i = 0; i < 10; i++) {
            intDeque.addFirst(i);
            assert intDeque.size() == i + 1;
        }

        for (int i = 9; i >= 0; i--) {
            assert intDeque.removeFirst() == i;
            assert intDeque.size() == i;

        }

        assert intDeque.isEmpty();
        assert intDeque.size() == 0;

        // Test for different inserting.
        for (int i = 2; i <= 20; i += 2) {
            intDeque.addLast(i);
            intDeque.addFirst(i);
            assert intDeque.size() == i;
        }

        for (int i = 20; i >= 2; i -= 2) {
            assert intDeque.size() == i;
            assert intDeque.removeFirst() == intDeque.removeLast();
        }

        assert intDeque.isEmpty();

        for (int i = 0; i < 10; i += 2) {
            intDeque.addLast(i);
        }

        assert intDeque.size() == 5;

        for (int iter: intDeque) {
            assert iter % 2 == 0;
        }

        while (!intDeque.isEmpty()) {
            assert intDeque.removeLast() % 2 == 0;
        }

        intDeque.addFirst(2);
        intDeque.addLast(3);

        Iterator<Integer> iterator = intDeque.iterator();

        assert iterator.hasNext();
        assert iterator.next() == 2;
        assert iterator.hasNext();
        assert iterator.next() == 3;
        assert !iterator.hasNext();

    }


}
