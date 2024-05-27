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
ENV = BaselSlurmEnvironment(cpus_per_task=16)
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
exp.add_parser(make_parser())

exp.add_algorithm("additive-pdb-gen", get_repo(), "01db3e5", "Debug",
                  ["--path", "/infai/heuser0000/stp-pho-solver/PDBFILES"])

exp.add_tasks([get_explicit_pdb("0 1 2 3 4 5 6 7"), get_explicit_pdb("0 8 9 10 11 12 13 14 15"),
               get_explicit_pdb("0 4 5 6 7 8 9 10 11"), get_explicit_pdb("0 1 2 5 6 9 10 13 14"),
               get_explicit_pdb("0 1 4 5 8 9 12 13"), get_explicit_pdb("0 2 3 6 7 10 11 14 15")])

# exp.add_tasks([get_explicit_pdb("0 1 2 3 4 5 6 7")])

# Make a report.
exp.add_report(AbsoluteReport(attributes=ATTRIBUTES), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
