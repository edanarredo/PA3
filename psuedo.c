#define ARR 0
#define DEP 1
#define MAX_CPU_COUNT 4

struct event
{
	int type;
	float time;
	int cpu; // associated CPU, mainly for DEP event
	event *nextEvent;
}

float lambda;
float T_s;
int cpu_count;

float clock;
event *eq_head = NULL;

int proc_executing_on_cpu[MAX_CPU_COUNT]; // -1 means idle
int ARR_count;
int DEP_count;

queue parReadyQueue[MAX_CPU_COUNT];
int parReadyCount[MAX_CPU_COUNT]; // initialized to 0

queue gloReadyQueue;
int gloReadyCount; // initialized to 0

//generate a value that follows an Exponential Distribution with an average of avg
float exp_dist_gen(float avg)
{
	srand(time(NULL));
	float z = ((float)rand()) / RAND_MAX;
	return -avg * log(z);
}

//create a future event and add to the event list in order of event time
//for future departure event, associated "cpu" must be apecified
//for future arrival event, just set "cpu" to -1 in this schedule_event function,
//because which processor this arrival will be on might need to be determined later, especially for global scheduling
schedule_event(int type, float time, int cpu, eq_head)
{
	event *e = new event(type, time, cpu);
	Insert e to the linked list headed at eq_head while keep the linked list still sorted w.r.t.time.(similar to insertion sort)
}

void init()
{
	clock = 0;
	proc_executing_on_cpu[0 ... cpu_count - 1] = -1;
	ARR_count = 0;
	DEP_count = 0;
	parReadyCount[0 ... cpu_count - 1] = 0;
	gloReadyCount = 0;

	ensure queues parReadyQueue[0...cpu_count - 1] are all empty
		 ensure queue gloReadyQueue is empty

			  sched_event(ARR, 0 + exp_dist_gen(1 / lambda), -1, eq_head);
}

//*****BEGIN PARTITIONED
void par_arr_handler(e)
{
	cpu_assigned = rand() % cpu_count;
	if (proc_executing_on_cpu[cpu_assigned] == -1)
	{
		proc_executing_on_cpu[cpu_assigned] = ARR_count; // ARR_count is infact the index of the process being handled
		sched_event(DEP, clock + exp_dist_gen(T_s), cpu_assigned, eq_head);
	}
	else
	{
		parReadyQueue[cpu_assigned].enqueue(ARR_count);
		//begin metrics
		parReadyCount[cpu_assigned]++;
		//end metrics
	}

	//	creat next arrival
	sched_event(ARR, clock + exp_dist_gen(1 / lambda), -1, eq_head);
}

void par_dep_handler(e)
{
	dep_cpu = e->cpu;
	if (parReadyQueue[dep_cpu].empty() == true)
	{
		proc_executing_on_cpu[dep_cpu] = -1;
	}
	else
	{
		proc_executing_on_cpu[dep_cpu] = parReadyQueue[dep_cpu].dequeue();
		sched_event(DEP, clock + exp_dist_gen(T_s), dep_cpu, eq_head);
		//begin metrics
		parReadyCount[dep_cpu]--;
		//end metrics
	}
}

void par_Simul()
{
	//begin metrics
	totalTime = 0;
	totalTurnaroundTime = 0;
	totalBusyTime[cpu_count] = 0;
	weightedReadyCount[cpu_count] = 0;
	//end metrics

	init();
	while (DEP_count < 10000)
	{

		lastClock = clock;

		event *e = get_next_event();
		clock = e->time;

		//begin metrics
		duration = clock - lastClock;
	for
		i = 0 to cpu_count - 1 if (proc_executing_on_cpu[i] > -1)
										 totalBusyTime[i] += duration;
	for
		i = 0 to cpu_count - 1 weightedReadyCount[i] += parReadyCount[i] * duration;
	//end metrics

	switch (e->type)
	{
	case ARR:
		//begin metrics
		arrivalTime[ARR_count] = clock; //process indexed from 0
		//Note that when having *multiple CPUs*,
		//the k-th departure might not be the process that is the k-th arrival, even under FCFS.
		//So, the size of array arrivalTime[] needs to be greater than 10000,
		//you can just estimate (or try and error) a large enough size for the purpose of this project
		//end metrics
		par_arr_handler(e);
		ARR_count++; //ARR_count is indeed # of ARR we've handled, because this ++ is at the very end
		break;
	case DEP:
		//begin metrics
		dep_cpu = e->cpu;
		dep_proc = proc_executing_on_cpu[dep_cpu];
		totalTurnaroundTime += clock - arrivalTime[dep_proc];
		//end metrics
		par_dep_handler(e);
		DEP_count++; //DEP_count is indeed # of DEP we've handled, because this ++ is at the very end
		break;
	}
	delete e from the event list;
	}

	//begin metrics
	totalTime = clock;
	//end metrics

	// You need to calculate the required 4 metrics next
	// based on totalTime, totalTurnaroundTime, totalBusyTime[], weightedReadyCount[], etc.
}
//*****END PARTITIONED

//*****BEGIN GLOBAL
void glo_arr_handler(e)
{
	allCPUbusy = true;

	// find the idle cpu with the lowest index, if any idle
	for
		i = 0 to cpu_count - 1 if (proc_executing_on_cpu[i] == -1)
		{
			// ARR_count(which has not been ++ for this iteration yet) is in fact the index of the process being handled
			proc_executing_on_cpu[i] = ARR_count;
			allCPUbusy = false;
			sched_event(DEP, clock + exp_dist_gen(T_s), i, eq_head);
			break;
		}

	//	join the global ready queue, if all CPUs are busy
	if (allCPUbusy == ture)
	{
		gloReadyQueue.enqueue(ARR_count);
		//begin metrics
		gloReadyCount++;
		//end metrics
	}

	//	creat next arrival
	sched_event(ARR, clock + exp_dist_gen(1 / lambda), -1, eq_head);
}

void glo_dep_handler(e)
{
	dep_cpu = e->cpu;
	if (gloReadyQueue.empty() == true)
	{
		proc_executing_on_cpu[dep_cpu] = -1;
	}
	else
	{
		proc_executing_on_cpu[dep_cpu] = gloReadyQueue.dequeue();
		sched_event(DEP, clock + exp_dist_gen(T_s), dep_cpu, eq_head);
		//begin metrics
		gloReadyCount--;
		//end metrics
	}
}

void glo_Simul()
{
	//begin metrics
	totalTime = 0;
	totalTurnaroundTime = 0;
	totalBusyTime[cpu_count] = 0;
	weightedReadyCount = 0;
	//end metrics

	init();

	while (DEP_count < 10000)
	{

		lastClock = clock;

		event *e = get_next_event();
		clock = e->time;

		//begin metrics
		duration = clock - lastClock;
	for
		i = 0 to cpu_count - 1 if (proc_executing_on_cpu[i] > -1)
										 totalBusyTime[i] += duration;
	weightedReadyCount += gloReadyCount * duration;
	//end metrics

	switch (e->type)
	{
	case ARR:
		//begin metrics
		arrivalTime[ARR_count] = clock; //process indexed from 0
		//Note that when having *multiple CPUs*,
		//the k-th departure might not be the process that is the k-th arrival, even under FCFS.
		//So, the size of array arrivalTime[] needs to be greater than 10000,
		//you can just estimate (or try and error) a large enough size for the purpose of this project
		//end metrics
		par_arr_handler(e);
		ARR_count++; //ARR_count is indeed # of ARR we've handled
		break;
	case DEP:
		//begin metrics
		dep_cpu = e->cpu;
		dep_proc = proc_executing_on_cpu[dep_cpu];
		totalTurnaroundTime += clock - arrivalTime[dep_proc];
		//end metrics
		par_dep_handler(e);
		DEP_count++; //DEP_count is indeed # of DEP we've handled
		break;
	}
	delete e from the event list;
	}

	//begin metrics
	totalTime = clock;
	//end metrics

	// You need to calculate the required 4 metrics next
	// based on totalTime, totalTurnaroundTime, totalBusyTime[], weightedReadyCount, etc.
}
//*****END GLOBAL

void simul(float a1, float a2, int a3, int a4)
{
	lambda = a1;
	T_s = a2;
	cpu_count = a4;

	if (a3 == 1)
		par_Simul(); //partitioned
	else if (a3 == 2)
		glo_Simul(); //global
	else
		Print "Invalid Scenarios."
}