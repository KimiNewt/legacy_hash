This package was created to be able to access the old implementation of Python tuple hashes as changed here:
https://github.com/python/cpython/pull/9471

This is because of a case where items were cached with the old hash and needed to be retrieved.
