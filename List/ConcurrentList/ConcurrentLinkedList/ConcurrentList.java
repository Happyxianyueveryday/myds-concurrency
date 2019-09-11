package list;

/**
 * @author pzh
 */
public interface ConcurrentList<E> {
    int size();

    boolean isEmpty();

    boolean contains(Object o);

    boolean remove(Object o);

    boolean add(E e);
}
