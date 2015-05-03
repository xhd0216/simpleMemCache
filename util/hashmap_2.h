#define MAX_BUCKETS 256
#define INIT_NUM_BUCKETS 16
#define RESIZE_FACTOR 8
/* max 256 entries in hash map buckets*/
/* initially have 16 buckets */
/* enlarge to 8 times when hashmap_resize is called */
#define HASH_CODE_SIZE 15


struct HashCodeType{
  char hc [HASH_CODE_SIZE+1];
};
typedef struct HashCodeType hash_code_t;

struct ValueListNode{
  void * pVal;
  hash_code_t hc;//store hash code of the key
  struct ValueListNode * next;
  struct ValueListNode * prev;
};
typedef struct ValueListNode node;

typedef hash_code_t (* hashcode)(const void *);
typedef int (* copyvalue)(void **, const void *);
typedef int (* freevalue)(void *);

struct HashMap{
  node ** buckets;
  int size;
  /* pointer to functions */
  hashcode hash_func;
  copyvalue copy_value;
  freevalue free_value;
};
typedef struct HashMap hashmap;

int get_bucket(hash_code_t *, int);
hashmap * hashmap_create(int, hashcode, copyvalue, freevalue);
node * hashmap_get(hashmap *, void * key);/* return the node * */
int hashmap_insert(hashmap *, void * key,  void * value);
int hashmap_delete(hashmap *, void * key);
int hashmap_destroy(hashmap *);
hashmap * hashmap_resize(hashmap *);
