import os

for line in os.popen("ps fax | grep test"):
	if './test' in line[-1:-10:-1][::-1].rstrip():
		exit

#from subprocess import call
os.system('./test',)
