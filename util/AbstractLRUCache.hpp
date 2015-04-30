#include<unordered_map>
#include"AbstractCache.hpp"



/*
the key type K should be:
-- hashable
-- comparable using ==
 */
template <typename K, typename V>
class AbstractLRUCache : public AbstractCache<K, V>{
protected:
  unordered_map<K, LinkedListNode<K, V> *> umap;
};

