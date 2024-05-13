#! /usr/bin/env python

"""
Example experiment using a simple vertex cover solver.
"""

import glob
import os
import platform

from downward.reports.absolute import AbsoluteReport
from lab.environments import BaselSlurmEnvironment, LocalEnvironment
from pho_experiment import PhOExperiment, ExpType
from pho_experiment import get_repo
from lab.parser import Parser
from benchmarks import get_explicit, get_korf, get_korf_for_range
from lab.reports import Attribute

TIME_LIMIT = 1800
MEMORY_LIMIT = 2048
ENV = BaselSlurmEnvironment()
ATTRIBUTES = ["solution", "wctime", "time"]

"""
CREATE PARSER
"""


def make_parser():
    vc_parser = Parser()
    vc_parser.add_pattern(
        "solution", r"Solution: (.*)\n", type=str, required=False
    )
    vc_parser.add_pattern(
        "time", r"Total solve time: (.*) microseconds", type=int, required=False
    )
    vc_parser.add_pattern(
        "wctime", r"wall-clock time: (.*)s", type=float, required=True, file="driver.log"
    )
    return vc_parser


"""
CREATE EXPERIMENT AND ADD RUNS
"""

# Create a new experiment.
exp = PhOExperiment(exp_type=ExpType.PHO, environment=ENV, time_limit=TIME_LIMIT, memory_limit=MEMORY_LIMIT)
# Add custom parser.
exp.add_parser(make_parser())

exp.add_algorithm("5-5-5", get_repo(), "7564ec4bd97dfab9f12a6d4ffed50f1365cc29fa", "Release",
                  "-p 0 1 2 3 4 5 -p 0 6 7 8 9 -p 0 10 11 12 13 -p 0 14 15 -v true "
                  "--pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/".split())

exp.add_tasks(get_korf_for_range(0, 20))


# Make a report.
exp.add_report(AbsoluteReport(attributes=ATTRIBUTES), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
