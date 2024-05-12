import logging
import os
from collections import OrderedDict
from pathlib import Path

from lab.experiment import Experiment, Run, get_default_data_dir
from enum import Enum
from cached_revision import CachedPhORevision


class PhOAlgorithm:
    def __init__(
            self,
            name: str,
            cached_revision: CachedPhORevision,
            options,
    ):
        #: Algorithm name, e.g., ``"rev123:astar-lmcut"``.
        self.name = name
        #: An instance of :class:`CachedFastDownwardRevision
        #: <downward.cached_revision.CachedFastDownwardRevision>`.
        self.cached_revision = cached_revision
        #: Driver options, e.g., ``["--build", "debug"]``.
        self.options = options

    def __eq__(self, other):
        """Return true iff all components (excluding the name) match."""
        return (
                self.cached_revision == other.cached_revision
                and self.options == other.options
        )


class PDBGENRun(Run):
    """An experiment run that uses *algo* to solve *task*.

    See :py:class:`Run <lab.experiment.Run>` for inherited methods.

    """

    def __init__(self, exp: Experiment, algo: PhOAlgorithm, task: [str], time_limit, memory_limit):
        super().__init__(exp)
        options = algo.options[:]

        solver = os.path.join(
            exp.path,
            algo.cached_revision.get_relative_exp_path("build/pdb_generator"),
        )
        self.add_command(
            "pdb_generator",
            [solver] + options + task,
            time_limit=time_limit,
            memory_limit=memory_limit)

        self._set_properties(algo, options, task)

    def _set_properties(self, algo, options, task):
        self.set_property("algorithm", algo.name)
        self.set_property("repo", algo.cached_revision.repo)
        self.set_property("local_revision", algo.cached_revision.local_rev)
        self.set_property("global_revision", algo.cached_revision.global_rev)
        self.set_property("build_options", algo.cached_revision.build_options)
        self.set_property("options", options)
        self.set_property("problem", "_".join(task))
        self.set_property("domain", "stp")
        self.set_property("experiment_name", self.experiment.name)
        self.set_property("id", [algo.name] + task)


class PhORun(Run):
    """An experiment run that uses *algo* to solve *task*.

    See :py:class:`Run <lab.experiment.Run>` for inherited methods.

    """

    def __init__(self, exp: Experiment, algo: PhOAlgorithm, task: [str], time_limit, memory_limit):
        super().__init__(exp)
        options = algo.options[:]

        solver = os.path.join(
            exp.path,
            algo.cached_revision.get_relative_exp_path("build/pho_solver"),
        )
        self.add_command(
            "pho_solver",
            [solver] + options + task,
            time_limit=time_limit,
            memory_limit=memory_limit)

        self._set_properties(algo, options, task)

    def _set_properties(self, algo, options, task):
        self.set_property("algorithm", algo.name)
        self.set_property("repo", algo.cached_revision.repo)
        self.set_property("local_revision", algo.cached_revision.local_rev)
        self.set_property("global_revision", algo.cached_revision.global_rev)
        self.set_property("build_options", algo.cached_revision.build_options)
        self.set_property("options", options)
        self.set_property("problem", "_".join(task))
        self.set_property("domain", "stp")
        self.set_property("experiment_name", self.experiment.name)
        self.set_property("id", [algo.name] + task)


class ExpType(Enum):
    PHO = 1
    PDBGEN = 2


class PhOExperiment(Experiment):

    def __init__(self, exp_type: ExpType, path=None, environment=None, revision_cache=None,
                 time_limit=None, memory_limit=None):

        Experiment.__init__(self, path=path, environment=environment)

        self._exp_type = exp_type
        self.revision_cache = revision_cache or os.path.join(
            get_default_data_dir(), "revision-cache"
        )

        self._tasks = []

        # Use OrderedDict to ensure that names are unique and ordered.
        self._algorithms = OrderedDict()
        self._time_limit = time_limit
        self._memory_limit = memory_limit

        self.add_step("build", self.build)

        # Add step that executes all runs.
        self.add_step("start", self.start_runs)

        # Add step that parses the logs.
        self.add_step("parse", self.parse)

        # Add step that collects properties from run directories and
        # writes them to *-eval/properties.
        self.add_fetcher(name="fetch")

    def add_tasks(self, tasks):
        self._tasks += tasks

    def add_algorithm(
            self,
            name,
            repo,
            rev,
            build_type=None,
            options=None,
    ):

        if not isinstance(name, str):
            logging.critical(f"Algorithm name must be a string: {name}")
        if name in self._algorithms:
            logging.critical(f"Algorithm names must be unique: {name}")
        build_type = build_type or []
        algorithm = PhOAlgorithm(
            name,
            CachedPhORevision(self.revision_cache, repo, rev, build_type),
            options,
        )
        for algo in self._algorithms.values():
            if algorithm == algo:
                logging.critical(
                    f"Algorithms {algo.name} and {algorithm.name} are identical."
                )
        self._algorithms[name] = algorithm

    def build(self, **kwargs):
        """Add Fast Downward code and runs, and write everything to disk."""
        if not self._algorithms:
            logging.critical("You must add at least one algorithm.")

        self.set_property("algorithms", list(self._algorithms.keys()))

        self._cache_revisions()
        self._add_code()

        match self._exp_type:
            case ExpType.PHO:
                self._add_pho_runs()
            case ExpType.PDBGEN:
                self._add_pdb_runs()
            case _:
                logging.critical("Unknown Experiment Type, couldn't match enum to add run function")

        Experiment.build(self, **kwargs)

    def _get_unique_cached_revisions(self):
        unique_cached_revs = set()
        for algo in self._algorithms.values():
            unique_cached_revs.add(algo.cached_revision)
        return unique_cached_revs

    def _cache_revisions(self):
        for cached_rev in self._get_unique_cached_revisions():
            cached_rev.cache()

    def _add_code(self):
        """Add the compiled code to the experiment."""
        for cached_rev in self._get_unique_cached_revisions():
            dest_path = cached_rev.get_relative_exp_path()
            self.add_resource("", cached_rev.path, dest_path)

    def _add_pho_runs(self):
        tasks = self._tasks
        for algo in self._algorithms.values():
            for task in tasks:
                self.add_run(PhORun(self, algo, task, time_limit=self._time_limit, memory_limit=self._memory_limit))

    def _add_pdb_runs(self):
        tasks = self._tasks
        for algo in self._algorithms.values():
            for task in tasks:
                self.add_run(PDBGENRun(self, algo, task, time_limit=self._time_limit, memory_limit=self._memory_limit))


def get_repo():
    return str(Path(__file__).parent.parent)
