from calculate_results import get_cpu_util, get_queue_avg, get_turnaround, get_throughput
from partition_queue import generate_processes_partition
from global_queue import generate_processes_global

def main():

   # define constants
   ARRIVAL_RATE = float(input("Enter an arrival rate: "))
   AVERAGE_SERVICE_TIME = float(input("Enter an average service time: "))
   CPU_COUNT = int(input("Enter number of CPUs: "))
   print("Select one of the following scenarios - ")
   print(" (1) Partitioned Scheduling")
   print(" (2) Global Scheduling")
   SCENARIO = int(input("Sect Scenario (1 or 2): "))

   # Run simulation... returns list with following values [ARRIVAL_RATE, AVERAGE_SERVICE_TIME, CLOCK, BUSY_TIME, TOTAL_TIME]
   if (SCENARIO == 1):
      log = generate_processes_partition(ARRIVAL_RATE, AVERAGE_SERVICE_TIME, CPU_COUNT)
   else:
      log = generate_processes_global(ARRIVAL_RATE, AVERAGE_SERVICE_TIME, CPU_COUNT)


   # Calculate simulation results
   cpu_util = get_cpu_util(log, SCENARIO)
   queue_avg = get_queue_avg(log, SCENARIO)
   turnaround = get_turnaround(log, SCENARIO)
   throughput = get_throughput(log, SCENARIO)

   # Output results
   output_results(cpu_util, queue_avg, turnaround, throughput, CPU_COUNT, SCENARIO)


def output_results(cpu_util, queue_avg, turnaround, throughput, CPU_COUNT, SCENARIO):
   print("Results")
   print("+-------------------------+")
   print(f"CPU Count:{CPU_COUNT}, Scenario:{SCENARIO}")
   print(f"Average CPU Utilization: {cpu_util}")
   print(f"Average Turnaround Time: {turnaround}")
   print(f"Average Processes in Ready Queue: {queue_avg}")
   print(f"Total Throughput: {throughput}")
   print("+-------------------------+")
   print("\nNow Exiting...")

main()