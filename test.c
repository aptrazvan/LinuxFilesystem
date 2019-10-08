#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "struct.h"
#include "navigationFunctions.h"
#include "buildFunctions.h"

int main()
{
	TrieNode *root = createTrienode('/', NULL);
	TrieNode *currentNode = root, *node;

	int i, j, nr;
	char command[100], cpyCommand[100], line[100], location[10][100], path[100], cpyPath[100], c, *buffer;
	buffer = (char*)calloc(100, sizeof(char));
	buffer[0] = '\0';

	FILE *input, *output, *error;
	input = fopen("input.in", "r");	
	output = fopen("output.out", "w+");
	error = fopen("error.out", "w+");

	fscanf(input, "%d", &nr);

	while(fscanf(input, "%s%c", command, &c) != EOF)
	{
		nr--;

		buffer[0] = '\0';
		line[0] = '\0';

		if(strcmp(command, "mkdir") == 0)
		{		
			if(c == '\n')
			{
				fprintf(error, "%s: missing operand\n", command);
				continue;
			}	

			while(fscanf(input, "%s%c", command, &c) && c != '\n')
			{
				node = pathType(root, currentNode, command);
				makeDirectory(node, command, 'd', error);
			}
			node = pathType(root, currentNode, command);
			makeDirectory(node, command, 'd', error);

			continue;

		}
		if(strcmp(command, "touch") == 0)
		{
			if(c == '\n')
			{
				fprintf(error, "%s: missing operand\n", command);
				continue;
			}

			while(fscanf(input, "%s%c", command, &c) && c != '\n')
			{
				node = pathType(root, currentNode, command);
				makeDirectory(node, command, 'f', error);
			}
			node = pathType(root, currentNode, command);
			makeDirectory(node, command, 'f', error);

			continue;

		}
		if(strcmp(command, "print") == 0)
		{
			printTrienode(root, buffer, output);
			continue;
		}
		if(strcmp(command, "cd") == 0)
		{
			if(c == '\n')
			{
				fprintf(error, "%s: missing operand\n", command);
				continue;
			}

			fscanf(input, "%s%c", command, &c);

			c = checkEndOfFile(input, nr, c);

			if(c != '\n')
			{
				fprintf(error, "%s: too many arguments\n", command);
				while(fscanf(input, "%s%c", command, &c) != EOF && c != '\n')
				{

				}
				continue;
			}
			
			node = currentNode;
			node = pathType(root, node, command);
			

			if(changeDirectory(node, command, error))
			{
				currentNode = changeDirectory(node, command, error);				
			}

			continue;

		}
		if(strcmp(command, "ls") == 0)
		{
			if(c == '\n')
			{
				listItems(currentNode, buffer, 'n', output);
				fprintf(output, "\n");				
			}
			else
			{
				strcpy(cpyCommand, command);
				strcat(line, "ls ");
				fscanf(input, "%s%c", command, &c);
				strcat(line, command);


				if(strcmp(command, "-F") == 0)
				{
					if(c != '\n')
					{
						fscanf(input, "%s%c", command, &c);
						strcat(line, " ");
						strcat(line, command);

						if(c != '\n')
						{
							while(c != '\n')
							{
								fscanf(input, "%s%c", command, &c);
								strcat(line, " ");
								strcat(line, command);
							}
							fprintf(error, "%s: too many arguments\n", line);
							continue;
						}

						node = pathType(root, currentNode, command);
						node = changeDirectory(node, command, error);						
					}
					else
					{
						node = currentNode;
					}
					
					listItems(node, buffer, 'f', output);
					fprintf(output, "\n");				
				}
				else
				{
					node = pathType(root, currentNode, command);
					node = changeDirectory(node, command, error);
					listItems(node, buffer, 'n', output);	
					fprintf(output, "\n");								
				}

				if(c != '\n')
				{
					fprintf(error, "%s: too many arguments\n", cpyCommand);
					while(c != '\n')
					{
						fscanf(input, "%s%c", command, &c);
					}
					continue;
				}
			}

			continue;

		}
		if(strcmp(command, "pwd") == 0)
		{
			if(c != '\n')
			{
				fprintf(error, "%s: too many arguments\n", command);
				while(c != '\n')
				{
					fscanf(input, "%s%c", command, &c);
				}
				continue;
			}

			printWorkingDirectory(root, currentNode, buffer, output);
			continue;
		}
		if(strcmp(command, "rmdir") == 0)
		{
			if(c == '\n')
			{
				fprintf(error, "%s: missing operand\n", command);
				continue;
			}

			while(fscanf(input, "%s%c", command, &c) && c != '\n')
			{
				node = pathType(root, currentNode, command);
				
				if(isFile(node, command) == 1)
				{
					fprintf(error, "%s: Not a directory\n", command);
					continue;
				}

				removeDirectory(node, command, error);					
			}

			if(isFile(node, command) == 1)
			{
				fprintf(error, "%s: Not a directory\n", command);
				continue;
			}

			node = pathType(root, currentNode, command);
			removeDirectory(node, command, error);	

			continue;

		}
		if(strcmp(command, "rm") == 0)
		{
			if(c == '\n')
			{
				fprintf(error, "%s: missing operand\n", command);
				continue;
			}

			while(fscanf(input, "%s%c", command, &c) && c != '\n')
			{
				node = pathType(root, currentNode, command);
				recursiveRemoval(node, command, error);					
			}
			node = pathType(root, currentNode, command);
			recursiveRemoval(node, command, error);

			continue;
		
		}
		if(strcmp(command, "cp") == 0)
		{
			if(c == '\n')
			{
				fprintf(error, "%s: missing operand\n", command);
				continue;
			}

			i = 0;
			while(fscanf(input, "%s%c", command, &c) && c != '\n')
			{
				strcpy(location[i], command);
				i++;
			}
			strcpy(path, command);
			strcpy(cpyPath, path);

			for(j=0; j<i; j++)
			{
				copyFile(root, currentNode, location[j], path, error);
				strcpy(path, cpyPath);
			}

			continue;

		}
		if(strcmp(command, "mv") == 0)
		{
			c = checkEndOfFile(input, nr, c);

			if(c == '\n')
			{
				fprintf(error, "%s: missing operand\n", command);
				continue;
			}

			i = 0;

			while(fscanf(input, "%s%c", command, &c) && checkEndOfFile(input, nr, c) != '\n')
			{
				strcpy(location[i], command);
				i++;
			}

			if(i == 0)
			{
				fprintf(error, "mv %s: missing operand\n", command);
				continue;
			}

			strcpy(path, command);
			strcpy(cpyPath, path);

			for(j=0; j<i; j++)
			{
				moveFile(root, currentNode, location[j], path, error);
				strcpy(path, cpyPath);
			}

			continue;

		}

		fprintf(error, "%s: command not found\n", command);
		while(c != '\n')
		{
			fscanf(input, "%s%c", command, &c);
		}
	}

	free(buffer);
	destroyTrienode(root);
	fclose(input);
	fclose(output);
	fclose(error);
}