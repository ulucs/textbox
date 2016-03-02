from subprocess import Popen, PIPE
from os import listdir
from os.path import isfile, join
import sys

path = sys.argv[1]
offset = 0

onlyfiles = sorted([join(path, f) for f in listdir(path) if isfile(join(path, f)) and f.endswith(".png")])
print onlyfiles

for x in onlyfiles:
	
	process = Popen(["/usr/local/bin/boxslice", x], stdout=PIPE)
	(output, err) = process.communicate()
	exit_code = process.wait()
	stripped = output.strip()

	offset += 1

	print "File %d sliced!" % (offset)