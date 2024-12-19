#ifndef IFNDEF
#define RESULT_IFNDEF

#elifndef ELIFNDEF1
#define RESULT_ELIFNDEF1

#elifndef ELIFNDEF2
#define RESULT_ELIFNDEF2

#elifdef ELIFDEF
#define RESULT_ELIFDEF 
#ifdef IFDEF
#define RESULT_IFDEF
#endif

#else
#define RESULT_ELSE

#endif
