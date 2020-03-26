import re
opcodesCloseList=[]
mapFile=open('map.txt','r+')
argsMap={}
for line in mapFile.readlines():
	tokens=line.replace("\n","").split("=")
	argsMap[tokens[0]]=tokens[1]
argsMap[None]='NULL'
mapFile.close()
instructionsFile=open('instructionsSet.txt','r+')
instructionsLines=instructionsFile.readlines()
instructionsFile.close()
instructionTemplate='opcodes[0x{}] = { "{}",&Cpu::{},{},{},{} };\n'
opcodeMapcode=""
declarationsCode=""
definitionsCode=""
funcNameRe=re.compile(r'(?P<name>\w+)( )?(?P<arg1>\(?\w+\)?)?(,)?(?P<arg2>\(?\w+\)?)?')
funcCallRe=re.compile(r'(?P<name>\w+)( (?P<arg1>\(?[\w#\*\-\+]+\)?),?(?P<arg2>\(?[\w#\*\-\+]+\)?)?)? (?P<opcode>\w+) (?P<cycles>\w+)?')
def getInstructionTitle(x):
	m=funcNameRe.match(x)
	title=m.group('name')
	if m.group('arg1'):
		title+=' '+m.group('arg1')
	if m.group('arg2'):
		title+=','+m.group('arg2')
	return title

def getInstructionName(x):
	m=funcNameRe.match(x)
	name=m.group('name')
	
	arg1,arg2='none','none'
	if m.group('arg1'):
		arg1=m.group('arg1').replace('(','$')
		arg1=arg1.replace(')','')
		name+='_'+arg1
	if m.group('arg2'):
		arg2=m.group('arg2').replace('(','$')
		arg2=arg2.replace(')','')
		name+='_'+arg2
	declaration='{}(char* {},char* {})'.format(name,arg1,arg2)
	return name,declaration

	
for i in range(len(instructionsLines)):
	instructionLine=instructionsLines[i].replace("\n","")
	if instructionLine[0]=="#":
		continue
	elif instructionLine[0]=="*":
		instructionName=instructionLine[1:]
		
		title=getInstructionTitle(instructionName)
		instructionName,declaration=getInstructionName(instructionName)
		#print('title:',title,'name:',instructionName)
		opcodeMapcode+='//{}\n'.format(title)
		declarationsCode+='void {};\n'.format(declaration)
		definitionsCode+='void Cpu::{}{{\n}}\n'.format(declaration)
	else:
		opcodeLineTokens=instructionLine.split(" ")
		#LD (HL),L 75 8
		m=funcCallRe.match(instructionLine)
		opcode=m.group('opcode')
		if not opcode in opcodesCloseList:
			arg1,arg2=argsMap[m.group('arg1')],argsMap[m.group('arg2')]
			cycles=m.group('cycles')
			opcodeLine='opcodes[0x{}] = {{ "{}",&Cpu::{},{},{},{}}};\n'.format(opcode,instructionName,instructionName,arg1,arg2,cycles)
			opcodeMapcode+=opcodeLine
			opcodesCloseList.append(opcode)
print(len(instructionsLines))
opcodeMapcodeFile=open('opcodeMapcodeFile.txt','w+')
opcodeMapcodeFile.write(opcodeMapcode)
opcodeMapcodeFile.close()
declarationsCodeFile=open('declarationsCodeFile.txt','w+')
declarationsCodeFile.write(declarationsCode)
declarationsCodeFile.close()
definitionsCodeFile=open('definitionsCodeFile.txt','w+')
definitionsCodeFile.write(definitionsCode)
definitionsCodeFile.close()