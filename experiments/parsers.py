from lab.parser import Parser


def solver_parser():
    parser = Parser()
    parser.add_pattern(
        "solution", r"Solution: (.*)\n", type=str, required=False
    )
    parser.add_pattern(
        "time", r"Total solve time: (.*) seconds", type=int, required=False
    )
    parser.add_pattern(
        "mean time", r"Total solve time: (.*) seconds", type=int, required=False
    )
    parser.add_pattern(
        "wctime", r"wall-clock time: (.*)s", type=float, required=True, file="driver.log"
    )
    parser.add_pattern(
        "expansions", r"Expansions: (.*)\n", type=int, required=False
    )
    parser.add_pattern(
        "generated", r"Generated: (.*)\n", type=int
    )
    return parser
