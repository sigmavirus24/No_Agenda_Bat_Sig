import os

for line in os.popen("ps fax | grep nabatsignal"):
	if './nabatsignal' in line[-1:-15:-1][::-1].rstrip():
		exit

from subprocess import call
call('./nabatsignal',)
