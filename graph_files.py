
import subprocess
import time

if __name__=="__main__":
	while True:
		time.sleep(60)
		print("suttt")
		fh = open("stuff.txt", "a+")
		print(subprocess.check_output("./thing.sh"))
		fh.write(str(subprocess.check_output("./thing.sh").decode("ascii")))
		fh.write("oof")

		fh.close()


