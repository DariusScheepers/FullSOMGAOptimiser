def readFile(fileName):
    file = open(fileName, "r")
    fileLines = file.read()
    lines = []
    currentRead = ""
    outline = ""
    for line in fileLines:
        for character in line:
            if (character == '\n'):
                lines.append(outline)     
                outline = ""
                continue          
            outline = outline + character
    file.close()
    return lines

def writeToFile(fileName, lines):
    file = open(fileName, "w")
    for line in lines:
        file.write(line)
    file.close()

def interpretLine(inputLine):
    separator = " "
    a0 = 0
    a0Read = False
    a1 = 0
    a1Read = False
    a2 = 0
    a2Read = False
    a3 = 0
    a3Read = False
    a4 = 0
    a4Read = False
    a5 = 0
    a5Read = False
    a6 = 0
    a6Read = False
    skip = True
    currentRead = ""
    for character in inputLine:
        if skip:
            currentRead = ""
            skip = False
            continue
        if character == separator and not a0Read:
            a0 = int(currentRead)
            a0Read = True
            currentRead = ""
            continue
        if character == separator and not a1Read:
            a1 = int(currentRead)
            a1Read = True
            currentRead = ""
            continue
        if character == separator and not a2Read:
            a2 = int(currentRead)
            a2Read = True
            currentRead = ""
            continue
        if character == separator and not a3Read:
            a3 = int(currentRead)
            a3Read = True
            currentRead = ""
            continue
        if character == separator and not a4Read:
            a4 = int(currentRead)
            a4Read = True
            currentRead = ""
            continue
        if character == separator and not a5Read:
            a5 = int(currentRead)
            a5Read = True
            currentRead = ""
            continue
        if character == separator and not a6Read:
            a6 = int(currentRead)
            a6Read = True
            currentRead = ""
            break
        currentRead = currentRead + character
    outLine = ""
    outLine = outLine + convertToBinary(a1, 1, 3)
    outLine = outLine + convertToBinary(a2, 1, 3)
    outLine = outLine + convertToBinary(a3, 1, 2)
    outLine = outLine + convertToBinary(a4, 1, 3)
    outLine = outLine + convertToBinary(a5, 1, 4)
    outLine = outLine + convertToBinary(a6, 1, 2) + "\n"
    return outLine    
    
def convertToBinary(number, min, length):
    out = ""
    for i in range(min, length + 1):
        if i == number:
            out = out + "1,"
        else:
            out = out + "0,"
    return out
    
# Main
def Main():
    for i in range(1, 3 + 1):
        fileNumber = str(i)
        inputFileName = "./Monks/monks-" + fileNumber + ".test"
        outputFileName = "./Converted/monks" + fileNumber + ".data"
        lines = readFile(inputFileName)
        output = []
        for line in lines:
            interpretedOut = interpretLine(line)
            output.append(interpretedOut)
        writeToFile(outputFileName, output)

Main()
