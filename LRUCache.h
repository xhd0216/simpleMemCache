#define HASH_CODE_BYTES 23



struct HashCode{
  char c [HASH_CODE_BYTES + 1];
};
typedef struct HashCode hash_code_t;

struct LinkedListNode{
  void * value;
  int reading_by;//indicate that this node is being read by how many other threads
  struct LinkedListNode next;
  struct LinkedListNode prev;
};
typedef struct LinkedListNode linked_list_node;

struct HashMapLinkedListNode{
  hash_code_t key;//key's hash code
  linked_list_node * ptrNode;//pointer to node in linked list
  struct HashMapLinkedListNode * next;
  struct HashMapLinkedListNode * prev;
};
typedef struct HashMapLinkedListNode hash_map_node;


/*
 * key_hash_func: returns a char array as a hash value
 * if the length of char array < HASH_CODE_BYTES, 
 * then 0s will be padded to the end
 */
typedef char * (* key_hash_func)(const void *);
/*
 * value_clone_function: clones value to heap and return its ptr
 * typical use:
 * struct ValueType{
 *   char c [30];
 *   int i;
 * };
 * void * ValueClone(void ** dest, const void * src){
 *   *dest = malloc(sizeof(ValueType)); //return a ptr to ValueType
 *   memcpy(*dest, src, sizeof(ValueType));
 *   return *dest;
 * }
 *
 * typical use 2:
 * struct ValueType{
 *   char * c;
 *   int i;
 * };
 * void * ValueClone(void ** dest, const void * src){
 *   //allocate the memory
 *   *dest = malloc(sizeof(ValueType)); 
 *   (*dest)->c = malloc(strlen(src->c) + 1);
 *   strncpy((*dest)->c, src->c, strlen(src->c) + 1);
 *   (*dest)->i = src->i;
 *   return *dest;
 * }
 */
typedef void * (* value_clone_func)(void **, const void *);

/*
maybe we can try this...later
 */
//typedef void * (* value_clone_func)(const void *);


struct HashMap{
  hash_map_node ** buckets;
  int num_buckets;
};
typedef struct HashMap hash_map_t;

struct LRUCache{
  hash_map_t * map;
  linked_list_node * list_start;
  linked_list_node * list_end;
  int current_size;
  int max_size;
  key_hash_func khf;
  value_clone_func vcf;
};
typedef struct LRUCache cache;

cache * cache_init(
		   int size_of_cache,
		   int num_of_buckets,
		   key_hash_func khf,
		   value_clone_func vcf
		   );
void * cache_get(cache * , const void * key);
int cache_insert(cache * , const void * key, const void * value);
int cache_destroy(cache *);




//typedef int (* key_equals_func)(const void *, const  void *);
//typedef int (* key_clone_func)(void *, const void *);
