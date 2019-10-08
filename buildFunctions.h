

TrieNode *createTrienode(char c, TrieNode *parent)
{
  TrieNode *node = (TrieNode*)malloc(sizeof(TrieNode));
  node->c = c;
  node->parent = parent;
  node->children = NULL;
  node->isWord=0;
  return node;
}

void addChild(TrieNode* parent, TrieNode* node)
{
	NodeList* newChild = (NodeList*)malloc(sizeof(NodeList));
	newChild->node = node;
	newChild->next = NULL;

	if(!parent->children)
	{
		parent->children = newChild;
	}
	
}

void destroyTrienode(TrieNode *node)
{
  NodeList* prevChild;

  if(node==NULL)
  {
      return;
  }

  while(node->children)
  {
  	prevChild = node->children;
  	node->children = node->children->next;
      destroyTrienode(prevChild->node);
      free(prevChild);
  }

  free(node);
  return;
}

TrieNode* pathType(TrieNode* root, TrieNode* currentNode, char* name)
{
	if(name[0] == '/')
	{
		return root;
	}
	else
	{
		return currentNode;
	}
}


void makeDirectory(TrieNode* currentNode, char name[100], char type, FILE* error)
{
	TrieNode *newNode, *parent = currentNode;
	NodeList *child, *newChild, *prevChild = NULL;
	char cpyName[100];
	int i, j;

	if(isFile(currentNode, name))
	{
		removeLastChar(name);
		fprintf(error, "%s: already exists\n", name);
		return;
	}

	strcpy(cpyName, name);
	i = strlen(cpyName) - 2;

	if(i > 0)
	{
		while(cpyName[i] != '/' && i > 0)
		{
			i--;
		}		
	}

	cpyName[i] = '\0';

	if(isFile(currentNode, cpyName) == 0 && strlen(cpyName) > 1 && i>0)
	{
		removeLastChar(name);
		fprintf(error, "%s: No such file or directory\n", name);
		return;
	}

	if(name[0] == '.' && name[1] == '/')
	{
		for(i=2; i<strlen(name); i++)
		{
			cpyName[i-2] = name[i];
		}
		cpyName[i-2] = '\0';

		strcpy(name, cpyName);
	}

	while(name[0] == '.' && name[1] == '.')
	{
		currentNode = changeDirectory(currentNode, "../", error);

		for(i=3; i<strlen(name); i++)
		{
			cpyName[i-3] = name[i];
		}
		cpyName[i-3] = '\0';

		strcpy(name, cpyName);
	}

	if(name[0] == '/')
	{
		for(i=1; i<strlen(name); i++)
		{
			cpyName[i-1] = name[i];
		}
		cpyName[i-1] = '\0';

		strcpy(name, cpyName);
	}

	child = currentNode->children;

	if(type == 'd')
	{
		name[strlen(name)+1] = '\0';
		name[strlen(name)] = '/';
	}
	else
	{
		name[strlen(name)+1] = '\0';
		name[strlen(name)] = '*';
	}
	
	if(child)
	{
		i = 0;
		while(child && i < strlen(name))
		{
			prevChild = NULL;

			while(child->node->c < name[i] && child->next)
			{
				prevChild = child;
				child = child->next;
			}

			if(child->node->c != name[i])
			{
				newNode = createTrienode(name[i], parent);
				newChild = (NodeList*)malloc(sizeof(NodeList));
				newChild->node = newNode;

				if(child->node->c > name[i])
				{
					if(prevChild)
					{
						prevChild->next = newChild;
						newChild->next = child; 					
					}
					else
					{
						newChild->next = child;
						parent->children = newChild;
					}					
				}
				else
				{
					child->next = newChild;
					newChild->next = NULL;
				}

				prevChild = NULL;
				parent = newNode;

				for(j=i+1; j<strlen(name); j++)
				{
					newNode = createTrienode(name[j], parent);
					addChild(parent, newNode);
					parent = newNode;
				}
				newNode->isWord = 1;

				break;
				
			}
			else
			{
				i++;
				parent = child->node;
				child = child->node->children;
				if(!child)
				{
					for(; i<strlen(name); i++)
					{
						newNode = createTrienode(name[i], parent);
						addChild(parent, newNode);
						parent = newNode;
					}
					newNode->isWord = 1;

					break;
				}
			}

		}
	}
	else
	{
		for(i=0; i<strlen(name); i++)
		{
			newNode = createTrienode(name[i], parent);
			addChild(parent, newNode);
			parent = newNode;
		}
		newNode->isWord = 1;

	}
	//printf("%c %c\n", currentNode->c, currentNode->children->node->c);
}

void removeChild(TrieNode* parent, TrieNode* childNode)
{
	NodeList* child = parent->children, *prevChild = NULL;

	while(child->node != childNode)
	{
		prevChild = child;
		child = child->next;
	}

	if(!prevChild)
	{
		parent->children = child->next;
		free(child->node);
		free(child);
	}
	else
	{
		prevChild->next = child->next;
		free(child->node);
		free(child);
	}

}

void removeDirectory(TrieNode *currentNode, char name[100], FILE* error)
{
	TrieNode *parent, *node = changeDirectory(currentNode, name, error);
	int i, charCount = 0, prevDirectory, directory = 0;
	char location[100];

	if(node == NULL)
	{
		return;
	}

	if(node->children)
	{
		name[strlen(name)-1] = '\0';
		/*if(name[0] != '/')
		{
			char path[] = "./";
			strcat(path, name);
			strcpy(name, path);
		}*/
		removeLastChar(name);
		fprintf(error, "%s: Directory not empty\n", name);

		return;
	}

	if(name[strlen(name)-1] != '/')
	{
		name[strlen(name)+1] = '\0';
		name[strlen(name)] = '/';
	}

	for(i=0; i<strlen(name); i++)
	{
		if(name[i] == '/')
		{
			charCount++;
			prevDirectory = directory;
			directory = i;
		}
	}

	if(name[0] == '/')
	{
		charCount--;
	}
	charCount--;

	if(charCount > 0)
	{
		prevDirectory++;
		strncpy(location, name, prevDirectory);
		location[prevDirectory] = '\0';
		currentNode = changeDirectory(currentNode, location, error);
		//printf("%c\n", currentNode->parent->c);
	}

	while(!node->children && node != currentNode)
	{
		parent = node->parent;
		removeChild(parent, node);
		node = parent;
	}  

}

void recursiveRemoval(TrieNode* currentNode, char name[100], FILE* error)
{	
	NodeList* child;
	TrieNode *parent, *node;
	int i = 0;

	if(isFile(currentNode, name) == 0)
	{
		removeLastChar(name);
		fprintf(error, "%s: No such file or directory\n", name);
		return;
	}

	if(isFile(currentNode, name) == 1)
	{
		node = currentNode;

		if(name[0] == '/')
		{
			i++;
		}


		while(i < strlen(name))
		{
			if(name[i] == '.')
			{
				i++;
				if(name[i] == '/')
				{
					i++;
					continue;
				}
				else if(name[i] == '.')
				{
					i += 2;
					node = node->parent;

					while(node->c != '/')
					{
						node = node->parent;					
					}
					continue;
				}
				else
				{
					i--;
				}
			}

			{
				child = node->children;
				
				while(child->node->c != name[i])
				{
					child = child->next;
				}

				node = child->node;
				child = child->node->children;
				i++;
			}
		}

		while(child->node->c != '*')
		{
			child = child->next;
		}

		node = child->node;

		while(node->c != '/' && !node->children)
		{
			parent = node->parent;
			removeChild(parent, node);
			node = parent;			
		}
	}
	else if(isFile(currentNode, name) > 1)
	{
		node = changeDirectory(currentNode, name, error);

		child = node->children;
		node->children = NULL;

		removeDirectory(currentNode, name, error);		
	}


	//free

}

void copyFile(TrieNode* root, TrieNode* currentNode, char location[100], char path[100], FILE* error)
{
	TrieNode* node;
	int directory, i = 0;

	node = pathType(root, currentNode, path);

	if(strlen(path) > 1)
	{
		if(isFile(node, path) == 0 || isFile(node, path) == 1)
		{
			removeLastChar(path);
			fprintf(error, "%s: No such file or directory\n", path);
			return;
		}
	}

	node = pathType(root, currentNode, location);

	if(isFile(node, location) == 0)
	{
		removeLastChar(location);
		fprintf(error, "%s: No such file or directory\n", location);
		return;
	}

	if(path[strlen(path)-1] != '/')
	{
		path[strlen(path)+1] = '\0';
		path[strlen(path)] = '/';
	}

	if(location[0] != '/' && location [0] != '.')
	{
		char cpyLocation[100];
		cpyLocation[0] = '.';
		cpyLocation[1] = '/';

		for(i=0; i<strlen(location); i++)
		{
			cpyLocation[i+2] = location [i];
		}

		cpyLocation[i+2] = '\0';
		strcpy(location, cpyLocation);
	}

	for(i=0; i < strlen(location); i++)
	{
		if(location[i] == '/')
		{
			directory = i;
		}
	}

	strcat(path, location+directory+1);

	node = pathType(root, currentNode, path);

	makeDirectory(node, path, 'f', error);

}

void moveFile(TrieNode* root, TrieNode* currentNode, char location[100], char path[100], FILE* error)
{
	TrieNode* node;

	node = pathType(root, currentNode, location);

	if(isFile(node, location) == 0 || isFile(node, location) == 2)
	{
		removeLastChar(location);
		fprintf(error, "%s: No such file or directory\n", location);
		return;
	}

	node = pathType(root, currentNode, path);

	if(strlen(path) > 1)
	{
		removeLastChar(path);
		//printf("%s\n", path);
		if(isFile(node, path) == 0)
		{
			fprintf(error, "%s: No such file or directory\n", path);
			return;
		}
		else if(isFile(node, path) == 1)
		{
			fprintf(error, "%s: Not a directory\n", path);
			return;
		}
	}
	
	node = pathType(root, currentNode, location);

	copyFile(root, currentNode, location, path, error);

	if(location[0] == '/')
	{
		node = root;
	}
	else
	{
		node = currentNode;
	}

	recursiveRemoval(node, location, error);
}

/*
struct TrieNode *create_tree(FILE *file)
{
  struct TrieNode *root = create_trienode(' ', NULL);
  struct TrieNode *ptr = root;
  int character;
  int converted;
  int buffer;

  character = fgetc(file);
  buffer = fgetc(file);

  while(character != EOF)
  {
      character = tolower(character);

      if(isalpha(character))
      {
          converted = character - 97;
          if(ptr->children[converted] == NULL)
          {
              ptr->children[converted] = create_trienode(character, ptr);
          }
          ptr = ptr->children[converted];
      }
      if(ptr != root && (!isalpha(character) || buffer == EOF))
      {
          ptr->is_word = true;
          ptr = root;
      }

      character = buffer;
      buffer = fgetc(file);
  }

  return root;
}

*/
