import os 
import re
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

JAVASIGN = "Java_time"
CPPSIGN = "cpp_time"
QSIGN = "q_time"

logPost = ".log"
paths = ["tests/benchmark/CPP/out","tests/benchmark/Java/out"]

rep = 5

javadata = {}
cppdata = {}

def findIndex(list, str):
    for i in list:
        if i == str:
            return i
    return -1

def paintDig(dataFile, diagramFile, title):
    algorithem = []
    checkType = []
    data = {}

    file = open(dataFile, 'r') 
    for line in file.readlines():
        sp = line.split(' ')
        list = filter(lambda x: x!='', sp)
        list = filter(lambda x: x!='\n', sp)
        if len(list) == 0:
            continue
        if len(list) == 1:
            alg = list[0]
            algorithem.append(alg)
        else:
            checkT = list[0]
            if not checkT in checkType:
                checkType.append(checkT)
            
    for ct in checkType:
        data[ct] = [0]*len(algorithem)

    file.close()
    
    file = open(dataFile, 'r') 
    alg = ""
    for line in file.readlines():
        sp = line.split(' ')
        list = filter(lambda x: x!='', sp)
        list = filter(lambda x: x!='\n', sp)
        if len(list) == 0:
            continue
        if len(list) == 1:
            alg = list[0]
        else:
            checkT = list[0]
            avg = list[len(list)-1]
            if "avg:" in avg:
                avg = avg.replace("avg:","")
                avg = avg[0 : len(avg)-2]
                avg = float(avg)
                arr = data[checkT]
                arr[algorithem.index(alg)]=avg
    file.close()

    algorithem = tuple(algorithem)
    bar_width = 0.7
    startIndex = 0
    widthGap = bar_width*(len(checkType)+1)
    for ct in checkType:
        list = data[ct]
        widthIndex = [0]*len(list)
        for i in range(len(widthIndex)):
            widthIndex[i] = startIndex + i * widthGap

        startIndex = startIndex + bar_width
        list = np.array(list)
        plt.bar(x=widthIndex, height=list, width=bar_width, label=ct)
    
    labelIndex = [0]*len(algorithem)
    labelIndex[0] = len(algorithem)*bar_width/2
    for i in range(len(labelIndex)):
        if i>0:
            labelIndex[i] = labelIndex[i-1] + widthGap

    plt.legend() 
    plt.xticks(labelIndex, algorithem) 
    plt.ylabel('the time spend with different check methods (ms)') 
    plt.title(title)
    plt.savefig(diagramFile)
    plt.show()


def printTxt(dataFile, dateDic):
    f = open(dataFile,'w')
    
    for algorithem in dateDic:
        
        f.write( algorithem + ": \n" )

        newDic1 = dateDic[algorithem]  # which algorithem search or sort
        
        hasCOrJ = False
        for checkType in newDic1:      # shich check type, dy or ndy

            newDic2 = newDic1[checkType]

            for language in newDic2:    # cpp? java? or q
                if ("Java" in language or "cpp" in language) and hasCOrJ==False:
                    f.write("%-15s" % (language + ": ") )

                    sum = 0  
                    count = 0
                    for num in newDic2[language]:
                        f.write( "%-10.2f" % float(num) )
                        sum = sum + float(num)
                        count = count + 1
                    
                    f.write("avg:"+str(sum/count)) 
                    f.write("\n")
                    hasCOrJ = True

                elif (not ("Java" in language)) and (not ("cpp" in language)):
                    f.write("%-15s" % (checkType + ": ") )

                    sum = 0  
                    count = 0
                    for num in newDic2[language]:
                        f.write( "%-10.2f" % float(num) )
                        sum = sum + float(num)
                        count = count + 1
                    
                    f.write("avg:"+str(sum/count))  
                    f.write("\n")
        f.write("\n")      

    f.close()


def getDic(dataDic, fileRead, SIGN, file):
    time = []

    i = 0
    while i < rep:
        timeline = fileRead.readline()
        list = re.findall(r"\d+\.?\d*", timeline)
        
        if len(list)==1:
            time.append(list[0])
        i = i + 1

    dataDic[SIGN] = time


def getData( dataDic , path, COMPSIGN):
    for file in os.listdir(path):  
        if logPost in file: # find log file
            file_path = os.path.join(path, file) #get file route

            check_type = file.split('.')[1]      # get check type as subkey
            file = file.split('.')[0]            # get algorithem name as key

            if not (dataDic.has_key(file)):
                dataDic[file] = {}
            dic = dataDic[file]

            fileRead = open(file_path) 
            while 1:
                line = fileRead.readline()

                if not line:
                    break

                if not (COMPSIGN in line or QSIGN in line): #filter waring message
                    continue

                if not (dic.has_key(check_type)):
                    dic[check_type] = {}
                subdic = dic[check_type]

                if COMPSIGN in line:
                    getDic(subdic, fileRead, COMPSIGN, file)
                if QSIGN in line:
                    getDic(subdic, fileRead, QSIGN, file)

            fileRead.close()



for path in paths:
    if "CPP" in path :
        getData( cppdata , path, CPPSIGN)
    if "Java" in path:
        getData( javadata , path, JAVASIGN)

dataFile = "diagramCpp.txt"
diagramFile = "diagramCpp.png"
title = "comparition between the performance of cpp and q"
printTxt(dataFile, cppdata)
#paintDig(dataFile, diagramFile, title)

dataFile = "diagramJava.txt"
diagramFile = "diagramJava.png"
title = "comparition between the performance of java and q"
printTxt(dataFile, javadata)
paintDig(dataFile, diagramFile, title)
