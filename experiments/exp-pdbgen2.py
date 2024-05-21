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
from benchmarks import *
from lab.reports import Attribute


NODE = platform.node()
REMOTE = NODE.endswith(".scicore.unibas.ch") or NODE.endswith(".cluster.bc2.ch")
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TIME_LIMIT = 1800
MEMORY_LIMIT = 2048
ENV = BaselSlurmEnvironment(cpus_per_task=8)
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
exp = PhOExperiment(exp_type=ExpType.PDBGEN, environment=ENV, time_limit=TIME_LIMIT, memory_limit=MEMORY_LIMIT)
# Add custom parser.
exp.add_parser(make_parser())

exp.add_algorithm("additive-pdb-gen", get_repo(), "207a2c9ae1be8e9653a6f2bc501fb6084535ff35", "Debug",
                  ["--path", "/infai/heuser0000/stp-pho-solver/PDBFILES"])

exp.add_tasks([get_explicit_pdb("0 1 2 3 4 5 6 7"), get_explicit_pdb("0 2 3 6 7 10 11 14 15"),
               get_explicit_pdb("0 8 9 10 11 12 13 14 15"), get_explicit_pdb("0 1 4 5 8 9 12 13")])

# exp.add_tasks([get_explicit_pdb("0 1 2 3 4 5 6 7")])

# Make a report.
exp.add_report(AbsoluteReport(attributes=ATTRIBUTES), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
