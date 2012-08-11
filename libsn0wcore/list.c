/*
 * libsn0w: A collection of useful routines for programming.
 * list.c: Linked lists.
 *
 * Copyright (c) 2007 William Pitcock <nenolod -at- sacredspiral.co.uk>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice is present in all copies.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

//
// Adapted for opensn0w.
//

#include "core.h"

static void *sn0w_node_heap;
static void *sn0w_list_heap;

void sn0w_node_bootstrap(void)
{
        sn0w_node_heap = malloc(sizeof(sn0w_node_t) * 1024);
        sn0w_list_heap = malloc(sizeof(sn0w_list_t) * 64);

	if (sn0w_node_heap == NULL || sn0w_list_heap == NULL)
	{
		printf("heap allocator failure.\n");
		abort();
	}
}

/* creates a new node */
sn0w_node_t *sn0w_node_create(void)
{
        sn0w_node_t *n;

        /* allocate it */
        n = malloc(sizeof(sn0w_node_t));

        /* initialize */
        n->next = n->prev = n->data = NULL;

        /* return a pointer to the new node */
        return n;
}

/* frees a node */
void sn0w_node_free(sn0w_node_t *n)
{
	return_if_fail(n != NULL);

        /* free it */
        free(n);
}

/* adds a node to the end of a list */
void sn0w_node_add(void *data, sn0w_node_t *n, sn0w_list_t *l)
{
	sn0w_node_t *tn;

	return_if_fail(n != NULL);
	return_if_fail(l != NULL);

	n->next = n->prev = NULL;
	n->data = data;

	/* first node? */
	if (l->head == NULL)
	{
		l->head = n;
		l->tail = n;
		l->count++;
		return;
	}

	/* use the cached tail. */
	tn = l->tail;

	/* set the our `prev' to the last node */
	n->prev = tn;

	/* set the last node's `next' to us */
	n->prev->next = n;

	/* set the list's `tail' to us */
	l->tail = n;

	/* up the count */
	l->count++;
}

/* adds a node to the head of a list */
void sn0w_node_add_head(void *data, sn0w_node_t *n, sn0w_list_t *l)
{
	sn0w_node_t *tn;

	return_if_fail(n != NULL);
	return_if_fail(l != NULL);

	n->next = n->prev = NULL;
	n->data = data;

	/* first node? */
	if (!l->head)
	{
		l->head = n;
		l->tail = n;
		l->count++;
		return;
	}

	tn = l->head;
	n->next = tn;
	tn->prev = n;
	l->head = n;
	l->count++;
}

/* adds a node to a list before another node, or to the end */
void sn0w_node_add_before(void *data, sn0w_node_t *n, sn0w_list_t *l, sn0w_node_t *before)
{
	return_if_fail(n != NULL);
	return_if_fail(l != NULL);

	if (before == NULL)
		sn0w_node_add(data, n, l);
	else if (before == l->head)
		sn0w_node_add_head(data, n, l);
	else
	{
		n->data = data;
		n->prev = before->prev;
		n->next = before;
		before->prev = n;

		if (n->prev != NULL)
			n->prev->next = n;

		l->count++;
	}
}

/* adds a node to a list after another node, or to the end */
void sn0w_node_add_after(void *data, sn0w_node_t *n, sn0w_list_t *l, sn0w_node_t *before)
{
	return_if_fail(n != NULL);
	return_if_fail(l != NULL);

	if (before == NULL || before->next == NULL)
		sn0w_node_add(data, n, l);
	else
	{
		n->data = data;
		n->prev = before;
		n->next = before->next;
		before->next = n;
		n->next->prev = n;
		l->count++;
	}
}

/* retrieves a node at `position` position. */
sn0w_node_t *sn0w_node_nth(sn0w_list_t *l, size_t pos)
{
	size_t iter;
	sn0w_node_t *n;

	return_val_if_fail(l != NULL, NULL);

	/* locate the proper position. */
	if (pos < LIST_LENGTH(l) / 2)
		for (iter = 0, n = l->head; iter != pos && n != NULL; iter++, n = n->next);
	else
		for (iter = LIST_LENGTH(l) - 1, n = l->tail;
			iter != pos && n != NULL; iter--, n = n->prev);

	return n;
}

/* returns the data from node at `position` position, or NULL. */
void *sn0w_node_nth_data(sn0w_list_t *l, size_t pos)
{
	sn0w_node_t *n;

	return_val_if_fail(l != NULL, NULL);

	n = sn0w_node_nth(l, pos);

	if (n == NULL)
		return NULL;

	return n->data;
}

/* inserts a node at `position` position. */
void sn0w_node_insert(void *data, sn0w_node_t *n, sn0w_list_t *l, size_t pos)
{
	sn0w_node_t *tn;

	return_if_fail(n != NULL);
	return_if_fail(l != NULL);

	/* locate the proper position. */
	tn = sn0w_node_nth(l, pos);

	sn0w_node_add_before(data, n, l, tn);
}

/* retrieves the index position of a node in a list. */
ssize_t sn0w_node_index(sn0w_node_t *n, sn0w_list_t *l)
{
	ssize_t iter;
	sn0w_node_t *tn;

	return_val_if_fail(n != NULL, -1);
	return_val_if_fail(l != NULL, -1);

	/* locate the proper position. */
	for (iter = 0, tn = l->head; tn != n && tn != NULL; iter++, tn = tn->next);

	return iter < (ssize_t) LIST_LENGTH(l) ? iter : -1;
}

/* deletes a link between a node and a list. */
void sn0w_node_delete(sn0w_node_t *n, sn0w_list_t *l)
{
	return_if_fail(n != NULL);
	return_if_fail(l != NULL);

        /* are we the head? */
        if (!n->prev)
                l->head = n->next;
        else
                n->prev->next = n->next;

        /* are we the tail? */
        if (!n->next)
                l->tail = n->prev;
        else
                n->next->prev = n->prev;

        /* down the count */
        l->count--;
}

/* finds a node by `data' */
sn0w_node_t *sn0w_node_find(void *data, sn0w_list_t *l)
{
	sn0w_node_t *n;

	return_val_if_fail(l != NULL, NULL);

	LIST_FOREACH(n, l->head) if (n->data == data)
		return n;

	return NULL;
}

/* moves a node from one list to another. */
void sn0w_node_move(sn0w_node_t *m, sn0w_list_t *oldlist, sn0w_list_t *newlist)
{
	return_if_fail(m != NULL);
	return_if_fail(oldlist != NULL);
	return_if_fail(newlist != NULL);

        /* Assumption: If m->next == NULL, then list->tail == m
         *        and: If m->prev == NULL, then list->head == m
         */
        if (m->next != NULL)
                m->next->prev = m->prev;
        else
                oldlist->tail = m->prev;

        if (m->prev != NULL)
                m->prev->next = m->next;
        else
                oldlist->head = m->next;

        m->prev = NULL;
        m->next = newlist->head;

        if (newlist->head != NULL)
                newlist->head->prev = m;
        else if (newlist->tail == NULL)
                newlist->tail = m;

        newlist->head = m;

        oldlist->count--;
        newlist->count++;
}

/* creates a new list. */
sn0w_list_t *sn0w_list_create(void)
{
	sn0w_list_t *out = malloc(sizeof(sn0w_list_t));

	return out;
}

/* frees a created list. */
void sn0w_list_free(sn0w_list_t *l)
{
	free(l);
}

/* concatenates two lists together. */
void sn0w_list_concat(sn0w_list_t *l, sn0w_list_t *l2)
{
	return_if_fail(l != NULL);
	return_if_fail(l2 != NULL);

	if (l->tail != NULL)
		l->tail->next = l2->head;

	if (l->tail->next != NULL)
		l->tail->next->prev = l->tail;

	l->tail = l2->tail;
	l->count += l2->count;

	/* clear out l2 as it is no longer needed. */
	l2->head = l2->tail = NULL;
	l2->count = 0;
}

/* reverse a list -- O(n)! */
void sn0w_list_reverse(sn0w_list_t *l)
{
	sn0w_node_t *n, *tn;

	return_if_fail(l != NULL);

	LIST_FOREACH_SAFE(n, tn, l->head)
	{
		sn0w_node_t *tn2 = n->next;
		n->next = n->prev;
		n->prev = tn2;
	}

	tn = l->head;
	l->head = l->tail;
	l->tail = tn;
}

/* sorts a list -- O(n ^ 2) most likely, i don't want to think about it. --nenolod */
void sn0w_list_sort(sn0w_list_t *l, sn0w_list_comparator_t comp, void *opaque)
{
	sn0w_node_t *n, *tn, *n2, *tn2;

	return_if_fail(l != NULL);
	return_if_fail(comp != NULL);

	LIST_FOREACH_SAFE(n, tn, l->head)
	{
		LIST_FOREACH_SAFE(n2, tn2, l->head)
		{
			int result;
			int i, i2;

			if (n == n2)
				continue;

			i = sn0w_node_index(n, l);
			i2 = sn0w_node_index(n2, l);

			if ((result = comp(n, n2, opaque)) == 0)
				continue;
			else if (result < 0 && i > i2)
			{
				sn0w_node_delete(n, l);
				sn0w_node_add_before(n->data, n, l, n2);
			}
			else if (result > 0 && i < i2)
			{
				sn0w_node_delete(n, l);
				sn0w_node_add_after(n->data, n, l, n2);
			}
		}
	}
}
