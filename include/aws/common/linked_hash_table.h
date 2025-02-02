#ifndef AWS_COMMON_LINKED_HASH_TABLE_H
#define AWS_COMMON_LINKED_HASH_TABLE_H
/*
 * Copyright 2010-2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include <aws/common/hash_table.h>
#include <aws/common/linked_list.h>

/**
 * Simple linked hash table. Preserves insertion order, and can be iterated in insertion order.
 *
 * You can also change the order safely without altering the shape of the underlying hash table.
 */
struct aws_linked_hash_table {
    struct aws_allocator *allocator;
    struct aws_linked_list list;
    struct aws_hash_table table;
    aws_hash_callback_destroy_fn *user_on_value_destroy;
};

/**
 * Linked-List node stored in the table. This is the node type that will be returned in
 * aws_linked_hash_table_get_iteration_list().
 */
struct aws_linked_hash_table_node {
    struct aws_linked_list_node node;
    struct aws_linked_hash_table *table;
    const void *key;
    void *value;
};

AWS_EXTERN_C_BEGIN

/**
 * Initializes the table. Sets up the underlying hash table and linked list.
 * For the other parameters, see aws/common/hash_table.h. Hash table
 * semantics of these arguments are preserved.
 */
AWS_COMMON_API
int aws_linked_hash_table_init(
    struct aws_linked_hash_table *table,
    struct aws_allocator *allocator,
    aws_hash_fn *hash_fn,
    aws_hash_callback_eq_fn *equals_fn,
    aws_hash_callback_destroy_fn *destroy_key_fn,
    aws_hash_callback_destroy_fn *destroy_value_fn,
    size_t initial_item_count);

/**
 * Cleans up the table. Elements in the table will be evicted and cleanup
 * callbacks will be invoked.
 */
AWS_COMMON_API
void aws_linked_hash_table_clean_up(struct aws_linked_hash_table *table);

/**
 * Finds element in the table by key. If found, AWS_OP_SUCCESS will be
 * returned. If not found, AWS_OP_SUCCESS will be returned and *p_value will be
 * NULL.
 *
 * If any errors occur AWS_OP_ERR will be returned.
 */
AWS_COMMON_API
int aws_linked_hash_table_find(struct aws_linked_hash_table *table, const void *key, void **p_value);

/**
 * Puts `p_value` at `key`. If an element is already stored at `key` it will be replaced.
 */
AWS_COMMON_API
int aws_linked_hash_table_put(struct aws_linked_hash_table *table, const void *key, void *p_value);

/**
 * Removes item at `key` from the table.
 */
AWS_COMMON_API
int aws_linked_hash_table_remove(struct aws_linked_hash_table *table, const void *key);

/**
 * Clears all items from the table.
 */
AWS_COMMON_API
void aws_linked_hash_table_clear(struct aws_linked_hash_table *table);

/**
 * returns number of elements in the table.
 */
AWS_COMMON_API
size_t aws_linked_hash_table_get_element_count(const struct aws_linked_hash_table *table);

/**
 * returns the underlying linked list for iteration.
 *
 * The returned list has nodes of the type: aws_linked_hash_table_node. Use AWS_CONTAINER_OF for access to the element.
 */
AWS_COMMON_API
const struct aws_linked_list *aws_linked_hash_table_get_iteration_list(struct aws_linked_hash_table *table);

AWS_EXTERN_C_END

#endif /* AWS_COMMON_LINKED_HASH_TABLE_H */
