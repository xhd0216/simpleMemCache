#include"thread_safe_LRU.hpp"
#include<stdlib.h>
#include<iostream>
using namespace std;

template <typename K, typename V>
ThreadSafeLRU<K, V>::ThreadSafeLRU(int n){
  if(n<=0 || n > MAX_ELEMENT){
    maxElem = MAX_ELEMENT;
  }
  else{
    maxElem = n;
  }
  start = (struct LinkedListNode<V> *)malloc(sizeof(struct LinkedListNode<V>));
  end = start;
  currentNElem = 0;
  
}

template <typename K, typename V>
V * ThreadSafeLRU<K, V>::getCacheElement(K key){
  cout<<"got element!"<<endl;
  return NULL;
}

template <typename K, typename V>
V * ThreadSafeLRU<K, V>::getFromDB(K key){
  return NULL;
}
/*
typedef ThreadSafeLRU<int, int> Cache;
int main(){
  Cache cache (100);
  cache.getCacheElement(200);
  return 0;
}
*/
