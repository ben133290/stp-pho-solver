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

exp.add_algorithm("Connected", get_repo(), "c60f22bcf7b34983c5f157f96a8bf6557efe00be", "Release",
                  (build_pattern_string("1 2 3 4 5 6 7") + build_pattern_string("8 9 10 11 12 13 14 15") +
                   build_pattern_string("1 4 5 8 9 12 13") + build_pattern_string("2 3 6 7 10 11 14 15") +
                   "--pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/").split())

exp.add_algorithm("Rows+Cols", get_repo(), "c60f22bcf7b34983c5f157f96a8bf6557efe00be", "Release",
                  (build_pattern_string("1 2 3 8 9 10 11") + build_pattern_string("4 5 6 7 12 13 14 15") +
                   build_pattern_string("2 4 6 8 10 12 14") + build_pattern_string("1 3 5 7 9 11 13 15") +
                   "--pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/").split())

exp.add_algorithm("Checkers", get_repo(), "c60f22bcf7b34983c5f157f96a8bf6557efe00be", "Release",
                  (build_pattern_string("2 5 7 8 10 13 15") + build_pattern_string("1 3 4 6 9 11 12 14") +
                   build_pattern_string("1 4 5 10 11 14 15") + build_pattern_string("2 3 6 7 8 9 12 13") +
                   "--pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/").split())

exp.add_algorithm("ChequeredVariant", get_repo(), "c60f22bcf7b34983c5f157f96a8bf6557efe00be", "Release",
                  (build_pattern_string("2 5 7 8 10 13 15") + build_pattern_string("1 3 4 6 9 11 12 14") +
                   build_pattern_string("2 4 6 9 11 13 15") + build_pattern_string("1 3 5 7 8 10 12 14") +
                   "--pdbPathPrefix /infai/heuser0000/stp-pho-solver/PDBFILES/").split())

exp.add_tasks(get_korf_for_range(0, 100))

# Make a report.
exp.add_report(AbsoluteReport(attributes=solver_attributes()), outfile="report.html")

# Parse the commandline and run the given steps.
exp.run_steps()
