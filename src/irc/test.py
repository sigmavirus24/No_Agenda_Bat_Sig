import os

running = False
for line in os.popen("ps fax | grep test"):
	if './test' in line[-1:-10:-1][::-1].rstrip():
		running = True

if not running:
	i = os.spawnlp(os.P_NOWAIT, '/usr/bin/screen', 'screen', '-d', '-m', '-S', 'batsig', './test')
