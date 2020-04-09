codeFile=open("code.txt","r+")
codeLines=codeFile.readlines()
outputCode=""
def findAllFlagSeting(line):
	flagSetting=[]
	for i in range(len(line)):
		k=line.find('setFlag(')
		print(k)

def changeFlagsUpdate(line):
	flags=['Z','N','H','C']
	for flag in flags:
		line=line.replace("resetFlag('{}')".format(flag),"flagsArray[{}] = 0".format(flag))
		line=line.replace("setFlag('{}')".format(flag),"flagsArray[{}] = 1".format(flag))
	return line
for i in range(len(codeLines)):

	codeLine=codeLines[i]
	if 'setFlag(' in codeLine or 'resetFlag(' in codeLine:
		startSet=codeLine.find('setFlag(')
		startRes=codeLine.find('resetFlag(')
		start=0;
		if startRes<0:
			start=startSet
		elif startSet<0:
			start=startRes
		else:
			start=min(startRes,startSet)
		outputCode+=codeLine[:start]+'//'+codeLine[start:]
		outputCode+='\n'+changeFlagsUpdate(codeLine)
	else:
		outputCode+=codeLine

outputCode+='\n'+'updateFlags();'

print(outputCode)
outputFile=open('outputFile.txt','w+')
outputFile.write(outputCode)
	#findAllFlagSeting(codeLine)
#if 'setFlag(' or 'resetFla(':
'''k=0
while codeLine[k] in [' ','\t']:
	k+=1
#print(codeLine[k:])
if codeLine[k:k+8]=='setFlag(':
	print(codeLine)
elif codeLine[k:k+10]=='resetFlag(':
	print(codeLine)
else:
	outputCode+=codeLine.replace('')'''