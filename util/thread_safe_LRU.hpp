#include<unordered_map>
#define MAX_ELEMENT 1024
using namespace std;



template <typename K, typename V>
class ThreadSafeLRU{
private:
  unordered_map<K, LinkedListNode<V> > map;
  
  int maxElem;//final
  int currentNElem;
  V * getFromDB(K key);
public:
  ThreadSafeLRU(int nMaxElement);
  V * getCacheElement(K key);

};
