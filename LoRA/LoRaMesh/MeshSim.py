#!/usr/bin/env python





import random





def InitPopulation():
    random.seed()
    population  = []
    for i in range(100):
        population.append({'x':random.random(),'y':random.random()}) 

    return population



def CycleSim(time, population):

    for node in population:
        print node


if __name__ == '__main__':

    print('LoRaMesh sim')

    population  = InitPopulation()
    time        = 0
    while True:
        print(time)
        CycleSim(time, population)
        time    = time + 1


