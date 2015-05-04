# simpleMemCache
A pure C Implementation of MemCache

feature:
1. implement in c
2. hashmap that takes any structures pairs as (K, V)
3. thread safe (read-write) cache
4. replaces oldest entry in cache
5. hashmap has not implement any locks.

future change:
1. use socket
2. implement random replacement cache
3. LRU cache (hard)


To initialize cache:
cache * cache_create(int size,
		     hashcode key_type_hash_function,
		     cachevalueclone cache_value_clone_function,
		     cachevaluefree cache_value_free_function,
		     getfromothersource get_from_other_function);
		     
where:
size is the number of items hold in cache
key_type_hash_function: a (pointer of) function that hashes a K to hash code (a char array);
cache_value_clone_function: a (pointer of) function that copies the value to heap;
cache_value_clone_function: a (pointer of) function that frees a value object in heap; 
get_from_other_function: a (pointer of) function that, once cache misses, obtains V from other source (e.g., a DB)
