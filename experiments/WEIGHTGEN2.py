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
exp = PhOExperiment(exp_type=ExpType.OGEN, environment=ENV)
# Add custom parser.
# exp.add_parser(make_parser())

exp.add_algorithm("additive-pdb-gen", get_repo(), "aefb159349e705087416526cf03ff253220e00c1", "Debug",
                  flatten(get_pdbs_for_range(6, 7)) + ['-f', '/infai/heuser0000/stp-pho-solver/opho6.txt'])
exp.add_tasks([['-r', '1000']])

# Make a report.
# exp.add_report(AbsoluteReport(attributes=ATTRIBUTES), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
