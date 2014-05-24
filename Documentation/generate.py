#!/usr/bin/python

from subprocess import call
from shutil import rmtree

rmtree("bwapi.wiki")
call("git clone git@github.com:bwapi/bwapi.wiki.git", shell=True)
call("doxygen")


