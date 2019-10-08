void removeLastChar(char path[])
{
	if(path[strlen(path)-1] == '/' || path[strlen(path)-1] == '*')
	{
		path[strlen(path)-1] = '\0';
	}
}


int isFile(TrieNode* currentNode, char name[100])
{
	NodeList* child = NULL;
	TrieNode* node = currentNode;
	int i = 0, result = 0;

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
				if(!node->parent)
				{
					return 0;
				}

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

			if(!child)
			{
				//printf("%s: No such file or directory\n", name);
				return 0;
			}
			
			while(child->node->c != name[i] && child)
			{
				child = child->next;

				if(!child)
				{
					return 0;
				}
			}

			node = child->node;
			child = child->node->children;
			i++;
		}
	}

	while(child)
	{
		if(child->node->c == '*')
		{
			result++;
		}
		else if(child->node->c == '/')
		{
			result += 2;
		}
		child = child->next;
	}

	return result;
}

TrieNode* changeDirectory(TrieNode* currentNode, char name[100], FILE* error)
{
	NodeList* child;
	TrieNode* node = currentNode;
	int i = 0;

	if(isFile(currentNode, name) == 1)
	{
		removeLastChar(name);
		fprintf(error, "%s: Not a directory\n", name);
		return NULL;
	}

	if(name[strlen(name)-1] != '/')
	{
		name[strlen(name)+1] = '\0';
		name[strlen(name)] = '/';
	}

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
			}
			else
			{
				if(!currentNode->parent)
				{
					removeLastChar(name);
					fprintf(error, "%s: No such file or directory\n", name);
					return NULL;
				}
				
				i += 2;
				node = node->parent;

				while(node->c != '/')
				{
					node = node->parent;					
				}
			}
		}
		else
		{
			child = node->children;

			while(name[i] != '/')
			{
				if(!child)
				{
					removeLastChar(name);
					fprintf(error, "%s: No such file or directory\n", name);
					return NULL;
				}

				while(child->node->c != name[i] && child)
				{
					child = child->next;

					if(!child)
					{
						removeLastChar(name);
						fprintf(error, "%s: No such file or directory\n", name);
						return NULL;
					}
				}

				child = child->node->children;
				i++;
				if(name[i] == '/')
				{
					break;
				}
			}

			while(child->node->c != name[i])
			{
				child = child->next;
			}

			node = child->node;
			child = child->node->children;
			i++;
		}
		

	}

	return node;

}

void printTrienode(TrieNode* node, char buffer[100], FILE* output)
{
	NodeList* children = node->children;
	char path[100];

	if(node==NULL)
	{
	    return;
	}

	buffer[strlen(buffer)] = node->c;
	buffer[strlen(buffer)+1] = '\0';
	strcpy(path, buffer);	

	while(children)
	{
	    printTrienode(children->node, buffer, output);
	    strcpy(buffer, path);
	    children = children->next;
	}

	if(node->c == '/' || node->c == '*')
	{
		fprintf(output, "%s\n", buffer);
		return;
	}

	return;
}

void printTrienodeFull(TrieNode *node, FILE* output)
{
  NodeList* children = node->children;

  if(node==NULL)
  {
      return;
  }

  fprintf(output, "%c", node->c);
  if(node->c == '/' || node->c == '*')
  {
  	fprintf(output, "\n");
  }

  while(children)
  {
      printTrienodeFull(children->node, output);
      children = children->next;
  }

  return;
}

void listItems(TrieNode* node, char buffer[100], char type, FILE* output)
{
	NodeList* children = node->children;
	char path[100];

	if(node==NULL)
	{
	    return;
	}

	buffer[strlen(buffer)] = node->c;
	buffer[strlen(buffer)+1] = '\0';
	strcpy(path, buffer);	

	if((node->c == '/' && buffer[1] != '\0') || node->c == '*')
	{
		if(type == 'n')
		{
			buffer[strlen(buffer)-1] = '\0';
		}

		fprintf(output, "%s ", buffer+1);			

		return;
	}

	while(children)
	{
	    listItems(children->node, buffer, type, output);
	    strcpy(buffer, path);
	    children = children->next;
	}

	return;
}

void printWorkingDirectory(TrieNode* node, TrieNode* currentNode, char buffer[100], FILE* output)
{
	NodeList* children = node->children;
	char path[100];

	if(node==NULL)
	{
	    return;
	}

	buffer[strlen(buffer)] = node->c;
	buffer[strlen(buffer)+1] = '\0';
	strcpy(path, buffer);	

	if(node == currentNode)
	{
		if(strlen(buffer) > 1)
		{
			buffer[strlen(buffer)-1] = '\0';			
		}
		fprintf(output, "%s\n", buffer);
		return;
	}

	while(children)
	{
	    printWorkingDirectory(children->node, currentNode, buffer, output);
	    strcpy(buffer, path);
	    children = children->next;
	}

	if(node->c == '/' || node->c == '*')
	{
		return;
	}

	return;
}

void fixBuffer(char buffer[100])
{
	char bufferCpy[100];
	int i;

	/*if(buffer[1] == '/')
	{
		for(i=1; i<strlen(buffer); i++)
		{
			bufferCpy[i-1] = buffer[i];
		}
		bufferCpy[i] = '\0';

		strcpy(buffer, bufferCpy);
	}*/

	for(i=0; i<strlen(buffer); i++)
	{
		buffer[i] = '/';
	}

	bufferCpy[0] = '\0';
	strcpy(buffer, bufferCpy);
}

char checkEndOfFile(FILE* file, int nr,  char c)
{
	if(nr == 0)
	{
		fscanf(file, "%c", &c);

		if(c == ' ')
		{
			c = '\n';
		}
		else
		{
			fseek(file, -1, SEEK_CUR);
		}
	}

	return c;
}
