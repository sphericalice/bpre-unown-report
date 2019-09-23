#ifndef GUARD_ALLOC_H
#define GUARD_ALLOC_H

#define malloc Alloc
#define free Free

void *Alloc(u32 size);
void Free(void *pointer);

#endif // GUARD_ALLOC_H