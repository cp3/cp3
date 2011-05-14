import commands
import time
'''
Takes in inputs from testData.txt in the form of "board description	#" 
where # represents the points for a win at that point in time. 
'''
def main():
	file = open('testData.txt', 'r');
	input = file.read()
	lines = input.split("\n")
	pipeFile = open('pipeData', 'w');
	listOfTimes = []
	numberPassed = 0
	numberFailed = 0
	for line in lines:
		pipeFile = open('pipeData', 'w');
		command = line.split("\t")[0]
		expected = line.split("\t")[1]
		pipeFile.write(command+"\n")
		pipeFile.close()
		start = time.time()
		actual = commands.getstatusoutput('./trogdor.o < pipeData')
		end = time.time()
		if(actual[1] == expected):
			numberPassed+=1
			listOfTimes.append(end-start)
		else:
			print 'Failed at: '+str(command)
			print 'Expected: '+str(expected)
			print 'Actual: '+str(actual[1])
			numberFailed +=1
		
	stats(listOfTimes, numberPassed, numberFailed)
'''
Method used for printing useful stats relating to the previous tests

Times are based soley on passing test cases, failed test cases are not included
in the time calculations
'''
def stats(listOfTimes, numberPassed, numberFailed):
	listOfTimes.sort()
	avrTime = 0
	for i in listOfTimes:
		avrTime+=i
	avrTime = avrTime/len(listOfTimes)
	print '===========================================\n\t\tSTATS\t\t\n==========================================='
	print 'Number Passed:\t'+str(numberPassed)
	print 'Number Failed:\t'+str(numberFailed)
	print 'Average Time:\t'+str(avrTime)
	print 'Fastest Time:\t'+str(listOfTimes[0])
	print 'Slowest Time:\t'+str(listOfTimes[-1])


if __name__ == "__main__":
    main()
    
