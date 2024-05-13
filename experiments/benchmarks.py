from itertools import combinations


def get_explicit(state):
    return ["-s"] + state.split()


def get_pdbs_for_range(n: int):
    assert (n > 0)
    result = []
    for i in range(1, n):
        result += generate_subsets(i)
    return result


def get_korf_for_range(start: int, end: int):
    assert (end > start >= 0)
    result = []
    for i in range(start, end):
        result += [get_korf(i)]
    return result


def get_korf(n: int):
    result: [str] = ["-k", str(n)]
    return result

def get_random(walklength: int):
    return ["-r", str(walklength)]


def generate_subsets(n):
    numbers = [str(i) for i in range(1, 16)]
    subsets = [["-p", "0"] + list(subset) for subset in combinations(numbers, n)]
    return subsets


if __name__ == '__main__':
    print(get_korf_for_range(0, 3))
