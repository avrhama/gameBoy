opcodes=open("opcodes.txt","r+")
opcodesLines=opcodes.readlines()
opcodesList=[]
opcodesMap={}
argsList=[]
for i in range(len(opcodesLines)):
	line=opcodesLines[i].replace('\n','')
	if line.startswith('opcodes['):	
		opcode=line[8:line.index(']')]
		args=line[line.index('{')+1:line.index('}')].split(',')
		func,param1,param2,cycles=args[1],args[2],args[3],args[4]
		func=func[6:]
		param1=param1[6:]
		param2=param2[6:]
		#opcodesList.append({'opcode':int(opcode,16),'func':func,'param1':param1,'param2':param2,'cycles':cycles})
		opcodesMap[int(opcode,16)]={'opcode':int(opcode,16),'func':func,'param1':param1,'param2':param2,'cycles':int(cycles)}
		if param1 not in argsList:
			argsList.append(param1)
		if param2 not in argsList:
			argsList.append(param2)
		#print(func,param1,param2,cycles)
'''print(opcodesList[0])
opcodesList=sorted(opcodesList, key=lambda opcodeCse: opcodeCse['opcode'])
print(opcodesList[0])'''

cyclesFile=open("cycles1.txt","r+")
cycelsLines=cyclesFile.readlines()
for prefix in range(16):
	cycelsRow=cycelsLines[prefix].replace('\n','').split(',')
	for suffix in range(16):
		opcode=prefix*16+suffix
		if opcode in opcodesMap.keys():
			cycles=int(cycelsRow[suffix])*4
			if cycles!=opcodesMap[opcode]['cycles']:
				print(hex(opcode),opcodesMap[opcode]['func'],'me:',opcodesMap[opcode]['cycles'],'other:',cycles)
				#input('press to continue')
			opcodesMap[opcode]['cycles']=int(cycelsRow[suffix])


cyclesFile=open("cycles2.txt","r+")
cycelsLines=cyclesFile.readlines()
for prefix in range(16):
	cycelsRow=cycelsLines[prefix].replace('\n','').split(',')
	for suffix in range(16):
		opcode=prefix*16+suffix+0xcb00
		if opcode in opcodesMap.keys():
			cycles=int(cycelsRow[suffix])*4
			if cycles!=opcodesMap[opcode]['cycles']:
				print(hex(opcode),opcodesMap[opcode]['func'],'me:',opcodesMap[opcode]['cycles'],'other:',cycles)
				#input('press to continue')
			opcodesMap[opcode]['cycles']=int(cycelsRow[suffix])

opcodesList=list(opcodesMap.values())
print(opcodesList[0])
opcodesList=sorted(opcodesList, key=lambda opcodeCase: opcodeCase['opcode'])
#print(argsList)
argsMapFile=open("argsMap.txt","r+")
argsMapLines=argsMapFile.readlines()
argsMapFileLen=len(argsMapLines)
argsMap={}
for i in range(argsMapFileLen):
	argLine=argsMapLines[i].replace('\n','')
	if argLine[-1]==':':
		k=i+1
		linesCode=[]
		while k<argsMapFileLen and not ':' in argsMapLines[k]:
			linesCode.append(argsMapLines[k].replace('\n',''))
			k+=1
		i=k
		argsMap[argLine[:-1]]=linesCode

print(argsMap['getNULL'])

switchCases='switch(opcode){\n'
opcodesListLen=len(opcodesList)
for i  in range(opcodesListLen):
	data=opcodesList[i]
	case='		case {}:\n'.format(hex(data['opcode']))

	case+='			lastOpcodeCycles={};\n'.format(data['cycles'])
	param1=argsMap[data['param1']]
	for k in range(len(param1)-1):
		case+='			{}\n'.format(param1[k])
	param2=argsMap[data['param2']]
	for k in range(len(param2)-1):
		case+='			{}\n'.format(param2[k])
	
	#case+='			{}({},{});\n'.format(data['func'],param1[-1],param2[-1])
	case+='			param1={};\n'.format(param1[-1])
	case+='			param2={};\n'.format(param2[-1])
	case+='			{}(param1,param2);\n'.format(data['func'])
	case+='			break;\n'.format(data['func'],param1[-1],param2[-1])
	switchCases+=case
	if param1[-1]==param2[-1]:
		print('same:opcode:',hex(data['opcode']),param1[-1],param2[-1])
switchCases+='}'
#print(switchCases)
switchCasesFile=open('switchCases.txt','w+')
switchCasesFile.write(switchCases)
switchCasesFile.close()

