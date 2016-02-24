from subprocess import Popen, PIPE
from os import listdir
from os.path import isfile, join
import re

path = "testdocs/fdd"
offset = 0

f1 = open(join(path,"cevaplar.txt"), 'w+')

onlyfiles = [join(path, f) for f in listdir(path) if isfile(join(path, f)) and f.endswith(".png")]
print onlyfiles

for x in onlyfiles:
	process = Popen(["tesseract", x, "stdout", "-psm", "6", "-l", "tst"], stdout=PIPE)
	(output, err) = process.communicate()
	exit_code = process.wait()
	stripped = output.strip()
	if stripped == "The page does not contain answers" or stripped == "Cannot open source image!":
		continue
	else:
		stripped = stripped.upper()
		offset += 1
		if stripped[0] != "1":
			stripped = stripped[1:]
		stripped = re.sub("\n\n", "\n", stripped)

	print >>f1, stripped
	print "Page %d imported" % (offset)