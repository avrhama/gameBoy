file=open("interrupt.txt","r+")
lines=file.readlines()
output="switch(address){\n  "
for i in range(len(lines)):
	line=lines[i]
	opcode=line[3:7]
	comment=line[16:-1]
	output+=' case {}:{{\n   printf("opcode:{} {} value:%04x\\n",value);\n   break;\n }}'.format(opcode,opcode,comment)

output+='\n}'
outputFile=open('output.txt','w+')
outputFile.write(output)
outputFile.close()