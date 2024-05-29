from lab.parser import Parser
from lab.reports import Attribute, finite_sum, geometric_mean


def solver_parser():
    parser = Parser()
    parser.add_pattern(
        "solution", r"Solution: (.*)\n", type=str, required=False
    )
    parser.add_pattern(
        "wallclock time", r"wall-clock time: (.*) ", type=float, required=True, file="driver.log"
    )
    parser.add_pattern(
        "system time", r"System time: (.*) ms", type=int, required=False
    )
    parser.add_pattern(
        "mean time", r"System time: (.*) seconds", type=int, required=False
    )
    parser.add_pattern(
        "expansions", r"Expanded: (.*)\n", type=int, required=False
    )
    parser.add_pattern(
        "generated", r"Generated: (.*)\n", type=int
    )
    return parser


def solver_attributes():
    return [
        Attribute("solution"),
        Attribute("wallclock time"),
        Attribute("system time"),
        Attribute("mean time", function=geometric_mean),
        Attribute("expansions"),
        Attribute("generated"),
    ]
