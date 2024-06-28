#! /usr/bin/env python

"""
Example experiment using a simple vertex cover solver.
"""
from random import randint

import lab.reports
from downward.parsers.exitcode_parser import ExitcodeParser
from downward.reports.absolute import AbsoluteReport
from lab.environments import BaselSlurmEnvironment
from pho_experiment import PhOExperiment, ExpType, get_repo
from parsers import solver_parser, solver_attributes
from benchmarks import *
from lab.reports import Attribute, geometric_mean, finite_sum

ENV = BaselSlurmEnvironment(cpus_per_task=1, partition="infai_1")

# Create a new experiment.
exp = PhOExperiment(exp_type=ExpType.PHO, environment=ENV)
# Add custom parser.
exp.add_parser(solver_parser())

collections = add_from_file("/infai/heuser0000/stp-pho-solver/experiments/PDBList-Connectedness4.txt")

# exp.add_parser(ExitcodeParser())
exp.add_algorithm("not at all connected", get_repo(), "c48aecdf26cc5d57255b67f5766e51e4a4aa308f", "Release", [])

exp.add_tasks([task + collection for task in get_heuser_for_range(0, 50) for collection in collections[400:500]])

# Make a report.
exp.add_report(AbsoluteReport(attributes=solver_attributes()), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
