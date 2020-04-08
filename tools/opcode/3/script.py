import re
opcodesCloseList=[]
instructionsFile=open('instructionsSet.txt','r+')
instructionsLines=instructionsFile.readlines()
instructionsFile.close()
opcodeLines=""
for i in range(len(instructionsLines)):
	line=instructionsLines[i]
	if line=='\n':
		opcodeLines+='\n'
		continue
	startArs=line.index('{')
	opcode=line[:startArs]
	#print(opcode)
	args=line[startArs+1:-3].split(',')
	funcName,func,parm1,parm2,setFunc,cycles=args[0],args[1],args[2],args[3],args[4],args[5]
	'''if '$' in parm1:
		setFunc="&CPU::setMemoryValue"
	else:
		setFunc="&CPU::setRegisterValue"'''
		
	opcodeLines+="{}{{{},{},{},{},{}}};\n".format(opcode,funcName,func,parm1,parm2,cycles)

opcodesFile=open("opcodesFile.txt","w+")
opcodesFile.write(opcodeLines)
opcodesFile.close()