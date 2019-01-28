#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "point.h"
#include "sorted_points.h"

struct node {
	struct point* data;
	struct node *next;
};
/* this structure should store all the points in a list in sorted order. */
struct sorted_points {
	/* you can define this struct to have whatever fields you want. */
	struct node *head;
};

/* think about where you are going to store a pointer to the next element of the
 * linked list? if needed, you may define other structures. */

struct sorted_points *
sp_init()
{
	struct sorted_points *sp;

	sp = (struct sorted_points *)malloc(sizeof(struct sorted_points));
	assert(sp);

	sp->head = NULL;

	return sp;
}

void
sp_destroy(struct sorted_points *sp)
{
	struct node *to_destroy, *next;
	if(sp->head != NULL){

		to_destroy = sp->head;
		next = to_destroy->next;
		free(to_destroy->data);
		free(to_destroy);

		while(next != NULL){
			to_destroy = next;
			next = to_destroy->next;
			free(to_destroy->data);
			free(to_destroy);
		}
	}
	free(sp);
}

// Helper function used to figure out correct position of new point when equal condition reached
// -1, 0, 1 indicating smaller, equal and greater respectively based on matching rules
int insert_point_equal(struct node *current, struct node *new_node){
	if(new_node->data->x < current->data->x){
		return -1;
	}
	else if(new_node->data->x > current->data->x){
		return 1;
	}
	else{
		if(new_node->data->y < current->data->y){
			return -1;
		}
		else if(new_node->data->y > current->data->y){
			return 1;
		}
		else{
			return 0;
		}
	}
}

int
sp_add_point(struct sorted_points *sp, double x, double y)
{
	// Initialize and malloc new point struct
	struct point *new_point;
	new_point = (struct point *)malloc(sizeof(struct point));
	new_point->x = x;
	new_point->y = y;

	// Initialize and allocate a new node struct
	struct node *new_node;
	new_node = (struct node *)malloc(sizeof(struct node));

	new_node->data = new_point;
	new_node->next = NULL;

	// If linked list is empty just add new node
	if(sp->head == NULL){
		sp->head = new_node;
	}
	// Linked list is not empty
	else{
		struct node *current = sp->head;

		//Deal with case where only head exists ie only one element
		if(current->next == NULL){
			// New point smaller, add to left of head
			if(point_compare(new_point,current->data) == -1){
				sp->head = new_node;
				new_node->next = current;
			}

			// new point greater than head, add to right
			else if(point_compare(new_point,current->data) == 1){
				current->next = new_node;
			}

			// equal, figure out right or left
			else{
				// Handle case when they are equal in length
				int where = insert_point_equal(current, new_node);

				// point is smaller or equal, add to left
				if(where == -1 || where == 0){
					sp->head = new_node;
					new_node->next = current;
				}

				// point is greater add to right
				else if(where == 1){
					current->next = new_node;
				}
			}
		}

		// More than 1 element in linked list
		else{
			// pointer always one behind current pointer
			struct node *follow_pointer = NULL;

			// while new point greater than current or end reached keep traversing
			while(point_compare(new_point,current->data) == 1){
				follow_pointer = current;
				current = current->next;

				if(current == NULL) break;
			}

			// Reached the last node
			if(current == NULL){
				follow_pointer->next = new_node;
			}

			// Not the last node
			else{
				// new point is smaller, add to left
				if(point_compare(new_point, current->data) == -1){
					if(follow_pointer == NULL) sp->head = new_node;
					else follow_pointer->next = new_node;
					new_node->next = current;
				}

				// new point is equal, follow rules to figure position
				else{
					// handle case when equal in length
					int where = insert_point_equal(current, new_node); // use helper function

					// new point smaller or equal, add to left
					if(where == -1 || where == 0){
						if(follow_pointer == NULL) sp->head = new_node;
						else follow_pointer->next = new_node;
						new_node->next = current;
					}

					// new point greater, add to right
					else if(where == 1){
						new_node->next = current->next;
						current->next = new_node;
					}

				}
			}
		}
	}

	return 1;
}

int
sp_remove_first(struct sorted_points *sp, struct point *ret)
{
	if(sp->head == NULL) return 0;

	struct node *to_remove = sp->head;
	ret->x = to_remove->data->x;
	ret->y = to_remove->data->y;
	sp->head = to_remove->next;

	free(to_remove->data);
	free(to_remove);
	return 1;
}

int
sp_remove_last(struct sorted_points *sp, struct point *ret)
{
	struct node *current, *follow;
	current = sp->head;
	follow = current;

	if(current == NULL) return 0;

	while(current->next != NULL){
		follow = current;
		current = current->next;
	}

	ret->x = current->data->x;
	ret->y = current->data->y;

	if(follow == current) sp->head = NULL;
	else follow->next = NULL;

	free(current->data);
	free(current);

	return 1;
}

int
sp_remove_by_index(struct sorted_points *sp, int index, struct point *ret)
{
	struct node *current, *follow;
	int counter = index;

	current = sp->head;
	if(current == NULL) return 0;

	if(index == 0){
		ret->x = current->data->x;
		ret->y = current->data->y;

		sp->head = current->next;

		free(current->data);
		free(current);
	}
	else{
		while(current->next != NULL){
			follow = current;
			current = current->next;
			counter -= 1;
			if(counter == 0) break;
		}

		if(counter != 0) return 0;

		ret->x = current->data->x;
		ret->y = current->data->y;

		follow->next = current->next;

		free(current->data);
		free(current);
	}

	return 1;
}

int same_points(struct node *p1, struct node *p2){
	if(p2 == NULL) return 0;

	if(p1->data->x == p2->data->x && p1->data->y == p2->data->y) return 1;
	else return 0;
}

int
sp_delete_duplicates(struct sorted_points *sp)
{
	struct node *current;

	current = sp->head;

	int delete_count = 0;

	if(current == NULL) return 0;

	while(current != NULL){
		while(same_points(current, current->next)){
			struct node *to_delete = current->next;
			current->next = to_delete->next;
			free(to_delete->data);
			free(to_delete);
			delete_count+=1;
		}
		current = current->next;
	}

	return delete_count;
}
