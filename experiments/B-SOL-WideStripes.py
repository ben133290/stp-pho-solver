#! /usr/bin/env python

"""
Example experiment using a simple vertex cover solver.
"""

from downward.parsers.exitcode_parser import ExitcodeParser
from downward.reports.absolute import AbsoluteReport
from lab.environments import BaselSlurmEnvironment
from pho_experiment import PhOExperiment, ExpType, get_repo
from lab.parser import Parser
from benchmarks import get_korf_for_range
from lab.reports import Attribute, geometric_mean

ENV = BaselSlurmEnvironment(cpus_per_task=16)
ATTRIBUTES = ["solution", "wctime", "time", Attribute("mean time", function=geometric_mean)]

"""
CREATE PARSER
"""


def make_parser():
    vc_parser = Parser()
    vc_parser.add_pattern(
        "solution", r"Solution: (.*)\n", type=str, required=False
    )
    vc_parser.add_pattern(
        "time", r"Total solve time: (.*) seconds", type=int, required=False
    )
    vc_parser.add_pattern(
        "mean time", r"Total solve time: (.*) seconds", type=int, required=False
    )
    vc_parser.add_pattern(
        "wctime", r"wall-clock time: (.*)s", type=float, required=True, file="driver.log"
    )
    return vc_parser


"""
CREATE EXPERIMENT AND ADD RUNS
"""

# Create a new experiment.
exp = PhOExperiment(exp_type=ExpType.PHO, environment=ENV)
# Add custom parser.
exp.add_parser(make_parser())
# exp.add_parser(ExitcodeParser())

exp.add_algorithm("four seasons", get_repo(), "01db3e5", "Release",
                  "-p 0 1 2 3 4 5 6 7 -p 0 8 9 10 11 12 13 14 15 -p 0 1 4 5 8 9 12 13 -p 0 2 3 6 7 10 11 14 15 "
                  "-v false "
                  "--pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/".split())

exp.add_algorithm("german flag", get_repo(), "01db3e5", "Release",
                  "-p 0 1 2 3 4 5 6 7 -p 0 8 9 10 11 12 13 14 15 -p 0 4 5 6 7 8 9 10 11 "
                  "-v false "
                  "--pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/".split())


exp.add_tasks(get_korf_for_range(22, 23))


# Make a report.
exp.add_report(AbsoluteReport(attributes=ATTRIBUTES), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
