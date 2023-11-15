import setuptools


setuptools.setup(
    name="legacy_hash",
    version="1.1",
    ext_modules=[setuptools.Extension("legacy_hash", sources=["legacy_hash.c"])],
)
