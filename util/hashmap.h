#define MAX_LENGTH_IN_BIT 3
/* max 256 entries in hash map array*/

struct ValueListNode{
  void * pVal;
  void * pKey;
  struct ValueListNode * next;
  struct ValueListNode * prev;
};
typedef struct ValueListNode node;


struct HashMap{
  node ** buckets;
  int size;
};

typedef struct HashMap hashmap;


typedef int (* hashcode)(void *);
typedef int (* equals)(void *, void *);
int get_bucket(int, int BitLengthOfHashMap);
int hashmap_init(hashmap *, int);
void * hashmap_get(hashmap *, void * key,  hashcode, equals);
int hashmap_insert(hashmap *, void * key,  void * value,  hashcode, equals);
int hashmap_delete(hashmap *, void * key,  hashcode, equals);
