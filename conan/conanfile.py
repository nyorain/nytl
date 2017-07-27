import os
from conans import ConanFile

class NytlConan(ConanFile):
    name = "nytl"
    version = "master"
    generators = "txt"
    url="https://github.com/nyorain/nytl"
    description="Modern C++ generic header-only template library."
    license = "https://github.com/nyorain/nytl/blob/master/LICENSE"
    exports_sources = ["FindNYTL.cmake", os.sep.join(["..", "nytl*"])]
    exports = os.sep.join(["..", "LICENSE"])

    def build(self):
        self.output.info("No compilation necessary for nytl")

    def package(self):
        self.copy("FindNYTL.cmake", ".", ".")
        self.copy("*", src="nytl", dst=os.sep.join([".", "include", "nytl"]))
        self.copy("LICENSE", dst="licenses",  ignore_case=True, keep_path=False)
