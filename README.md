WILL HIDE THIS REPOSITORY AFTER SUBMITTING...

Notes ref - https://docs.google.com/document/d/1TJCHBYeQtcmixbe3UK7ixc_qP4A_IJ6mXE0TrCD_9DM/edit

===============================

Need to calculate
Average turnaround time
Total throughput
utilization of every CPU
average number of processes in ready queue(s)

Need to
Generate list of processes (arrival time, requested service time)
Arrive with average rate lambda that follows Poisson distribution
Service times granted according to exponential distribution
10k total processes, and output metrics above

2 Design Scenarios
Every CPU has its own ready queue (partitioned), each process as equal chance of going to any of a CPUs queues
All CPUs share a global ready Queue. Whenever CPU is available we dispatch process first to lowest indexed CPU that's available

Events (arrival/departure) make simulator update its state (status of CPU and ready queue contents).

Keep Events in priority queue called "Event Queue"

Clock maintained that represents current time with each added event

Report
- breif description of results with plot for each
(lamda X axis)

Partitioned (multiple lines)
set up array of queues
