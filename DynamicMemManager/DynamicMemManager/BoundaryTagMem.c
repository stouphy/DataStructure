#include "BoundaryTagMem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORD_SIZE	(64)
#define MIN_REMAIN_BLOCKS (1)
#define FootLoc(p) (p+p->size-1)

TagSpace alloc_addrs[MAX_WORD_SIZE];
int nAllocated = 0;

void SpaceInit(TagSpace* freeSpace, TagSpace* pav)
{
	TagSpace h, f;
	*freeSpace = (TagSpace)malloc(sizeof(struct TAGWORD)*(MAX_WORD_SIZE + 2));
	memset(*freeSpace, 0, sizeof(struct TAGWORD)*(MAX_WORD_SIZE + 2));
	h = *freeSpace;
	h->tag = TAG_USED;
	h++;

	h->tag = TAG_FREE;
	h->llink = h->rlink = h;
	h->size = MAX_WORD_SIZE;
	f = FootLoc(h);
	f->tag = TAG_FREE;
	f->uplink = h;
	*pav = h;

	f++;
	f->tag = TAG_USED;
}

TagSpace AllocBoundTag(TagSpace *pav, unsigned int n)
{
	TagSpace p = NULL;
	TagSpace f = NULL;
	/* S1. Find the memory block that greater and equal than n. */
	for (p = *pav; p && p->size < n && p->rlink != *pav; p = p->rlink)
	{
		// Nothing to do.
	}

	if (!p || p->size < n)
		return NULL; /* not find a valid block. */
	
	*pav = p->rlink; /* change the memory head point to next block.*/
	if (p->size - n <= MIN_REMAIN_BLOCKS)
	{
		if (p == *pav) 
			*pav = NULL; /* the last block, so no space to the next alloc. */
		else
		{	/* unlink the current allocated block from mem-link. */
			(*pav)->llink = p->llink;
			p->llink->rlink = *pav;
		}

		f = FootLoc(p);
		p->tag = f->tag = TAG_USED;
	}
	else
	{
		/* Use the behind address as the allocated memory. */
		p->size -= n;
		f = FootLoc(p);
		f->tag = TAG_FREE;
		f->uplink = p;

		p = f + 1;
		p->size = n;
		p->tag = TAG_USED;
		f = FootLoc(p);
		f->tag = TAG_USED;
		f->uplink = p;
	}

	alloc_addrs[nAllocated++] = p;
	return p;
}

void FreeBoundTag(TagSpace* pav, TagSpace ptr)
{
	TagSpace pre = (ptr - 1)->uplink;
	TagSpace next = ptr + ptr->size;
	TagSpace q = NULL, q1 = NULL;
	unsigned int n;
	int i;
	unsigned int lTag = pre->tag;
	unsigned int rTag = next->tag;
	if (lTag == TAG_USED && rTag == TAG_USED)
	{	/* C1 */
		ptr->tag = TAG_FREE;
		FootLoc(ptr)->uplink = ptr;
		FootLoc(ptr)->tag = TAG_FREE;
		if (NULL == *pav)
		{
			*pav = ptr->llink = ptr->rlink = ptr; /* All link point to itsself. */
		}
		else
		{	/* Insert to the position before the pav (Head Insert). */
			q = (*pav)->llink;
			ptr->rlink = (*pav);
			ptr->llink = q;
			q->rlink = (*pav)->llink = ptr;
			(*pav) = ptr;  /* change the pav to the free node. */
		}
	}
	else if(lTag == TAG_FREE && rTag == TAG_USED)
	{	/* C2 */
		pre->size += ptr->size;
		FootLoc(ptr)->uplink = pre;
		FootLoc(ptr)->tag = TAG_FREE;
	}
	else if (lTag == TAG_USED && rTag == TAG_FREE)
	{	/* C3 */
		ptr->tag = TAG_FREE;
		q = next->llink;
		ptr->llink = q;
		q->rlink = ptr;
		ptr->rlink = next->rlink;
		next->rlink->llink = ptr;
		ptr->size += next->size;
		FootLoc(next)->uplink = ptr;
	}
	else 
	{	/* C4 */
		n = ptr->size;
		pre->size += (n + next->size);
		q = next->llink;
		q1 = next->rlink;
		q->rlink = q1;
		q1->llink = q;
		FootLoc(next)->uplink = pre;
	}

	for (i = 0; i < nAllocated; i++)
	{
		if (ptr == alloc_addrs[i])
		{
			alloc_addrs[i] = 0x0;
		}
	}
}

void print(TagSpace s) {
	printf("The head is 0x%08x SIZE: %d \r\n pre is 0x%08x ,next is 0x%08x\n", s, s->size, s->llink, s->rlink);
}

void print_space(TagSpace pav) {
	if (pav != NULL) {
		printf("you an use the list:\n");
		TagSpace pos = pav;
		for (pos = pos->rlink; pos != pav; pos = pos->rlink) {
			print(pos);
		}
	}

	printf("_____________________________\n");
	int i;
	for (i = 0; i< nAllocated; i++) {
		TagSpace us = alloc_addrs[i];
		if (us) {
			printf("the head is 0x%08x  SIZE is %d\n", us, us->size);
		}
	}

}
