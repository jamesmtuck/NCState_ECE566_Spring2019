/*
 * $Id: timing.h,v 1.1 2004/04/15 20:18:41 renau Exp $
 */
#ifndef _TIMING_H
#define _TIMING_H

char	*p64(uint64 big);
char	*p64sz(uint64 big);
double	Delta(void);
double	Now(void);
void	adjust(int usec);
void	bandwidth(uint64 bytes, uint64 times, int verbose);
int	bytes(char *s);
void	context(uint64 xfers);
uint64	delta(void);
int	get_enough(int);
uint64	get_n(void);
void	kb(uint64 bytes);
double	l_overhead(void);
char	last(char *s);
void	latency(uint64 xfers, uint64 size);
void	mb(uint64 bytes);
void	micro(char *s, uint64 n);
void	micromb(uint64 mb, uint64 n);
void	milli(char *s, uint64 n);
void	morefds(void);
void	nano(char *s, uint64 n);
uint64	now(void);
void	ptime(uint64 n);
void	rusage(void);
void	save_n(uint64);
void	settime(uint64 usecs);
void	start(struct timeval *tv);
uint64	stop(struct timeval *begin, struct timeval *end);
uint64	t_overhead(void);
double	timespent(void);
void	timing(FILE *out);
uint64	tvdelta(struct timeval *, struct timeval *);
void	tvsub(struct timeval *tdiff, struct timeval *t1, struct timeval *t0);
void	use_int(int result);
void	use_pointer(void *result);
uint64	usecs_spent(void);
void	touch(char *buf, int size);

#if defined(hpux) || defined(__hpux) || defined(WIN32)
int	getpagesize();
#endif

#endif /* _TIMING_H */
