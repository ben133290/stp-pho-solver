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
from parsers import *
from benchmarks import *
from lab.reports import Attribute, geometric_mean, finite_sum

ENV = BaselSlurmEnvironment(cpus_per_task=1, email='ben.heuser@unibas.ch', partition='infai_1')

# Create a new experiment.
exp = PhOExperiment(exp_type=ExpType.PHO, environment=ENV, time_limit=7200)
# Add custom parser.
exp.add_parser(solver_parser())

for sample in range(20):
    exp.add_algorithm("6-"+str(sample), get_repo(), "c48aecd", "Release",
                      add_random_collection(6, 5) + ones())
    exp.add_algorithm("5-"+str(sample), get_repo(), "c48aecd", "Release",
                      add_random_collection(5, 50) + ones())
    exp.add_algorithm("4-"+str(sample), get_repo(), "c48aecd", "Release",
                      add_random_collection(4, 550) + ones())

exp.add_tasks(get_heuser_for_range(0, 100))

# Make a report.
exp.add_report(AbsoluteReport(attributes=solver_attributes()), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
