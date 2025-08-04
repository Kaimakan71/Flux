/*
 * Doubly-linked list implementation.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#ifndef _FLUX_LIST_H
#define _FLUX_LIST_H

typedef struct FluxListItem {
    struct FluxListItem *previous;
    struct FluxListItem *next;
} FluxListItem;

typedef struct {
    FluxListItem *tail;
    FluxListItem *head;
} FluxList;

static inline bool
fluxListIsEmpty(FluxList *list)
{
    return list->head == list->tail;
}

static inline void
fluxListInitialize(FluxList *list)
{
    list->head = (FluxListItem*)list;
    list->tail = (FluxListItem*)list;
}

static inline void
fluxListRemove(FluxListItem *item)
{
    item->previous->next = item->next;
    item->next->previous = item->previous;
}

static inline void
fluxListInsert(FluxListItem *item, FluxListItem *after, FluxListItem *before)
{
    after->next = item;
    item->previous = after;
    before->previous = item;
    item->next = before;
}

static inline void
fluxListAppend(FluxList *list, FluxListItem *item)
{
    fluxListInsert(item, list->tail, (FluxListItem*)list);
}

static inline void
fluxListPrepend(FluxList *list, FluxListItem *item)
{
    fluxListInsert(item, (FluxListItem*)list, list->head);
}

#endif /* !_FLUX_LIST_H */
