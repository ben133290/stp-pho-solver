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

ENV = BaselSlurmEnvironment(cpus_per_task=8)

# Create a new experiment.
exp = PhOExperiment(exp_type=ExpType.PHO, environment=ENV)
# Add custom parser.
exp.add_parser(solver_parser())

# exp.add_parser(ExitcodeParser())

pdbs = add_from_file("/infai/heuser0000/stp-pho-solver/experiments/randpdbs.txt")
print(pdbs)

for i in range(1):
    exp.add_algorithm("Random-" + str(i), get_repo(), "c60f22bcf7b34983c5f157f96a8bf6557efe00be", "Release",
                      flatten(pdbs[i*4:i*4+4])
                      + ("--pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/").split())


exp.add_tasks(get_korf_for_range(0, 100))

# Make a report.
exp.add_report(AbsoluteReport(attributes=solver_attributes()), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
