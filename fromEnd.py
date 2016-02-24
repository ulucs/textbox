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

	if stripped == "The page does not contain answers" or stripped == "Cannot open source image!" or stripped == "":
		continue
	else:
		stripped = stripped.upper()
		offset += 1
		if stripped[0] != "1":
			stripped = stripped[1:]
		# pure stylistic changes to fit the format
		# regex helps us to preserve the generalness
		# and in this situation speed doesn't matter as much
		stripped = re.sub("\n\n", "\n", stripped)
		stripped = re.sub(" ", "\t", stripped)
		stripped = re.sub("[-\xe2]", ".", stripped)
		stripped = re.sub("\.", ". ", stripped)
		stripped = re.sub("[^A-ETS0-9\. \s\t]","",stripped)
		stripped = re.sub("T\.","T -", stripped)
		stripped = re.sub(r"\bEST","TEST", stripped)

	print >>f1, stripped
	print "Page %d imported" % (offset)