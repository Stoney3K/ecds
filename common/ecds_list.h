/*****************************************************************************/
/*	@file ecds_list.h														 */
/*	@brief Defines a double linked list.					 				 */
/*																			 */
/*	This is an implementation of a dynamically-sized, double linked list	 */
/*	which grows or shrinks depending on the items added or removed.			 */
/*	It is an independent implementation, the items themselves do not		 */
/*	register in the memory manager but are managed by the list itself.		 */
/*																			 */
/*	Only ecds_list_take_item() and ecds_list_drop_item() use the memory		 */
/*	manager to signal that objects are still in use by the containing lists. */
/*																			 */
/*	The list structures are opaque but accessible through the manipulation	 */
/*	functions which are declared in this header.							 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_LIST_H
#define _ECDS_LIST_H

typedef struct _ecds_list_t * ecds_list_t;
typedef struct _ecds_list_item_t * ecds_list_item_t;

/**
 * @brief Create a new list containing no items.
 */
ecds_list_t * ecds_list_new();

/**
 * @brief Initialize a list that was allocated externally.
 */
void ecds_list_initialize(ecds_list_t * list);

/**
 * @brief Dispose a list created earlier.
 * @param list The list object to dispose.
 */
void ecds_list_dispose(ecds_list_t * list);

/**
 * @brief Add an item to a list.
 * @param list The list to add an item to.
 * @param obj The ECDS object to be added to the list.
 * @return The newly created list item if succesful, or NULL if not.
 */
ecds_list_item_t * ecds_list_add_item(ecds_list_t * list, ecds_object_t * obj);

/**
 * @brief Create an orphaned list item.
 * @param obj The ECDS object to be encapsulated.
 * @return The newly created list item if succesful, or NULL if not.
 */
ecds_list_item_t * ecds_list_create_item(ecds_object_t * obj);

/**
 * @brief Get the next item for an item.
 * @param item The item to act on.
 * @return The next item.
 */
ecds_list_item_t * ecds_list_next_item(ecds_list_item_t * item);

/**
 * @brief Get the previous item for an item.
 * @param item The item to act on.
 * @return The previous item.
 */
ecds_list_item_t * ecds_list_previous_item(ecds_list_item_t * item);

/**
 * @brief Get the first item in a list.
 * @param list The list to get the first item from.
 * @return The first item.
 */
ecds_list_item_t * ecds_list_first_item(ecds_list_t * list);

/**
 * @brief Get the last item in a list.
 * @param list The list to get the last item from.
 * @return The last item.
 */
ecds_list_item_t * ecds_list_last_item(ecds_list_t * list);

/**
 * @brief Take an ecds_list_item_t that already exists and add it to this list.
 * @param list The list to add an item to.
 * @param item The ecds_list_item_t to take. The object inside the item is referenced.
 * @return The adopted list item if succesful, or NULL if not.
 */
ecds_list_item_t * ecds_list_take_item(ecds_list_t * list, ecds_list_item_t * item);

/**
 * @brief Copy an item from another ecds_list_t to a new list.
 * @param item The item to copy. If the item's list is NULL, this will work the same as ecds_list_take_item().
 * @param to The list to copy to. If this parameter is NULL, this will return a new orphaned item with the same
 *		  data as the source.
 * @note When both the source and target lists are NULL no action is performed and the original item is returned.
 * @return The adopted list item if succesful, or NULL if not.
 */
ecds_list_item_t * ecds_list_copy_item(ecds_list_item_t * item, ecds_list_t * target);

/**
 * @brief Drop (orphan) an item from a list.
 * @param list The list to remove an item from.
 * @param item The list item to remove. When orphaned, the object inside the item is dereferenced.
 * @return The list item that is now dropped. NULL if the item was not found or any other error.
 */
ecds_list_item_t * ecds_list_drop_item(ecds_list_item_t * item);

/**
 * @brief Dispose a list item.
 * @param list The list to remove an item from.
 * @param item The list item to delete.
 * @return The contained object if removal was succesful. NULL otherwise.
 */
ecds_object_t * ecds_list_dispose_item(ecds_list_item_t * item);

/**
 * @brief Find an object in the list.
 * @param list The list to search.
 * @param obj The object that you are looking for.
 * @return The encapsulating ecds_list_item_t object if succesful, or NULL otherwise.
 */
ecds_list_item_t * ecds_list_find_item(ecds_list_t * list, ecds_object_t * obj);

/**
 * @brief Fetch an item from the list at a specific index.
 * @param list The list to act on.
 * @param index The integer index of the object you want.
 * @return The ecds_object_t that was requested, or NULL if it was not found.
 */
ecds_object_t * ecds_fetch_item(ecds_list_t * list, int index);

/**
 * @brief Convert a list into an array.
 * @param list The list to convert.
 * @return An array of (ecds_object_t *) objects pointing to the list entries. The array is managed by
 *		   the list.
 */
ecds_object_t ** ecds_list_to_array(ecds_list_t * list);

/**
 * @brief Conduct an operation on each of the objects in a list.
 * @param list The list to operate on.
 * @param operation A function that is performed on each of the list items.
 */
void ecds_list_foreach(ecds_list_t * list, void (* operation)(ecds_object_t * obj));

/**
 * @brief Clone the list.
 * @param list The list to operate on.
 * @return A shallow copy of the list with the same items as the original.
 */
ecds_list_t * ecds_list_clone(ecds_list_t * list);

/**
 * @brief Clone the list and copy only the items that match a specific filter.
 * @param list The list to operate on.
 * @param filter_func A function that returns TRUE if the filter conditions match.
 */
ecds_list_t * ecds_list_filter(ecds_list_t * list, bool (* filter_func)(ecds_object_t * obj));

/**
 * @brief Concatenate two lists together.
 * @param list1 The first list in the chain. The items of the second list are added behind the first.
 * @param list2 The second list in the chain.
 * @return A new list which is the two lists chained together.
 */
ecds_list_t * ecds_list_concat(ecds_list_t * list1, ecds_list_t * list2);

/**
 * @brief Zip two lists, merging the items in an interleaved manner.
 * @param list1 The first list in the chain.
 * @param list2 The second list in the chain.
 * @return A merged list with the items of both lists merged together.
 */
ecds_list_t * ecds_list_zip(ecds_list_t * list1, ecds_list_t * list2);

/**
 * @brief Split a list. The list is cut after the item at position and the remainder is returned.
 * @param list The list to split.
 * @param position The position in the list where to split.
 * @return The tail end of the split list.
 */
ecds_list_t * ecds_list_split(ecds_list_t * list, unsigned int position);

/**
 * @brief Insert a list item at a specific position.
 * @param list1 The list to act on.
 * @param position The index after which to insert the object.
 * @param list2 The list to insert.
 * @return The merged list.
 */
ecds_list_t * ecds_list_insert_list(ecds_list_t * list1, unsigned int position, ecds_list_t * list);

/**
 * @brief Insert a list item at a specific position.
 * @param list The list to act on.
 * @param position The index after which to insert the object.
 * @param item The item to insert.
 */
void ecds_list_insert_item(ecds_list_t * list1, unsigned int position, ecds_list_item_t * item);

/**
 * @brief Sort a list with a user-defined comparison function.
 * @param list The list to sort.
 * @param compare_func A pointed to a boolean function which returns TRUE if object a should be sorted behind object b, and false otherwise.
 * @return A merged list with the items of both lists merged together.
 */
void ecds_list_sort(ecds_list_t * list, bool (* compare_func)(ecds_object_t * a, ecds_object_t * b));

/**
 * @brief Copy part of the list into a new list.
 * @param list The list to copy from.
 * @param from The index of the first item to copy.
 * @param to The index of the last item to copy.
 * @return A merged list with the items of both lists merged together.
 */
ecds_list_t * ecds_list_sublist(ecds_list_t * list, unsigned int from, unsigned int to);


ecds_object_t * ecds_list_construct_object();
void ecds_list_dispose_object(ecds_object_t * obj);

#endif /* _ECDS_LIST_H */