import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;


public class CopyOnWriteMap<K, V> implements Map<K, V>, Cloneable{

    private volatile Map<K, V> internalMap;
    
    public CopyOnWriteMap() {
        internalMap = new HashMap<K, V>(100);//��ʼ��СӦ����ʵ��Ӧ����ָ��
    }
    
    @Override
    public V put(K key, V value) {
        synchronized (this) {
            Map<K, V> newMap = new HashMap<K, V>(internalMap);//���Ƴ�һ����HashMap
            V val = newMap.put(key, value);//����HashMap��ִ��д����
            internalMap = newMap;//��ԭ����Map����ָ����Map
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
