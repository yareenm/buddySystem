#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	char name;
	int block_Size;
	int internal_frag;
	struct Node* next;
} Node;

void request_block(Node **,char, int);
void release_block(Node **, char);
void block_stat(Node *); 

int main() {
	int choice,space;
	char process_name;
	Node *list;
	list = (Node *)malloc(sizeof(Node));

	list->block_Size = 1024;
	list->name = 'F';
	list->internal_frag = -4;
	list->next = NULL;


	printf("******MEMORY******\nBlock1: free- 1024M\n");

	while (1) {
		printf("Enter 0 (EXIT), 1 (request), 2 (release):\n");

		scanf("%d", &choice);

		switch (choice)
		{
		case 0:
			system("pause");
			return 0;
		case 1:
			printf("Process name must be different from F !!\n");
			printf("Enter process name and requested space(M):\n");
			scanf(" %c%d", &process_name, &space);
			request_block(&list, process_name, space);
			block_stat(list);
			break;
		case 2:
			printf("Process name must be different from F !!\n");
			printf("Enter process name:");
			scanf(" %c", &process_name);
			release_block(&list,process_name);
			block_stat(list);
			break;
		default:
			printf("!!THE END!!\n");
			system("pause");
			return 0;
		}
	}
	return 0;
}//main

void block_stat(Node * node) {
	int i = 0;

	if (node == NULL) {
		printf("!!!The list does not exists!!!");
	}
	else {
		printf("\n******MEMORY******\n");
		while (node !=NULL) {
			i++;

			if (node->internal_frag == -4)
				printf("Block - %d: %c - %d M \n", i, node->name, node->block_Size);
			else
				printf("Block - %d: %c - %d M (%d)\n", i, node->name, node->block_Size, node->internal_frag);

			node = node->next;
		}//while
	}//else
}//block_stat

void request_block(Node ** node, char pname, int size) {
	int  half_Size=0;
	Node *temp_Node = *node, *temp2_Node = NULL;

	if (size > temp_Node->block_Size) {
		printf("Not enough space for the request!!\n");
		return;
	}
	else {

		while (temp_Node != NULL) {

			half_Size = temp_Node->block_Size / 2;


			if ((size == temp_Node->block_Size) && (temp_Node->name == 'F')) {

				temp_Node->name = pname;
				temp_Node->internal_frag = 0;
				return;
			}
			else if ((size > half_Size) && (size < temp_Node->block_Size) && (temp_Node->name == 'F')) {
				temp_Node->internal_frag = temp_Node->block_Size - size;
				temp_Node->name = pname;
				return;
			}

			else {

				temp2_Node = (Node*)malloc(sizeof(Node));

				temp2_Node->block_Size = temp_Node->block_Size / 2;
				temp2_Node->internal_frag = -4;
				temp2_Node->name = 'F';

				if (temp_Node->name == 'F') {

					if (size == half_Size) {

						temp2_Node->next = temp_Node->next;
						temp_Node->block_Size = temp_Node->block_Size / 2;
						temp_Node->name = pname;
						temp_Node->internal_frag = 0;
						temp_Node->next = temp2_Node;
						return;
					}

					else if (size < half_Size) {
						temp2_Node->next = temp_Node->next;
						temp_Node->block_Size = temp_Node->block_Size / 2;
						temp_Node->next = temp2_Node;
						continue;
					}

				}
			}
			temp_Node = temp_Node->next;


		}//while
	}//if there is enough space for request
}//REQUEST


void release_block(Node ** node, char pname) {
	int sum = 0;
	Node *head_Node = *node, *prev_Node = *node;


	if (head_Node != NULL && head_Node->name == pname) {

		if (head_Node->next->name == 'F') {
			if (head_Node->next->block_Size == head_Node->block_Size) {
				while (head_Node->next->block_Size == head_Node->block_Size && head_Node->next->name == 'F') {
					*node = head_Node->next;
					(*node)->block_Size = (head_Node->block_Size * 2);
					free(head_Node);
					head_Node = (Node*)malloc(sizeof(Node));
					head_Node = *node;

					if (head_Node->next == NULL) {
						break;
					}

				}

			}//while
			else {
				(*node)->name = 'F';
				(*node)->internal_frag = -4;
				return;
			}
		}
		else {
			(*node)->name = 'F';
			(*node)->internal_frag = -4;
			return;
		}
		
	}// if it is the first node

	else if(head_Node != NULL && head_Node->name != pname){
		while (head_Node != NULL && head_Node->name != pname) {
			prev_Node = head_Node;
			head_Node = head_Node->next;
		}//while

		if (head_Node == NULL) {
			printf("The block named %c was not found in related memory space!!\n",pname);
			return;
		}

		else if (head_Node->name == pname && head_Node->next!=NULL) {

			if (head_Node->next->name == 'F' && head_Node->next->block_Size == head_Node->block_Size) {

				while (head_Node->next->block_Size == head_Node->block_Size && head_Node->next->name == 'F') {

					prev_Node->next = head_Node->next;
					prev_Node->next->block_Size = (head_Node->block_Size * 2);
					free(head_Node);
					head_Node = (Node*)malloc(sizeof(Node));
					head_Node = prev_Node->next;

					if (head_Node->next == NULL) {
						break;
					}

				}//while

			}//if
			else {
				(head_Node)->name = 'F';
				(head_Node)->internal_frag = -4;
				return;
			}
		}
		else {
			if (prev_Node->name == 'F' && prev_Node->block_Size == head_Node->block_Size) {
				while (prev_Node->block_Size == head_Node->block_Size && prev_Node->name == 'F') {

					prev_Node->next = NULL;
					prev_Node->block_Size = (prev_Node->block_Size * 2);
					free(head_Node);
					
					if (prev_Node->next == NULL) {
						break;
					}

				}//while
			
			}

			else {
				head_Node->name = 'F';
				head_Node->internal_frag = -4;
				return;
			}
		
		}//last one
	}//it it is in the middle or last one
}//RELEASE