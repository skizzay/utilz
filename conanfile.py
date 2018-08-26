from pathlib import Path
from conans import ConanFile


def _UtilzConan__read_contents_of_file(filename):
    license = Path(__file__).parent.joinpath(filename)
    with license.open() as fd:
        return fd.read()


class UtilzConan(ConanFile):
    name = "utilz"
    version = "0.0.3"
    license = _UtilzConan__read_contents_of_file("LICENSE")
    url = "https://github.com/skizzay/utilz"
    description = _UtilzConan__read_contents_of_file("README.md")
    no_copy_source = True
    exports_sources = "src/**/*.h"
    exports = "LICENSE", "README.md"
    # No settings/options are necessary, this is header only

    def package(self):
        self.copy("*.h", dst="include", src="src", keep_path=True)
