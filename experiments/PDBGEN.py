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
ENV = BaselSlurmEnvironment(cpus_per_task=10, partition="infai_2", email='ben.heuser@unibas.ch')
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
exp = PhOExperiment(exp_type=ExpType.PDBGEN, environment=ENV)
# Add custom parser.
# exp.add_parser(make_parser())

exp.add_algorithm("additive-pdb-gen", get_repo(), "f6e363c", "Debug",
                  ["--path", "/infai/heuser0000/stp-pho-solver/PDBFILES"])




patterns = [
    build_pattern_list("1 2 3 4 5 6 8", False),
    build_pattern_list("1 5 6 9 10 13 14", False),
    build_pattern_list("2 3 4 6 7 10 11", False),
    build_pattern_list("3 4 7 8 9 12 13", False),
    build_pattern_list("4 6 7 8 9 10 11", False),
    build_pattern_list("5 6 7 9 10 11 12", False),
    build_pattern_list("6 7 8 9 11 12 13", False),
    build_pattern_list("7 8 9 10 11 14 15", False),
    build_pattern_list("8 9 10 11 13 14 15", False),
]

exp.add_tasks(patterns)

# Make a report.
# exp.add_report(AbsoluteReport(attributes=ATTRIBUTES), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
