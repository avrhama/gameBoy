opcodesCloseList=[]
mapFile=open('map.txt','r+')
argsMap={}
for line in mapFile.readlines():
	tokens=line.replace("\n","").split("=")
	argsMap[tokens[0]]=tokens[1]
mapFile.close()
instructionsFile=open('instructionsSet.txt','r+')
instructionsLines=instructionsFile.readlines()
instructionsFile.close()
instructionTemplate='opcodes[0x{}] = { "{}",&Cpu::{},{},{},{} };\n'
code=""
for i in range(len(instructionsLines)):
	instructionLine=instructionsLines[i].replace("\n","")
	if instructionLine[0]=="#":
		continue
	elif instructionLine[0]=="*":
		instructionName=instructionLine[1:]
		print('name:',instructionName)
		code+='//{}\n'.format(instructionName)
	else:
		opcodeLineTokens=instructionLine.split(" ")
		#LD (HL),L 75 8
		args,opcode,cycles=opcodeLineTokens[1].split(","),opcodeLineTokens[2],opcodeLineTokens[3]

		if not opcode in opcodesCloseList:
			#opcodes[0x06] = { "LD",&Cpu::LD_nn_n,(uint16_t*)&this->b,NULL,8 };
			arg1,arg2=argsMap[args[0]],argsMap[args[1]]
			opcodeLine='opcodes[0x{}] = [ "{}",&Cpu::{},{},{},{}];\n'.format(opcode,instructionName,instructionName,arg1,arg2,cycles)
			code+=opcodeLine
			opcodesCloseList.append(opcode)
print(len(instructionsLines))
codeFile=open('codeFile.txt','w+')
codeFile.write(code)
codeFile.close()