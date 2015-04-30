#define MAX_LENGTH_IN_BIT 3
/* max 256 entries in hash map array*/

struct ValueListNode{
  void * pVal;
  void * pKey;
  struct ValueListNode * next;
  struct ValueListNode * prev;
};
typedef struct ValueListNode node;





typedef int (* hashcode)(void *);
typedef int (* equals)(void *, void *);
typedef int (* copykey)(void *, void *);
typedef int (* copyvalue)(void *, void *);

struct HashMap{
  node ** buckets;
  int size;
  /* pointer to functions */
  hashcode hash_func;
  equals eq_func;
  /* should need both: size of key/value and functions to copy key/value */
  copykey copy_key;
  copyvalue copy_value;
  int key_size;
  int value_size;
};

typedef struct HashMap hashmap;

int get_bucket(int, int);
hashmap * hashmap_init(int, hashcode, equals, int, int, copykey, copyvalue);
void * hashmap_get(hashmap *, void * key);
int hashmap_insert(hashmap *, void * key,  void * value);
int hashmap_delete(hashmap *, void * key);
int hashmap_destroy(hashmap *);
