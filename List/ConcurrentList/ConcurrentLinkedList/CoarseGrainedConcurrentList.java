package list;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * @author krzykrucz
 */
public class CoarseGrainedConcurrentList<E> extends AbstractConcurrentList<E> implements ConcurrentList<E> {

    private Lock lock = new ReentrantLock();

    @Override
    public boolean contains(Object o) {

        lock.lock();
        for (Node<E> x = guard; x.next != null; x = x.next) {
            if (o.equals(x.next.item)) {
                lock.unlock();
                return true;
            }
        }
        lock.unlock();
        return false;
    }

    @Override
    public boolean remove(Object o) {

        lock.lock();

        for (Node<E> x = guard; x.next != null; x = x.next) {
            if (o.equals(x.next.item)) {
                unlink(x);
                lock.unlock();
                return true;
            }
        }
        lock.unlock();
        return false;
    }

    @Override
    public boolean add(E e) {

        Node<E> x = guard;
        lock.lock();

        while (x.next != null) {
            x = x.next;
        }
        linkLast(x, e);

        lock.unlock();
        return true;
    }

}
