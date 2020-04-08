import re


opcodesFile=open("opcodes.txt","r+")
opcodesLines=opcodesFile.readlines()
opcodeMap={}
for line in opcodesLines:
	if line.startswith('opcodes'):
		opcodeEnd=line.index(']')
		opcode=int(line[8:opcodeEnd],16)
		opcodeMap[opcode]=line
	if 'SP' in line:
		'''tokens=line.split(',')
		if tokens[2]=='&CPU::get$N':'''


'''while True:
	opcode=input('insert opcode: ').lower().strip()
	opcode=opcode if opcode.startswith('0x') else ("0x"+opcode)
	opcode=int(opcode,16)
	if opcode in opcodeMap.keys():
		print(opcodeMap[opcode])'''
seenFunctions=[]
def f(cols,rowIndex):
	for i in range(1,17):
		if '&nbsp;&nbsp;' in cols[i]:
			start=cols[i].index('&nbsp;&nbsp;')+12
			if start!=-1:
				
				opcode=16*rowIndex+(i-1)+51968
				opcodeLine=opcodeMap[opcode]
				start=opcodeLine.index('{')+1
				args=opcodeLine[start:-4].split(',')
				if args[0] in seenFunctions:
					continue
				seenFunctions.append(args[0])
				start=cols[i].index('</td>')-7
				flags=cols[i][start:-5]
				print("opcode:",hex(opcode),flags)
				print(opcodeMap[opcode])
				input("press to continue...")
				

				continue
				cycles=cols[i][start:-16]
				
				#if opcode==203:
				#	continue
				if opcode in opcodeMap.keys():
					opcodeLine=opcodeMap[opcode]
					start=opcodeLine.index('{')+1
					args=opcodeLine[start:-4].split(',')
					cyclesArg=args[4]
					if cyclesArg!=cycles:
						print(opcodeLine,cyclesArg,'->',cycles)
						input('press to continue...')
						print('\n\n')
					return True
				else:
					print("Not found:",hex(opcode),opcode,cols[i])
					return False
					exit()
				#print(hex(15*rowIndex+(i-1)),cycles)

colEx=re.compile(".+(?P<col><td.+</td>)(.+)?")
colEx=re.compile("<td.*?td>")
cycleEx=re.compile("<b.*?br>")
cycleEx=re.compile("<b.*?&nbsp;/d+<br>")
table=open("opcodesTable2.txt","r+")
rows=table.readlines()
for i in range(len(rows)):
	row=rows[i]
	m=colEx.findall(row)
	f(m,i)