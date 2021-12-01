# Generate exponential distribution
def exp_dist_gen(ARRIVAL_RATE):
   random_number = uniform_random()
   return (-1) * (1 / (1 / ARRIVAL_RATE)) * math.log(random_number)

# Generate random number from 0 - 1
def uniform_random():
   return random.random()

