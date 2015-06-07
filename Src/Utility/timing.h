#pragma once

#include <assert.h>

#define TIMER_ENABLE

#ifdef USE_TBB

#include "tbb/tick_count.h"
using namespace tbb;
#define clock_t tick_count
#define clock() tick_count::now()
#define clock_i double

#else

#include <time.h>
#define clock_i clock_t

#endif

# define	TIMING_BEGIN \
	{clock_t tmp_timing_start = clock();

#ifdef USE_TBB
# define	TIMING_END(message) \
	{clock_t tmp_timing_finish = clock();\
	double  tmp_timing_duration = (tmp_timing_finish - tmp_timing_start) .seconds();\
	printf("%s: %2.5f seconds\n", (message), tmp_timing_duration);}}
#else
# define	TIMING_END(message) \
	{clock_t tmp_timing_finish = clock();\
	double  tmp_timing_duration = (double)(tmp_timing_finish - tmp_timing_start) / CLOCKS_PER_SEC;\
	printf("%s: %2.5f seconds\n", (message), tmp_timing_duration);}}
#endif

# define TIMER_NUM     16

class CBVHTimer {
	clock_i _rets[TIMER_NUM];
	clock_t _tmps[TIMER_NUM];

	int _count;
	long _box_test, _tri_test, _contact, _ccd_test, _cov_test, _lp_test, _ccd_true;
	long _vf_test, _vf_true, _ee_test, _ee_true;

public:
	CBVHTimer() {
		resetTiming();
	}

	void updatTiming() {
			_count++;
	}

	void startTiming(int i) {
#ifdef TIMER_ENABLE
		assert(i >= 0 && i<TIMER_NUM);
		_tmps[i] = clock();
#endif
	}

	clock_i endTiming(int i) {
#ifdef TIMER_ENABLE
		assert(i>=0 && i<TIMER_NUM);
#ifdef USE_TBB
		clock_i dur = (clock()-_tmps[i]).seconds();
		_rets[i] += dur;
		return dur;
#else
		clock_i dur = clock() - _tmps[i];
		_rets[i] += dur;
		return dur;
#endif
#endif
	}

	void resetTiming();
	void incRecord(int box_test, int tri_test, int contact, int ccd_test, int cov_test, int lp_test, int ccd_true);
	void incRecord(int vf_test, int ee_test, int vf_true, int ee_true); 
	void report();
};

