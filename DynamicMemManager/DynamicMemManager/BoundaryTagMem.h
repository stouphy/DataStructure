#pragma once
#ifndef _BOUNDARY_TAG_MEM_H_
#define _BOUNDARY_TAG_MEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TAG_FREE (0)
#define TAG_USED (1) 
typedef struct TAGWORD{
	union {
		struct TAGWORD* llink;
		struct TAGWORD* uplink;
	};

	unsigned int tag;
	unsigned int size;
	struct TAGWORD* rlink;
}TAGWORD, *TagSpace;

TagSpace AllocBoundTag(TagSpace *pav, unsigned int n);
void FreeBoundTag(TagSpace* pav, TagSpace ptr);
void SpaceInit(TagSpace* freeSpace, TagSpace* pav);

void print_space(TagSpace pav);
void print(TagSpace s);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _BOUNDARY_TAG_MEM_H_