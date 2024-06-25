from itertools import combinations
import os
from random import randint, sample


def check_file_exists(number_string):
    # Split the input string into individual numbers
    numbers = number_string.split()

    # Construct the filename
    filename = f"STP(4,4)-0-0;{';'.join(numbers)}-8bpe-lex.pdb"

    # Define the directory path
    directory = "/infai/heuser0000/stp-pho-solver/PDBFILES/"

    # Construct the full file path
    file_path = os.path.join(directory, filename)

    # Check if the file exists
    file_exists = os.path.isfile(file_path)
    if not file_exists:
        # Raise a FileNotFoundError if the file does not exist
        print(f"The file {filename} does not exist in the directory {directory} Pattern: {number_string}\n")
    return file_exists


def build_pattern_string(pattern: str):
    check_file_exists(pattern)
    return "-p 0 " + pattern + " "


def build_state_list(state):
    return ["-s"] + state.split()


def ones():
    return flatten([build_pattern_list(str(s)) for s in range(1, 16)])


def build_pattern_list(pattern: str, expect_file: bool = True):
    file_exists = check_file_exists(pattern)
    if file_exists == expect_file:
        return ["-p", "0"] + pattern.split()
    else:
        print("Pattern " + pattern + f" file {file_exists} ?= {expect_file} exist, do you want to continue? (y/n)")
        raise FileNotFoundError(f"Pattern file {pattern} missing")


def get_pdbs_for_range(start: int, end: int):
    assert (end > 0)
    result = []
    for i in range(start, end):
        result += generate_subsets(i)
    return result


def get_korf_for_range(start: int, end: int):
    assert (end > start >= 0)
    result = []
    for i in range(start, end):
        result += [get_korf(i)]
    return result


def get_heuser_for_range(start: int, end: int):
    assert (end > start >= 0)
    result = []
    for i in range(start, end):
        result += [get_heuser(i)]
    return result


def get_korf(n: int):
    result: [str] = ["-k", str(n)]
    return result


def get_heuser(n: int):
    result: [str] = ["-k", str(n), "-H", str(1)]
    return result


def get_random(walklength: int):
    return ["-r", str(walklength)]


def generate_subsets(n):
    numbers = [str(i) for i in range(1, 16)]
    subsets = [["-p", "0"] + list(subset) for subset in combinations(numbers, n)]
    return subsets


def build_random_pattern_list(size: int):
    subsets = generate_subsets(size)
    return subsets[randint(0, len(subsets))]


def add_random_collection(size: int, length: int):
    subsets = generate_subsets(size)
    run = True
    if length > len(subsets):
        raise ValueError("Requested length exceeds the number of unique subsets available.")
    random_collection = sample(subsets, length)
    for collection in random_collection:
        try:
            check_file_exists(" ".join(collection[2:]))
        except FileNotFoundError:
            append_to_file("/infai/heuser0000/stp-pho-solver/experiments/randpdbs.txt", " ".join(collection[2:]))
            run = False
    if run:
        return flatten(random_collection)
    else:
        return []


def append_to_file(file_path: str, text: str) -> None:
    """
    Appends a given text to a file. Creates the file if it doesn't exist.

    Parameters:
    file_path (str): The path to the file.
    text (str): The text to be written to the file.

    Returns:
    None
    """
    try:
        with open(file_path, 'a') as file:
            file.write(text + '\n')
    except IOError as e:
        print(f"An IOError has occurred: {e}")


def flatten(xss):
    return [x for xs in xss for x in xs]


def add_from_file(filename):
    groups = []
    current_group = []
    try:
        with open(filename, 'r') as file:
            for line in file:
                stripped_line = line.strip()
                if stripped_line:
                    current_group += build_pattern_list(stripped_line, expect_file=True)
                else:
                    if current_group:
                        groups.append(current_group)
                        current_group = []
        if current_group:
            groups.append(current_group)
        return groups
    except FileNotFoundError:
        print(f"Error: The file at {file} was not found.")
        return []
    except Exception as e:
        print(f"An error occurred while reading the file: {e}")
        return []


if __name__ == '__main__':
    print(add_from_file("/infai/heuser0000/stp-pho-solver/experiments/randpdbs.txt"))
