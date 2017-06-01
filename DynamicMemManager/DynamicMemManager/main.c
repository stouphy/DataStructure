#include <stdio.h>
#include "BoundaryTagMem.h"
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

int main(int args, const char* argv[])
{
	TagSpace freeSpace = NULL;
	TagSpace pav = NULL;
	TagSpace p1 = NULL, p2 = NULL;
	SpaceInit(&freeSpace, &pav);
	print(pav);
	p1 = AllocBoundTag(&pav, 10);
	print_space(pav);
	p2 = AllocBoundTag(&pav, 4);
	print_space(pav);

	//FreeBoundTag(&pav, p1);
	//print_space(pav);
	//FreeBoundTag(&pav, p2);
	//print_space(pav);
	if (freeSpace)
	{
		free(freeSpace);
		freeSpace = NULL;
	}

	_CrtDumpMemoryLeaks();
	return 0;
}