# Calculate Results

def get_turnaround(log, SCENARIO):
   # turnaround = waiting time + service time
   return log[2] + log[3]

def get_throughput(log, SCENARIO):
   # throughput = work done per unit of time
   return 10000 / log[2]

def get_queue_avg(log, SCENARIO):
   # queue average is customers_in_system - utilization
   avg_customers_in_system = 10
   return avg_customers_in_system - (log[0] / log[1])

def get_cpu_util(log, SCENARIO):
   # util = arrival_rate / service_rate
   return log[0] / log[1]
