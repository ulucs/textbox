from subprocess import Popen, PIPE
from os import listdir
from os.path import isfile, join
import re
import sys

path = sys.argv[1]
offset = 0

f1 = open(join(path,"cevaplar.txt"), 'w+')

onlyfiles = sorted([join(path, f) for f in listdir(path) if isfile(join(path, f)) and f.endswith(".png")])
print onlyfiles

for x in onlyfiles:
	process = Popen(["/usr/local/bin/layoutfit", x], stdout=PIPE)
	(output, err) = process.communicate()
	exit_code = process.wait()
	stripped = output.strip()

	if stripped == "Nothing cropped!":
		continue
	else:
		cropvars = stripped.split(" ")
		break

for x in onlyfiles:

	prargs = ["/usr/local/bin/layoutcrop", x]
	prargs.extend(cropvars)

	process = Popen(prargs, stdout=PIPE)
	(output, err) = process.communicate()
	exit_code = process.wait()
	stripped = output.strip()

	offset += 1
	if stripped == "" or stripped == "Cropped part is empty":
		continue
	else:
		stripped = stripped.upper()

		print >>f1, stripped
		print "Page %d imported" % (offset)