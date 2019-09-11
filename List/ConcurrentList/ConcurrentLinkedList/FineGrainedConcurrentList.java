package list;

/**
 * @author krzykrucz
 */
public class FineGrainedConcurrentList<E> extends AbstractConcurrentList<E> implements ConcurrentList<E> {

    @Override
    public boolean contains(Object o) {

        Node<E> x = guard;
        x.lock.lock();

        while (x.next != null) {

            x.next.lock.lock();
            if (o.equals(x.next.item)) {

                return true;
            }
            x.lock.unlock();
            x = x.next;
        }
        x.lock.unlock();

        return false;
    }

    @Override
    public boolean remove(Object o) {

        Node<E> x = guard;
        x.lock.lock();

        while (x.next != null) {

            x.next.lock.lock();
            if (o.equals(x.next.item)) {
                unlink(x);
                return true;
            }
            x.lock.unlock();
            x = x.next;
        }
        x.lock.unlock();

        return false;
    }

    @Override
    public boolean add(E e) {

        Node<E> x = guard;

        x.lock.lock();
        while (x.next != null) {
            x.lock.unlock();
            x = x.next;
            x.lock.lock();
        }
        linkLast(x, e);

        x.lock.unlock();
        return true;
    }

}
