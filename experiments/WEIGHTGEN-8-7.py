#! /usr/bin/env python

"""
Example experiment using a simple vertex cover solver.
"""

import os
import platform

from downward.reports.absolute import AbsoluteReport
from lab.environments import BaselSlurmEnvironment
from pho_experiment import PhOExperiment, ExpType
from pho_experiment import get_repo
from lab.parser import Parser
from benchmarks import *

NODE = platform.node()
REMOTE = NODE.endswith(".scicore.unibas.ch") or NODE.endswith(".cluster.bc2.ch")
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ENV = BaselSlurmEnvironment(cpus_per_task=16, partition="infai_1", email='ben.heuser@unibas.ch')
ATTRIBUTES = ["average", "count", "wctime"]

"""
CREATE PARSER
"""


def make_parser():
    vc_parser = Parser()
    vc_parser.add_pattern(
        "average", r"Average: (.*);", type=float, required=False
    )
    vc_parser.add_pattern(
        "count", r"count: (.*)", type=int, required=False
    )
    vc_parser.add_pattern(
        "wctime", r"wall-clock time: (.*)s", type=float, required=True, file="driver.log"
    )
    return vc_parser


"""
CREATE EXPERIMENT AND ADD RUNS
"""

# Create a new experiment.
exp = PhOExperiment(exp_type=ExpType.OGEN, environment=ENV)
# Add custom parser.
# exp.add_parser(make_parser())

patterns = [
    build_pattern_list("1 2 3 4 5 6 7"),
    build_pattern_list("8 9 10 11 12 13 14 15"),
    build_pattern_list("1 4 5 8 9 12 13"),
    build_pattern_list("2 3 6 7 10 11 14 15"),
    build_pattern_list("1 3 5 7 9 11 13 15"),
    build_pattern_list("2 4 6 8 10 12 14"),
    build_pattern_list("1 2 3 8 9 10 11"),
    build_pattern_list("4 5 6 7 12 13 14 15"),
    build_pattern_list("1 4 5 10 11 14 15"),
    build_pattern_list("2 3 6 7 8 9 12 13"),
    build_pattern_list("2 5 7 8 10 13 15"),
    build_pattern_list("1 3 4 6 9 11 12 14"),
    build_pattern_list("1 3 5 7 8 10 12 14"),
    build_pattern_list("2 4 6 9 11 13 15")
]

exp.add_algorithm("additive-pdb-gen", get_repo(), "da57e9d90df8346b92a203156068ca767821b79e", "Debug",
                  flatten(patterns) + ['-f', '/infai/heuser0000/stp-pho-solver/opho_01_78.txt'])

exp.add_tasks([['-r', '100', '-o', '1']])

# Make a report.
# exp.add_report(AbsoluteReport(attributes=ATTRIBUTES), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
