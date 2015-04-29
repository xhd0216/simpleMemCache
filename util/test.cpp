#include"thread_safe_LRU.cpp"

typedef ThreadSafeLRU<int, int> Cache;
int main(){
  Cache cache (100);
  cache.getCacheElement(200);
  return 0;
}
