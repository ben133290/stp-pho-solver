from itertools import combinations
from matplotlib import pyplot as plt
import numpy as np
import random as rd


def get_connectedness_data(pattern_size: int):
    all_possible_patterns = generate_subsets(pattern_size)
    connectedness_data = [connectedness(pattern) for pattern in all_possible_patterns]
    return connectedness_data


def get_patterns_with_connectedness(pattern_size: int):
    all_possible_patterns = generate_subsets(pattern_size)
    connectedness_data = [(connectedness(pattern), pattern) for pattern in all_possible_patterns]
    return connectedness_data


def connectedness(pattern):
    def is_connected_by_index(a, b):
        if (a + 4 == b) or (a + 1 == b) or (a - 1 == b) or (a - 4 == b):
            return 1
        else:
            return 0

    unique_pairs = [is_connected_by_index(pattern[i], pattern[j]) for i in range(len(pattern)) for j in
                    range(i + 1, len(pattern))]
    return sum(unique_pairs)


def generate_subsets(n):
    numbers = [i for i in range(1, 16)]
    subsets = [list(subset) for subset in combinations(numbers, n)]
    return subsets


def getKey(e):
    match e:
        case (c, p):
            return c
        case _:
            return 0


if __name__ == '__main__':

    """    # make data
    x = range(0, 9)
    y = [get_connectedness_data(7).count(i) for i in range(0, 9)]

    # plot
    fig, ax = plt.subplots()

    ax.stem(x, y, 'dimgray', basefmt=" ")

    ax.set(xlim=(0, 8), ylim=(0, 2500))

    # Add title and labels
    plt.xlabel('Connectedness')
    plt.ylabel('Occurrences')

    # Show the plot
    plt.savefig('stemplot7.png')
    """

    all_patterns = get_patterns_with_connectedness(6)
    all_patterns.sort(key=getKey)
    bins = [list(filter(lambda x: getKey(x) <= 1, all_patterns)),
            list(filter(lambda x: 2 <= getKey(x) <= 3, all_patterns)),
            list(filter(lambda x: 4 <= getKey(x) <= 4, all_patterns)),
            list(filter(lambda x: 5 <= getKey(x) <= 5, all_patterns)),
            list(filter(lambda x: 6 <= getKey(x) <= 7, all_patterns))]
    # print(bins[4])

    collections = [rd.sample(bins[filtered_bin], 10) for filtered_bin in range(5) for sample in range(20)]

    print("average connectedness")
    s = 0
    for col in collections[0:20]:
        s += sum([getKey(e) for e in col])
    print(s / 20)
    s = 0
    for col in collections[20:40]:
        s += sum([getKey(e) for e in col])
    print(s / 20)
    s = 0
    for col in collections[40:60]:
        s += sum([getKey(e) for e in col])
    print(s / 20)
    s = 0
    for col in collections[60:80]:
        s += sum([getKey(e) for e in col])
    print(s / 20)
    s = 0
    for col in collections[80:100]:
        s += sum([getKey(e) for e in col])
    print(s / 20)
