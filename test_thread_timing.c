int main()
{
	int err;
	zero = time(NULL);
	main_thr = pthread_self();
	printf(Time Thread \t Event\n);
	printf(==== ====== \t =====\n);
	printf([%2d] Main: \t Started [%s]\n, time1(),
			thread_name(main_thr));
	PTHREAD_ATTR_INIT(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	sleep(1);
	PTHREAD_CREATE(&thr_a, &attr, sub_a, NULL);
	printf([%2d] Main: \t Created thread A [%s]\n, time1(),
			thread_name(thr_a));
	sleep(1);
	PTHREAD_CREATE(&thr_c, &attr, sub_c, NULL);
	printf([%2d] Main: \t Created thread C [%s]\n, time1(),
			thread_name(thr_c));
	sleep(2);
	printf([%2d] Main: \t Cancelling thread D [%s]\n, time1(),
			thread_name(thr_c));
	pthread_cancel(thr_d);
	sleep(1);
	printf([%2d] Main: \t Thread exiting...\n, time1());
	pthread_exit((void *) NULL);
}
