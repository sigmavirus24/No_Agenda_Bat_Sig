import os

for line in os.popen("ps fax | grep test"):
	if './test' in line[-1:-10:-1][::-1].rstrip():
		exit

i = os.spawnlp(os.P_NOWAIT, '/usr/bin/screen', 'screen', '-d', '-m', '-S', 'batsig', './test')
