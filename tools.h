#include <stdio.h>

#ifndef _H_TOOL
#define _H_TOOL


#define fHERE(file, label) fprintf((file), "%s %s:%d: %s(): ", label, __FILE__, __LINE__, __func__)
#define HERE(label) (stdout, (label))

#define TODOF(...)								\
  do {											\
	HERE("[TODO]");								\
	printf(__VA_ARGS__);						\
	printf("\n");								\
  } while (0)
#define TODO() TODOF("") // TODO: Make it so its possible to to call ASSERT and TODO without prints

#define IMPLEMENT() (fHERE(stderr, "[IMPLEMENT]"), abort())

#define ASSERTF(cond, ...)						\
  do {											\
	if (!(cond)) {								\
	  fHERE(stderr, "[ASSERT]");				\
	  fprintf(stderr, __VA_ARGS__);				\
	  fprintf(stderr, "\n");					\
	  abort();									\
	}											\
  } while (0)

#define ASSERT(cond) ASSERTF((cond), "") // TODO: Make it so its possible to to call ASSERT and TODO without prints

#define ASSERT_NULL(ptr) ASSERT((ptr), "%s is null!\n", #ptr)

#define COND_EXIT(cond, fmt, ...)				\
  if ((cond)) {									\
	fprintf(stderr, (fmt), ##__VA_ARGS__);		\
	exit(1);									\
  }

#define WHILE_SIGNAL(cond, count)										\
  int while_signal_counter = 0;											\
  while ((++while_signal_counter % (count) == 0 ? printf("Iter: %d\n", while_signal_counter) : 1) \
		 && (cond))


#endif // _H_TOOL

