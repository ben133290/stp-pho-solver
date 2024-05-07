import subprocess

from lab import tools
from lab.cached_revision import CachedRevision


class CachedPhORevision(CachedRevision):
    """This class represents Fast Downward checkouts.

    It provides methods for caching compiled revisions, so they can be reused
    quickly in different experiments.

    """

    def __init__(self, revision_cache, repo, rev, build_type):
        """
        * *revision_cache*: Path to revision cache.
        * *repo*: Path to Fast Downward repository.
        * *rev*: Fast Downward revision.
        * *build_options*: List of build.py options.
        * *subdir*: relative path from *repo* to Fast Downward subdir.
        """
        CachedRevision.__init__(
            self,
            revision_cache,
            repo,
            rev,
            ["./build.sh", build_type],
            exclude=["experiments"],
        )
        # Store for easy retrieval by class users.
        self.build_options = build_type
