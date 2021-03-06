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
	
	process = Popen(["/usr/local/bin/selecttext", x], stdout=PIPE)
	(output, err) = process.communicate()
	exit_code = process.wait()
	stripped = output.strip()

	if stripped == "" or stripped == "Cropped part is empty":
		continue
	else:
		stripped = stripped.upper()
		offset += 1
		if stripped[0] != "1":
			stripped = stripped[1:]
		# pure stylistic changes to fit the format
		# regex helps us to preserve the generalness
		# and in this situation speed doesn't matter as much
		stripped = re.sub(" ", "\t", stripped)
		stripped = re.sub("[-\xe2]", ".", stripped)
		stripped = re.sub("^\.","1.",stripped)
		stripped = re.sub("\.", ". ", stripped)

		print >>f1, stripped
		print "Test %d imported" % (offset)