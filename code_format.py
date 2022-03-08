#!/usr/bin/env python3
"""Recursively formats all c, cpp, h, py files in a directory.

"""

import argparse
import os
import platform
import subprocess
import sys


def _project_dir():
    file_path = os.path.realpath(__file__)
    return os.path.dirname(file_path)


def _format_cxx_file(filepath):
    if platform.system() == "Linux":
        subprocess.call([
            _project_dir() + '/tools/clang-format/clang-format', '-i', filepath
        ])
    elif platform.system() == "Darwin":
        try:
            subprocess.call(['clang-format', '-i', filepath])
        except:
            print("clang-format not installed. please run")
            print("brew install clang-format")
            sys.exit(-1)
    else:
        print("Unsupported OS " + platform.system())
        sys.exit(-1)


def _format_cxx_dir(root_dir):
    """Recursively formats all c, cpp, h files in a directory.

    """
    file_ext = [".c", ".cc", ".cpp", ".cu", ".h", ".hpp", ".cuh"]
    for (root, _, files) in os.walk(root_dir):
        for filename in files:
            _, ext = os.path.splitext(filename)
            if ext in file_ext:
                _format_cxx_file(os.path.join(root, filename))


def _format_py_file(filepath):
    count = len(open(filepath, 'r', encoding="utf8").readlines())
    if count > 2000:
        # Note(Wanguanglu): yapf will stuck when python file is too large.
        return
    subprocess.call(['yapf', '-i', filepath])


def _format_py_dir(root_dir):
    """Recursively formats all python files in a directory.

    """
    file_ext = [".py"]
    for (root, _, files) in os.walk(root_dir):
        for filename in files:
            _, ext = os.path.splitext(filename)
            if ext in file_ext:
                _format_py_file(os.path.join(root, filename))


def main():
    """ Main function """

    parser = argparse.ArgumentParser(description='Formats cxx files')
    parser.add_argument('root_dir', help='Root directory path')
    args = parser.parse_args()

    _format_cxx_dir(args.root_dir)
    _format_py_dir(args.root_dir)


if __name__ == '__main__':
    sys.exit(main())
