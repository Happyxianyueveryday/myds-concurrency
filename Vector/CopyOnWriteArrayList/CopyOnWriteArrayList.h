import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;


public class CopyOnWriteMap<K, V> implements Map<K, V>, Cloneable{

    private volatile Map<K, V> internalMap;
    
    public CopyOnWriteMap() {
        internalMap = new HashMap<K, V>(100);//初始大小应根据实际应用来指定
    }
    
    @Override
    public V put(K key, V value) {
        synchronized (this) {
            Map<K, V> newMap = new HashMap<K, V>(internalMap);//复制出一个新HashMap
            V val = newMap.put(key, value);//在新HashMap中执行写操作
            internalMap = newMap;//将原来的Map引用指向新Map
            return val;
        }
    }
    
    @Override
    public void putAll(Map<? extends K, ? extends V> m) {
        synchronized (this) {
            Map<K, V> newMap = new HashMap<K, V>(internalMap);
            newMap.putAll(m);
            internalMap = newMap;
        }
        
    }
    
    @Override
    public V get(Object key) {
        V result = internalMap.get(key);
        return result;
    }
    ......//other methods inherit from interface Map
}
