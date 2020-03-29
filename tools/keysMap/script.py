keysText=open('keys.txt','r+')
keysEnum='enum CartridgeTypse {'
lines=keysText.readlines()
for i in range(0,len(lines),3):
	firstLineTokens=lines[i].strip().split(" ")
	keyName,keyValue=firstLineTokens[0],firstLineTokens[1]
	keyComment=lines[i+2].strip()
	keysEnum+='{}={},//{}\n'.format(keyName,keyValue,keyComment)
keysEnum+='\n};'
keysEnumFile=open("keysEnumFile.txt",'w+');
keysEnumFile.write(keysEnum)
keysEnumFile.close()