/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)time.h	8.5 (Berkeley) 5/4/95
 * $Id: time.h,v 1.18 1998/02/21 03:20:46 jb Exp $
 */

#ifndef _SYS_TIME_H_
#define _SYS_TIME_H_

#include <sys/types.h>

/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */
struct timeval {
	long	tv_sec;		/* seconds */
	long	tv_usec;	/* and microseconds */
};

#ifndef _TIMESPEC_DECLARED
#define _TIMESPEC_DECLARED
struct timespec {
	time_t	tv_sec;		/* seconds */
	long	tv_nsec;	/* and nanoseconds */
};
#endif

#define	TIMEVAL_TO_TIMESPEC(tv, ts) {					\
	(ts)->tv_sec = (tv)->tv_sec;					\
	(ts)->tv_nsec = (tv)->tv_usec * 1000;				\
}
#define	TIMESPEC_TO_TIMEVAL(tv, ts) {					\
	(tv)->tv_sec = (ts)->tv_sec;					\
	(tv)->tv_usec = (ts)->tv_nsec / 1000;				\
}

struct timezone {
	int	tz_minuteswest;	/* minutes west of Greenwich */
	int	tz_dsttime;	/* type of dst correction */
};
#define	DST_NONE	0	/* not on dst */
#define	DST_USA		1	/* USA style dst */
#define	DST_AUST	2	/* Australian style dst */
#define	DST_WET		3	/* Western European dst */
#define	DST_MET		4	/* Middle European dst */
#define	DST_EET		5	/* Eastern European dst */
#define	DST_CAN		6	/* Canada */

/*
 * Structure used to interface to the machine dependent hardware
 * support for timekeeping.
 *
 * A timecounter is a binary counter which has two simple properties:
 *    * it runs at a fixed frequency.
 *    * must not roll over in less than (1+epsilon)/HZ
 *
 * get_timecount reads the counter.
 *
 * get_timedelta returns difference between the counter now and offset_count
 *
 * counter_mask removes unimplemented bits from the count value
 *
 * frequency should be obvious
 *
 * name is a short mnemonic name for this counter.
 *
 * cost is a measure of how long time it takes to read the counter.
 *
 * adjustment [PPM << 16] which means that the smallest unit of correction
 *     you can apply amounts to 481.5 usec/year.
 *
 * scale_micro [2^32 * usec/tick]
 *
 * scale_nano_i [ns/tick]
 *
 * scale_nano_f [(ns/2^32)/tick]
 *
 * offset_count is the contents of the counter which corresponds to the
 *     rest of the offset_* values
 *
 * offset_sec [s]
 * offset_micro [usec]
 * offset_nano [ns/2^32] is misnamed, the real unit is .23283064365...
 *     attoseconds (10E-18) and before you ask: yes, they are in fact 
 *     called attoseconds, it comes from "atten" for 18 in Danish/Swedish.
 */

struct timecounter;
typedef u_int timecounter_get_t __P((struct timecounter *));
typedef	u_int64_t timecounter_delta_t __P((void));

struct timecounter {
	/* These fields must be initialized by the driver */
	timecounter_get_t	*get_timedelta;
	timecounter_delta_t	*get_timecount;
	u_int64_t		counter_mask;
	u_int32_t		frequency;
	char			*name;
	/* These fields will be managed by the generic code */
	int			cost;
	int32_t			adjustment;
	u_int32_t		scale_micro;
	u_int32_t		scale_nano_i;
	u_int32_t		scale_nano_f;
	u_int64_t		offset_count;
	u_int32_t		offset_sec;
	u_int32_t		offset_micro;
	u_int64_t		offset_nano;
	struct timecounter	*other;
	struct timecounter	*tweak;
};

/* Operations on timevals. */
#define	timerclear(tvp)		(tvp)->tv_sec = (tvp)->tv_usec = 0
#define	timerisset(tvp)		((tvp)->tv_sec || (tvp)->tv_usec)
#define	timercmp(tvp, uvp, cmp)						\
	(((tvp)->tv_sec == (uvp)->tv_sec) ?				\
	    ((tvp)->tv_usec cmp (uvp)->tv_usec) :			\
	    ((tvp)->tv_sec cmp (uvp)->tv_sec))
#ifndef KERNEL		/* use timevaladd/timevalsub in kernel */
/* NetBSD/OpenBSD compatable interfaces */
#define timeradd(tvp, uvp, vvp)						\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;	\
		if ((vvp)->tv_usec >= 1000000) {			\
			(vvp)->tv_sec++;				\
			(vvp)->tv_usec -= 1000000;			\
		}							\
	} while (0)
#define timersub(tvp, uvp, vvp)						\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;	\
		if ((vvp)->tv_usec < 0) {				\
			(vvp)->tv_sec--;				\
			(vvp)->tv_usec += 1000000;			\
		}							\
	} while (0)
#endif

/*
 * Names of the interval timers, and structure
 * defining a timer setting.
 */
#define	ITIMER_REAL	0
#define	ITIMER_VIRTUAL	1
#define	ITIMER_PROF	2

struct	itimerval {
	struct	timeval it_interval;	/* timer interval */
	struct	timeval it_value;	/* current value */
};

/*
 * Getkerninfo clock information structure
 */
struct clockinfo {
	int	hz;		/* clock frequency */
	int	tick;		/* micro-seconds per hz tick */
	int	tickadj;	/* clock skew rate for adjtime() */
	int	stathz;		/* statistics clock frequency */
	int	profhz;		/* profiling clock frequency */
};

#define CLOCK_REALTIME	0
#define CLOCK_VIRTUAL	1
#define CLOCK_PROF	2

#define TIMER_RELTIME	0x0	/* relative timer */
#define TIMER_ABSTIME	0x1	/* absolute timer */

#ifdef KERNEL
extern struct timecounter *timecounter;

void	forward_timecounter __P((void));
void	gettime __P((struct timeval *tv));
void	init_timecounter __P((struct timecounter *));
int	itimerfix __P((struct timeval *tv));
int	itimerdecr __P((struct itimerval *itp, int usec));
void	microtime __P((struct timeval *tv));
void	nanotime __P((struct timespec *ts));
void	second_overflow __P((u_int32_t *psec));
void	set_timecounter __P((struct timespec *));
void	timevaladd __P((struct timeval *, struct timeval *));
void	timevalsub __P((struct timeval *, struct timeval *));
#else /* !KERNEL */
#include <time.h>

#include <sys/cdefs.h>

__BEGIN_DECLS
int	adjtime __P((const struct timeval *, struct timeval *));
int	getitimer __P((int, struct itimerval *));
int	gettimeofday __P((struct timeval *, struct timezone *));
int	setitimer __P((int, const struct itimerval *, struct itimerval *));
int	settimeofday __P((const struct timeval *, const struct timezone *));
int	utimes __P((const char *, const struct timeval *));
__END_DECLS

#endif /* !KERNEL */

#endif /* !_SYS_TIME_H_ */
