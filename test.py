import random
from deap import base, creator, tools, algorithms

# 定义问题
creator.create("FitnessMin", base.Fitness, weights=(-1.0,))
creator.create("Individual", list, fitness=creator.FitnessMin)

toolbox = base.Toolbox()
toolbox.register("attr_float", random.uniform, -20, 20)
toolbox.register("individual", tools.initRepeat, creator.Individual, toolbox.attr_float, n=30)
toolbox.register("population", tools.initRepeat, list, toolbox.individual)

# 定义评估函数
def evaluate(individual):
    return sum(x**2 for x in individual),

toolbox.register("evaluate", evaluate)
toolbox.register("mate", tools.cxBlend, alpha=0.5)
toolbox.register("mutate", tools.mutGaussian, mu=0, sigma=1, indpb=0.2)
toolbox.register("select", tools.selTournament, tournsize=3)

# 设置算法参数
population_size = 50
crossover_probability = 0.7
mutation_probability = 0.2
number_of_generations = 100

pop = toolbox.population(n=population_size)
algorithms.eaMuPlusLambda(pop, toolbox, mu=population_size, lambda_=population_size,
                          cxpb=crossover_probability, mutpb=mutation_probability,
                          ngen=number_of_generations, stats=None, halloffame=None)

best_individual = tools.selBest(pop, 1)[0]
print("Best individual:", best_individual)
print("Best fitness value:", best_individual.fitness.values)
