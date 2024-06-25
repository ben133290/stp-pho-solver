from itertools import combinations
from matplotlib import pyplot as plt
import numpy as np
import random as rd
from benchmarks import append_to_file, add_from_file
from typing import List


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


def get_pattern_from_pair(e) -> List[int]:
    match e:
        case (c, p):
            return p
        case _:
            return 0


def transform_int_list_to_string_list(l: List[int]) -> List[str]:
    return [str(i) for i in l]


def connected_experiment_fun(filename: str = "/infai/heuser0000/stp-pho-solver/experiments/PDBList-Connectedness.txt"):
    all_patterns = get_patterns_with_connectedness(6)
    all_patterns.sort(key=getKey)
    bins = [list(filter(lambda x: getKey(x) <= 1, all_patterns)),
            list(filter(lambda x: 2 <= getKey(x) <= 3, all_patterns)),
            list(filter(lambda x: 4 <= getKey(x) <= 5, all_patterns)),
            list(filter(lambda x: 6 <= getKey(x), all_patterns))]
    # print(bins[4])

    collections = [rd.sample(bins[filtered_bin], 10) for filtered_bin in range(4) for sample in range(200)]

    print("average connectedness")
    s = 0
    for col in collections[0:200]:
        s += sum([getKey(e) for e in col])
    print(s / 200)
    s = 0
    for col in collections[200:400]:
        s += sum([getKey(e) for e in col])
    print(s / 200)
    s = 0
    for col in collections[400:600]:
        s += sum([getKey(e) for e in col])
    print(s / 200)
    s = 0
    for col in collections[600:800]:
        s += sum([getKey(e) for e in col])
    print(s / 200)

    for col in collections:
        for pattern_with_con in col:
            append_to_file(filename,
                           " ".join(transform_int_list_to_string_list(get_pattern_from_pair(pattern_with_con))))
        append_to_file(filename, "")


def build_connectedness_plots():
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


if __name__ == '__main__':
    connected_experiment_fun("/infai/heuser0000/stp-pho-solver/experiments/PDBList-Connectedness4.txt")
    print(add_from_file("/infai/heuser0000/stp-pho-solver/experiments/PDBList-Connectedness2.txt")[1])