import setuptools


setuptools.setup(
    name="legacy_hash",
    version="1.0",
    ext_modules=[setuptools.Extension("legacy_hash", sources=["legacy_hash.c"])],
)
