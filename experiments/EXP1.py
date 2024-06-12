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

ENV = BaselSlurmEnvironment(cpus_per_task=1)

# Create a new experiment.
exp = PhOExperiment(exp_type=ExpType.PHO, environment=ENV)
# Add custom parser.
exp.add_parser(solver_parser())


# exp.add_parser(ExitcodeParser())

for sample in range(2):
    exp.add_algorithm("random-4-" + str(sample), get_repo(), "c60f22bcf7b34983c5f157f96a8bf6557efe00be", "Release",
                      add_random_collection(4, 5) +
                      "--pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/".split())
    exp.add_algorithm("random-3-" + str(sample), get_repo(), "c60f22bcf7b34983c5f157f96a8bf6557efe00be", "Release",
                      add_random_collection(3, 13 * 5) +
                      "--pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/".split())

exp.add_tasks(get_korf_for_range(60, 65))

# Make a report.
exp.add_report(AbsoluteReport(attributes=solver_attributes()), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
