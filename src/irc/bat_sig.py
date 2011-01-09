#!/usr/bin/env py
import os

running = False
for line in os.popen("ps fax | grep nabatsignal"):
	if './nabatsignal' in line[-1:-15:-1][::-1].rstrip():
		running = True

if not running:
	i = os.spawnlp(os.P_NOWAIT, '/usr/bin/screen', 'screen', '-d', '-m', '-S', 'batsig', './nabatsignal')
