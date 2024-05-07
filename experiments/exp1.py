#! /usr/bin/env python

"""
Example experiment using a simple vertex cover solver.
"""

import glob
import os
import platform

from downward.reports.absolute import AbsoluteReport
from lab.environments import BaselSlurmEnvironment, LocalEnvironment
from pho_experiment import PhOExperiment
from pho_experiment import get_repo
from lab.parser import Parser
from benchmarks import get_explicit
from lab.reports import Attribute


NODE = platform.node()
REMOTE = NODE.endswith(".scicore.unibas.ch") or NODE.endswith(".cluster.bc2.ch")
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TIME_LIMIT = 1800
MEMORY_LIMIT = 2048
ENV = BaselSlurmEnvironment(email="ben.heuser@unibas.ch")
ATTRIBUTES = ["solution", "error"]

"""
CREATE PARSER
"""


def make_parser():
    vc_parser = Parser()
    vc_parser.add_pattern(
        "solution", r"Solution: \n((Left\n)|(Down\n)|(Right\n)|(Up\n))*", type=str, required=True
    )
    return vc_parser


"""
CREATE EXPERIMENT AND ADD RUNS
"""

# Create a new experiment.
exp = PhOExperiment(environment=ENV, time_limit=TIME_LIMIT, memory_limit=MEMORY_LIMIT)
# Add custom parser.
exp.add_parser(make_parser())

exp.add_algorithm("5-5-5", get_repo(), "4ed29de5", ["-DCMAKE_BUILD_TYPE=Release"],
                  "-p 0 1 2 3 4 5 -p 0 6 7 8 9 10 -p 0 11 12 13 14 15".split())

exp.add_tasks([get_explicit("1 2 6 3 5 0 10 7 4 8 9 11 12 13 14 15"),
               get_explicit("1 2 6 3 5 10 0 7 4 8 9 11 12 13 14 15")])


# Make a report.
exp.add_report(AbsoluteReport(attributes=ATTRIBUTES), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
