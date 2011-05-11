import commands

'''
Takes in inpuits from testData.txt in the form of "board description	#" 
where # represents the points for a win at that point in time. 
'''
def main():
	file = open('testData.txt', 'r');
	
	input = file.read()
	lines = input.split("\n")
	pipeFile = open('pipeData', 'w');
	
	for line in lines:
		pipeFile = open('pipeData', 'w');
		command = line.split("\t")[0]
		expected = line.split("\t")[1]
		pipeFile.write(command+"\n")
		pipeFile.close()
		actual = commands.getstatusoutput('./trogdor.o < pipeData')
		if(actual[1] == expected):
			print 'Pass'
		else:
			print 'Failed at: '+str(command)
			print 'Expected: '+str(expected)
			print 'Actual: '+str(actual[1])
		
		

if __name__ == "__main__":
    main()
    
