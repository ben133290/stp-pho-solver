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

ENV = BaselSlurmEnvironment(cpus_per_task=1, email='ben.heuser@unibas.ch')

# Create a new experiment.
exp = PhOExperiment(exp_type=ExpType.PHO, environment=ENV)
# Add custom parser.
exp.add_parser(solver_parser())

# exp.add_parser(ExitcodeParser())

exp.add_algorithm("Linear Conflicts", get_repo(),
                  "10d91c9ded6d22573d32e1f0b6f3305b24379d05",
                  "Release",
                  flatten(get_pdbs_for_range(2, 3))
                  + "--pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/".split())

exp.add_algorithm("3", get_repo(),
                  "10d91c9ded6d22573d32e1f0b6f3305b24379d05",
                  "Release",
                  flatten(get_pdbs_for_range(3, 4))
                  + "--pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/".split())

exp.add_tasks(get_korf_for_range(0, 100))

# Make a report.
exp.add_report(AbsoluteReport(attributes=solver_attributes()), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
