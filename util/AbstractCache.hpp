#define MAX_CACHE_ENTRIES 256

template <typename K, typename V>
struct LinkedListNode {
  V * val;
  K * key;
  struct LinkedListNode * next;
  struct LinkedListNode * prev;
};






template <typename K, typename V>
class AbstractCache{
protected:
  struct LinkedListNode<K, V> * start;
  struct LinkedListNode<K, V> * end;
  int currentNumElements;
  int maxEntries;
public:
  AbstractCache(int);
  bool initialized();
  virtual V getEntry(K * key);
};

template <typename K, typename V>
AbstractCache<K, V>::AbstractCache<K, V>(int n){
  start = (struct LinkedListNode<K, V> *) malloc(sizeof(struct LinkedListNode<K, V>));
  end = start;
  currentNumElements = (start == NULL)?-1:0;
  maxEntries = (n > 0 && n < MAX_CACHE_ENTRIES)?n:MAX_CACHE_ENTRIES;
}

template <typename K, typename V>
bool AbstractCache<K, V>::initialized(){
  return currentNumElements > -1;
}
