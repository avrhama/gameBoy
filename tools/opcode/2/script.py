opcodesFile=open("opcodes.txt","r+")
opcodesLines=opcodesFile.readlines()
opcodeMap={}
for line in opcodesLines:
	if line.startswith('opcodes'):
		opcodeEnd=line.index(']')
		opcode=int(line[8:opcodeEnd],16)
		opcodeMap[opcode]=line

while True:
	opcode=input('insert opcode: ').lower().strip()
	opcode=opcode if opcode.startswith('0x') else ("0x"+opcode)
	opcode=int(opcode,16)
	if opcode in opcodeMap.keys():
		print(opcodeMap[opcode])
