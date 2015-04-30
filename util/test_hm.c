#include"hashmap.c"


struct stype{
  char c [30];
  int v;
  int no_use;
};
typedef struct stype ss;

int KeyTypeEqual(void * k1, void * k2){
  ss * s1 = (ss *)k1;
  ss * s2 = (ss *)k2;
  if(s1->v != s2->v) return 0;
  char * c1 = s1->c;
  char * c2 = s2->c;
  int i = 0;
  while(c1 && c2 && i < 30){
    if(*c1 != *c2) return 0;
    if(*c1 == '\0') return 1;
    c1 = c1 + 1;
    c2 = c2 + 1;
    i = i+1;
  }
  return 1;
}

int KeyTypeHash(void * k){
  ss * s = (ss *) k;
  int r = s->v;
  int temp;
  for(int i = 0; i < 30; i++){
    if(s->c[i] == '\0') break;
    temp = ((1<<8)-1) & s->c[i];
    temp = temp << ((i%4)*8);
    r ^= temp;
  }
  return r;
}

struct vtype{
  int j;
};
typedef struct vtype vv;

void iterate(hashmap * h){
  if(!h) return;
  node * n;
  vv * tempv;
  ss * tempk;
  for(int i = 0; i < h->size; i++){
    printf("entry %d:\t", i);
    n = h->buckets[i];
    while(n){
      tempv = (vv*)n->pVal;
      tempk = (ss*)n->pKey;
      printf("(%d, %d)\t", tempk->v, tempv->j);
      n = n->next;
    }
    printf("\n");
  }
}

void helper(hashmap * e){
  ss a;
  vv v;
  strncpy(a.c, "backup", 30);
  a.v = 15;
  v.j = 35;
  /*will cause error: a, v are local/stack variables, 
   *when function quit, they will be delete
   *but hashmap still have a pointer pointing to it
   */
  hashmap_insert(e, &a, &v, &KeyTypeHash, &KeyTypeEqual);

}
int main(){
  hashmap hm;
  hashmap_init(&hm, 200);
  printf("initialized\n");
  ss a;
  /*ss b;
  ss c;
  ss d;*/
  strncpy(a.c, "hupu", 30);
  a.v = 10;
  /* b.c = "kkkkk";
  b.v = 10;
  c.c = "hupu";
  c.v = 10;
  d.c = "hupus";
  d.v = 10;*/
  vv jjj;
  jjj.j = 100;
  hashmap_insert(&hm, &a, &jjj, &KeyTypeHash, &KeyTypeEqual);
  /* hashmap_insert(b, sizeof(ss));
  hashmap_insert(c, sizeof(ss));
  hashmap_insert(d, sizeof(ss));*/
  printf("size of hash map: %d\n",hm.size);
  iterate(&hm);
  ss b;
  strncpy(b.c, "hupu", 30);
  b.v = 10;
  vv * hhh;
  //hhh.j = 90;
  hhh = (vv*)hashmap_get(&hm, &b, &KeyTypeHash, &KeyTypeEqual);
  printf("item got: %d\n", hhh->j);
  hashmap_delete(&hm, &b, &KeyTypeHash, &KeyTypeEqual);
  iterate(&hm);
  //helper(&hm);
  //free(hhh);
  //iterate(&hm);
  return 0;

}
