
enumFile=open("enum.txt","r+")
enumLines=enumFile.readlines()

switchCode='switch(cartridgeType){\n'
for i in range(len(enumLines)):
	enumLine=enumLines[i].replace('\n','').strip()
	enumTokens=enumLine.split(',')

	for j in range(len(enumTokens)):
		enumToken=enumTokens[j].split('=')
		enum=enumToken[0].strip()
		if len(enum)>0:
			#print(enum)
			switchCode+='\tcase CartridgeType::{}:\n\t\tprintf("{}\\n");\n\t\tbreak;\n'.format(enum,enum)

switchCode+='}'
print(switchCode)
