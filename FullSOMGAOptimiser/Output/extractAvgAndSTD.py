import os
import re
import math
from decimal import Decimal

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

def filterOutForTrainingSets(lines):
    startingRead = "Training 10"
    endRead = "Training 39"
    filteredLines = filterOutLines(startingRead, endRead, lines)
    return filteredLines

def filterOutForTestSets(lines):
    startingRead = "Test 10"
    endRead = "Test 39"
    filteredLines = filterOutLines(startingRead, endRead, lines)
    return filteredLines

def filterOutLines(startingRead, endRead, lines):
    filteredLines = []
    startReading = False
    for line in lines:
        if not startReading and re.search(startingRead, line):
            startReading = True
            filteredLines.append(line)
            continue
        if re.search(endRead, line):
            filteredLines.append(line)
            break
        stringLength = len(line)
        if startReading and stringLength > 0:
            filteredLines.append(line)
    return filteredLines

def calculateAverage(numbers):
    sum = 0
    for number in numbers:
        sum += number
    average = sum / len(numbers)
    return average

def calculateStandardDeviation(numbers):
    mean = calculateAverage(numbers)
    sum = 0
    for number in numbers:
        difference = number - mean
        power = difference**2
        sum += power
    variance = sum / len(numbers)
    std = math.sqrt(variance)
    return std

def trimUnnecessaryDataOfLines(lines):
    numbers = []
    for line in lines:
        numbers.append(retrieveNumbers(line))
    return numbers

def retrieveNumbers(line):
    numbers = []
    currentRead = ""
    dotRead = False
    for char in line:
        if char == "A":
            break
        if not dotRead and char == '.':
            dotRead = True
        if dotRead:
            if char == " ":
                number = float(currentRead)
                numbers.append(number)
                currentRead = ""
                continue
            currentRead = currentRead + char
    return numbers

def getAvgAndStd(setLines, description):
    numberData = trimUnnecessaryDataOfLines(setLines)
    qeErrors = []
    for numbers in numberData:
        average = calculateAverage(numbers)
        qeErrors.append(average)
    average = calculateAverage(qeErrors)
    standardDeviation = calculateStandardDeviation(qeErrors)
    return ["\n" + description + " Average: " + str(round(average, 8)), "\n" + description + " Std: " + str(round(standardDeviation, 8))]

#Main
def Main():
    for history in histories:
        method = history[0]
        inputFileName = history[1]
        lines = readFile("./" + method + "/" + inputFileName)
        trainingSetLines = filterOutForTrainingSets(lines)
        output = getAvgAndStd(trainingSetLines, method + " Training")
        testSetLines = filterOutForTestSets(lines)
        output.extend(getAvgAndStd(testSetLines, method + " Test"))
        filename = "./Converted/" + method + "OUT.txt"
        writeToFile(filename, output)

histories = [["WinIrisGA2", "07-10-2019-110027_TestAndTrainingSetQEHistories_iris.data.txt"], ["WinIrisSN", "06-10-2019-193616_TestAndTrainingSetQEHistories_iris.data.txt"], ["WinIonosphereGA", "11-10-2019-162302_TestAndTrainingSetQEHistories_ionosphere.data.txt"], ["WinIonosphereSN","11-10-2019-223348_TestAndTrainingSetQEHistories_ionosphere.data.txt"],["WinMonks1GA", "20-10-2019-185552_TestAndTrainingSetQEHistories_monks1.data.txt"], ["WinMonks1SN", "09-10-2019-160437_TestAndTrainingSetQEHistories_monks1.data.txt"], ["WinMonks2GA", "16-10-2019-114014_TestAndTrainingSetQEHistories_monks2.data.txt"], ["WinMonks2SN", "09-10-2019-090631_TestAndTrainingSetQEHistories_monks2.data.txt"], ["WinMonks3GA", "13-10-2019-151815_TestAndTrainingSetQEHistories_monks3.data.txt"], ["WinMonks3SN", "09-10-2019-092603_TestAndTrainingSetQEHistories_monks3.data.txt"], ["WinPimaGA", "10-10-2019-112510_TestAndTrainingSetQEHistories_pima.data.txt"], ["WinPimaSN", "07-10-2019-181212_TestAndTrainingSetQEHistories_pima.data.txt"]]
inputFileNames = ["07-10-2019-110027_TestAndTrainingSetQEHistories_iris.data.txt", "06-10-2019-193616_TestAndTrainingSetQEHistories_iris.data.txt", "11-10-2019-162302_TestAndTrainingSetQEHistories_ionosphere.data.txt", "11-10-2019-223348_TestAndTrainingSetQEHistories_ionosphere.data.txt", "09-10-2019-160437_TestAndTrainingSetQEHistories_monks1.data.txt", "09-10-2019-090631_TestAndTrainingSetQEHistories_monks2.data.txt", "13-10-2019-151815_TestAndTrainingSetQEHistories_monks3.data.txt", "09-10-2019-092603_TestAndTrainingSetQEHistories_monks3.data.txt", "10-10-2019-112510_TestAndTrainingSetQEHistories_pima.data.txt", "07-10-2019-181212_TestAndTrainingSetQEHistories_pima.data.txt"]
Main()
