from subprocess import Popen, PIPE
from os import listdir
from os.path import isfile, join
import re
import sys

path = sys.argv[1]

f1 = open(join(path,"cevaplar.txt"),"r")

# too tired rn