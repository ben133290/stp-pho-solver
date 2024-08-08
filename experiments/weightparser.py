
def read_lines_after(file_path, line_number):
    """
    Reads all lines from a file after a given line number, converts each line to a list of doubles,
    removes duplicates, and returns the result as a list of lists.

    :param file_path: Path to the file.
    :param line_number: The line number after which to start reading lines (0-based index).
    :return: A list of unique lists of doubles.
    """
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()

            # Process lines after the given line number
            lines_after = lines[line_number + 1:]

            # Convert each line to a list of doubles and remove duplicates using a set of tuples
            unique_lines = {tuple(map(float, line.strip().split())) for line in lines_after}

            # Convert set of tuples back to a list of lists
            unique_lines = [list(line) for line in unique_lines]

            return unique_lines
    except FileNotFoundError:
        print(f"The file {file_path} was not found.")
        return []
    except IndexError:
        print(f"The line number {line_number} is out of range for the file {file_path}.")
        return []
    except ValueError as e:
        print(f"Error processing line to float: {e}")
        return []


def read_lines_after2(file_path, line_number):
    """
    Reads all lines from a file after a given line number, converts each line to a list of doubles,
    normalizes 0.0 and -0.0 to the same value, removes duplicates, and returns the result as a list of lists.

    :param file_path: Path to the file.
    :param line_number: The line number after which to start reading lines (0-based index).
    :return: A list of unique lists of doubles.
    """
    def normalize_zero(value):
        return 0.0 if value <= 0.0001 else value

    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()

            # Process lines after the given line number
            lines_after = lines[line_number + 1:]

            # Convert each line to a list of doubles, normalize zeros, and remove duplicates using a set of tuples
            unique_lines = {tuple(normalize_zero(float(x)) for x in line.strip().split()) for line in lines_after}

            # Convert set of tuples back to a list of lists
            unique_lines = [list(line) for line in unique_lines]

            return unique_lines
    except FileNotFoundError:
        print(f"The file {file_path} was not found.")
        return []
    except IndexError:
        print(f"The line number {line_number} is out of range for the file {file_path}.")
        return []
    except ValueError as e:
        print(f"Error processing line to float: {e}")
        return []


if __name__ == '__main__':
    test = read_lines_after2("/infai/heuser0000/stp-pho-solver/opho-con6-6-100.txt", 359)
    print(len(test))
    for line in test:
        print(" ".join(str(d) for d in line))