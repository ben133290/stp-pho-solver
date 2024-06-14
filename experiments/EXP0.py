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

ENV = BaselSlurmEnvironment(cpus_per_task=5)

# Create a new experiment.
exp = PhOExperiment(exp_type=ExpType.PHO, environment=ENV)
# Add custom parser.
exp.add_parser(solver_parser())

# exp.add_parser(ExitcodeParser())

exp.add_algorithm("PAPDB-7-8" + str(sample), get_repo(),
                  "c60f22bcf7b34983c5f157f96a8bf6557efe00be",
                  "Release",
                  build_pattern_list("1 2 3 4 5 6 7", True)
                  + build_pattern_list("8 9 10 11 12 13 14 15", True)
                  + "--heuristic 1 --pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/".split())

exp.add_algorithm("PAPDB-5-5-5" + str(sample), get_repo(),
                  "c60f22bcf7b34983c5f157f96a8bf6557efe00be",
                  "Release",
                  build_pattern_list("1 2 4 5 8", True)
                  + build_pattern_list("3 6 7 10 11", True)
                  + build_pattern_list("9 12 13 14 15", True)
                  + "--heuristic 1 --pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/".split())

exp.add_algorithm("PAPDB-6-6-3" + str(sample), get_repo(),
                  "c60f22bcf7b34983c5f157f96a8bf6557efe00be",
                  "Release",
                  build_pattern_list("1 2 4 5 8 9", True)
                  + build_pattern_list("3 6 7 10 11 15", True)
                  + build_pattern_list("12 13 14", True)
                  + "--heuristic 1 --pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/".split())


exp.add_tasks(get_korf_for_range(0, 100))

# Make a report.
exp.add_report(AbsoluteReport(attributes=solver_attributes()), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
