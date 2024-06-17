from itertools import combinations
from matplotlib import pyplot as plt
import numpy as np


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


if __name__ == '__main__':

    # make data
    x = range(0, 9)
    y = [get_connectedness_data(5).count(i) for i in range(0, 9)]

    # plot
    fig, ax = plt.subplots()

    ax.stem(x, y, 'dimgray', basefmt=" ")

    ax.set(xlim=(0, 8), ylim=(0, 1800))

    # Add title and labels
    plt.xlabel('Connectedness')
    plt.ylabel('Occurrences')

    # Show the plot
    plt.savefig('stemplot5.png')


