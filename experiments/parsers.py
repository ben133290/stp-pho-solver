from lab.parser import Parser
from lab.reports import Attribute, finite_sum, geometric_mean, arithmetic_mean
import re


def parse_expansions_per_second(content, props):
    matches = re.findall(r"Expanded: (.*)\n.*\nSystem time: (.*) μs", content)
    unpack = [(float(e) * 1000000 / float(s)) for (e, s) in matches]
    props["exppersec"] = unpack[0]


def solver_parser():
    # NOTE: Don't use spaces in name attribute
    parser = Parser()
    # parser.add_pattern(
    #    "solution", r"Solution: (.*)\n", type=str, required=False
    # )
    parser.add_pattern(
        "wallclocktime", r"wall-clock time: (.*)s", type=float, required=True, file="driver.log"
    )
    parser.add_pattern(
        "systemtime", r"System time: (.*) μs", type=int, required=False
    )
    # parser.add_pattern(
    #    "meantime", r"System time: (.*) μs", type=int, required=False
    # )
    parser.add_pattern(
        "memory", r"Peak memory: (.*) KB", type=int, required=False
    )
    parser.add_pattern(
        "expansions", r"Expanded: (.*)\n", type=int, required=False
    )
    parser.add_pattern(
        "generated", r"Generated: (.*)\n", type=int, required=False
    )
    # parser.add_function(parse_expansions_per_second)
    return parser

def solver_parser_precise_time():
    # NOTE: Don't use spaces in name attribute
    parser = Parser()
    # parser.add_pattern(
    #    "solution", r"Solution: (.*)\n", type=str, required=False
    # )
    parser.add_pattern(
        "wallclocktime", r"wall-clock time: (.*)s", type=float, required=True, file="driver.log"
    )
    parser.add_pattern(
        "systemtime", r"System time: (.*) μs", type=int, required=False
    )
    parser.add_pattern(
        "solvertime", r"Solving: (.*) μs", type=int, required=False
    )
    parser.add_pattern(
        "retrievingtime", r"Retrieving: (.*) μs", type=int, required=False
    )
    parser.add_pattern(
        "expansions", r"Expanded: (.*)\n", type=int, required=False
    )
    parser.add_pattern(
        "generated", r"Generated: (.*)\n", type=int, required=False
    )
    # parser.add_function(parse_expansions_per_second)
    return parser

def solver_attributes():
    return [
        #Attribute("solution"),
        Attribute("wallclocktime"),
        Attribute("systemtime"),
        #Attribute("meantime", function=arithmetic_mean),
        Attribute("expansions"),
        #Attribute("exppersec", function=arithmetic_mean),
        Attribute("generated"),
        Attribute("memory"),
    ]

def solver_attributes_precise_time():
    return [
        #Attribute("solution"),
        Attribute("wallclocktime"),
        Attribute("systemtime"),
        Attribute("solvertime"),
        Attribute("retrievingtime"),
        #Attribute("meantime", function=arithmetic_mean),
        Attribute("expansions"),
        #Attribute("exppersec", function=arithmetic_mean),
        Attribute("generated"),
    ]