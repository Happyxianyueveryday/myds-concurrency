package list;

/**
 * @author krzykrucz
 */
public interface ConcurrentList<E> {
    int size();

    boolean isEmpty();

    boolean contains(Object o);

    boolean remove(Object o);

    boolean add(E e);
}
