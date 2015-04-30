#include"AbstractLRUCache.hpp"

template <typename K, typename V>
class AbstractThreadSafeLRUCache : public AbstractLRUCache<K, V>{
protected:
  virtual V getFromBack(K *);
  

};

template <typename K, typename V>
AbstractThreadSafeLRUCache::getEntry(K * key){
  //read_lock lock here
  if(umap.find(*key) != umap.end()){
    //cache hit!
    return *(umap[*key]->val);
  }
  //read_lock release here
  
  V r = getFromBack(key);
  LinkedListNode<K, V> * temp = (LinkedListNode<K, V> *) malloc(sizeof(LinkedListNode<K, V>));
  temp->val = &r;
  temp->key = key;
  //write lock here
  if(umap.find(*key) != umap.end()){
    free(umap[*key]->val);
  }


  //release write lock here
  return r;
  
}
