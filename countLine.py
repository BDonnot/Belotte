import os
import time
import sys
import glob 
import re
#~ print os.listdir(sys.path)  
currentDir = sys.path[0]
def count(files) :
	numberLines = 0
	for file in  files :
		fichier = open(file)
		count = 1
		for line in fichier :
			line = re.sub('{|}|;| *', '',line)
			if(re.search('/\*',line)) :
				count = 0
			if(re.search('\*/',line) and not(count)) :
				count = 1
			#~ if((len(line) > 1) and  (re.search(r"^[ \t]*/{2}",line) and count):
			if((len(line) > 1) and  (line[0:2]  !=  "//")&count):
				numberLines += 1
		fichier.close()
	return numberLines
files = glob.glob(currentDir+'/*.cpp')
numberLines = count(files)
files = glob.glob(currentDir+'/*.h')
numberLines += count(files)
print numberLines