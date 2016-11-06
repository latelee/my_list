/*
 * Doubly linked list
 * Copyright (c) 2016 Late Lee <latelee@163.com>
 *
 * for C&C++, M$ or GNU
 *
 */

#ifndef LL__LIST_H
#define LL__LIST_H

#ifdef __cplusplus
extern "C" {
#endif

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)


static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

static inline void __list_add(struct list_head *item,
                 struct list_head *prev,
                 struct list_head *next)
{
    prev->next = item;
    item->prev = prev;
    next->prev = item;
    item->next = next;
}

static inline void list_add_tail(struct list_head *item,
                    struct list_head *head)
{
    __list_add(item, head->prev, head);
}

static inline void list_add_head(struct list_head *item,
                    struct list_head *head)
{
    __list_add(item, head, head->next);
}

static inline void list_add(struct list_head *item,
                    struct list_head *node)
{
    __list_add(item, node, node->next);
}

static inline void list_add_prev(struct list_head *item,
                    struct list_head *node)
{
    __list_add(item, node->prev, node);
}

static inline void list_del(struct list_head *item)
{
    item->next->prev = item->prev;
    item->prev->next = item->next;
    item->next = NULL;
    item->prev = NULL;
}

static inline int list_empty(struct list_head *list)
{
    return list->next == list;
}

static inline unsigned int list_length(struct list_head *list)
{
    struct list_head *item;
    int count = 0;
    for (item = list->next; item != list; item = item->next)
        count++;
    return count;
}

#ifndef offsetof
#define offsetof(type, member) ((long) &((type *) 0)->member)
#endif

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *) ((char *) ptr - offsetof(type, member)))
#endif

#define list_entry container_of

#define list_first_entry(list, type, member) \
    (list_empty((list)) ? NULL : \
     list_entry((list)->next, type, member))

#define list_last_entry(list, type, member) \
    (list_empty((list)) ? NULL : \
     list_entry((list)->prev, type, member))

#define list_entry_prev list_last_entry

#define list_entry_next list_first_entry

#define list_for_each(item, list, type, member) \
    for (item = list_entry((list)->next, type, member); \
         &item->member != (list); \
         item = list_entry(item->member.next, type, member))

#define list_for_each_safe(item, n, list, type, member) \
    for (item = list_entry((list)->next,type, member), \
             n = list_entry(item->member.next, type, member); \
         &item->member != (list); \
         item = n, n = list_entry(n->member.next, type, member))

#define llist_for_each_reverse(item, list, type, member) \
    for (item = list_entry((list)->prev, type, member); \
         &item->member != (list); \
         item = list_entry(item->member.prev, type, member))

#ifdef __cplusplus
}
#endif

#endif /* LL__LIST_H */
