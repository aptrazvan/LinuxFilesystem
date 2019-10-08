typedef enum {false, true} bool;

typedef struct TrieNode TrieNode;

typedef struct NodeList
{
	struct TrieNode *node;
	struct NodeList *next;
} NodeList;

typedef struct TrieNode
{
  char c;
  struct TrieNode *parent;
  struct NodeList *children;
  short int isWord;
} TrieNode;
