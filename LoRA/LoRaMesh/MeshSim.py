#!/usr/bin/env python





import random





def InitPopulation():
    random.seed()
    population  = []
    for i in range(100):
        population.append({'x':random.random(),'y':random.random()}) 

    return population




if __name__ == '__main__':

    print('LoRaMesh sim')

    population  = InitPopulation()
    print(population)


