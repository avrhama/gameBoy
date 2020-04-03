import re
opcodesCloseList=[]
instructionsFile=open('instructionsSet.txt','r+')
instructionsLines=instructionsFile.readlines()
instructionsFile.close()
opcodeLines=""
for i in range(len(instructionsLines)):
	line=instructionsLines[i]
	startArs=line.index('{')
	opcode=line[:startArs]
	#print(opcode)
	args=line[startArs+1:-3].split(',')
	funcName,func,parm1,parm2,cycles=args[0],args[1],args[2],args[3],args[4]
	if '$' in parm1:
		setFunc="&CPU::setMemoryValue"
	else:
		setFunc="&CPU::setRegisterValue"
		
	opcodeLines+="{}{{{},{},{},{},{},{}}};\n".format(opcode,funcName,func,parm1,parm2,setFunc,cycles)

opcodesFile=open("opcodesFile.txt","w+")
opcodesFile.write(opcodeLines)
opcodesFile.close()