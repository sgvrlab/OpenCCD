# include "timing.h"
# include <stdio.h>
# include <assert.h>

void
CBVHTimer::resetTiming()
{
	_count = 0;

	for (int i=0; i<10; i++) {
		_rets[i] =  0;
	}

	_box_test = _tri_test = _contact = _ccd_test = _cov_test = _lp_test = 0L;
	_ccd_true = 0;

	_vf_true = _ee_true = _vf_test = _ee_test = 0L;

}

void
CBVHTimer::incRecord(int box_test, int tri_test, int contact, int ccd_test, int cov_test, int lp_test, int ccd_true)
{
#ifdef TIMER_ENABLE
	_box_test += box_test;
	_tri_test += tri_test;
	_contact += contact;
	_ccd_test += ccd_test;
	_cov_test += cov_test;
	_lp_test += lp_test;
	_ccd_true += ccd_true;
#endif
}

void
CBVHTimer::incRecord(int vf_test, int ee_test, int vf_true, int ee_true)
{
#ifdef TIMER_ENABLE
	_vf_test += vf_test;
	_ee_test += ee_test;
	_vf_true += vf_true;
	_ee_true += ee_true;
#endif
}

void
CBVHTimer::report()
{
#ifdef USE_TBB

#ifdef TIMER_ENABLE
	printf("frame %d: =================================\n", _count);
	printf("intersection time: %g\n", _rets[0]);
#ifdef PAIR_BUFFER
	printf("    clear ff array: %g\n", _rets[13]);
#endif
	printf("    traverse BVH & collection: %g\n", _rets[1]);
	printf("    processing colliding: %g\n", _rets[2]);
	printf("    processing self-colliding: %g\n", _rets[3]);
	printf("    processing elm tests: %g\n", _rets[4]);
	printf("BVH and update time: %g\n", _rets[9]);
	printf("    refit BVH: %g\n", _rets[10]);
	printf("         update normal cone: %g\n", _rets[12]);
	printf("    update vtx, boxes for V/E/F: %g\n", _rets[11]);
	printf("all ee/vf tests: %ld, true ee/vf tests: %ld\n", _ccd_test, _lp_test);
	printf("all vf tests: %ld, true vf tests: %ld, ratio: %f\n", _vf_test, _vf_true, float(_vf_true)/_vf_test);
	printf("all ee tests: %ld, true ee tests: %ld, ratio: %f\n", _ee_test, _ee_true, float(_ee_true)/_ee_test);
	printf("all tri tests: %ld, true tri tests: %ld\n", _tri_test, _cov_test);
	printf("box test: %ld, contact: %ld\n", _box_test, _contact);
#endif

#else

#ifdef TIMER_ENABLE
	printf("frame %d: =================================\n", _count);
	printf("intersection time: %g\n", _rets[0]/1000.);
	printf("    traverse BVH & collection: %g\n", _rets[1]/1000.);
	printf("    processing colliding: %g\n", _rets[2]/1000.);
	printf("    processing self-colliding: %g\n", _rets[3]/1000.);
	printf("    processing elm tests: %g\n", _rets[4]/1000.);
	printf("BVH and update time: %g\n", _rets[9]/1000.);
	printf("    refit BVH: %g\n", _rets[10]/1000.);
	printf("         update normal cone: %g\n", _rets[12]/1000.);
	printf("    update vtx, boxes for V/E/F: %g\n", _rets[11]/1000.);
	printf("all ee/vf tests: %ld, true ee/vf tests: %ld\n", _ccd_test, _lp_test);
	printf("all vf tests: %ld, true vf tests: %ld, ratio: %f\n", _vf_test, _vf_true, float(_vf_true)/_vf_test);
	printf("all ee tests: %ld, true ee tests: %ld, ratio: %f\n", _ee_test, _ee_true, float(_ee_true)/_ee_test);
	printf("all tri tests: %ld, true tri tests: %ld\n", _tri_test, _cov_test);
	printf("box test: %ld, contact: %ld\n", _box_test, _contact);
#endif

#endif
}
