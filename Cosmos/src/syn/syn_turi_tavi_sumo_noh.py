#!env python
import monsoon
import sys
import threading
import time
import socket
import sys
import tty, termios
import struct
import binascii
import random
import ctypes
import numpy as np
import os
import math
import subprocess
from battery import SaveBatteryFileHeader, SaveBatteryDistribution, SaveBatteryParams

exitFlag = 1
stDataColl = 0
collectedSamples = []
logTime = 60 # in Seconds

N_INITIAL_SAMPLES = 1000

constfile = "const.m"
batterycppfile = "exbat.cpp"
resultfile = "result.m"
markersFileName = 'markers.txt'
mat2py = "mat2py.dat"
py2mat = "py2mat.dat"

matlabPath = "/Applications/MATLAB_R2014a.app/bin/matlab"
#simmPar = "run('~/Projects/Cosmos/src/syn/sumo-toolbox_DDD/experiments/PM_TURI_TAVI.m');"
simmPar = "run('~/Documents/Cosmos/src/syn/sumo-toolbox_DDD/experiments/PM_TURI_TAVI.m');"
monsoonDeviceName = "/dev/tty.usbmodemfa131"

# modelName = "HeartModel_costs.slx"
# modelNameAllGrml = "HeartModelAll_costs.grml"
# fullmodelName = "HeartModelAll_costs.slx"
modelName = "HeartModel.slx"
modelNameAllGrml = "HeartModelAll.grml"
fullmodelName = "HeartModelAll.slx"
propName = "waitDepleted.lha"
cosmosName = "Cosmos"
modelConvertName = "modelConvert"

gK = 0.016890168
gc = 0.166
gC = 16.66

# 0 = ID, 1 = min, 2 = max, 3 = set value
parVals = [[0, 1000, 2000, 1000], [1, 20000, 10400], [2, 100, 30000, 3000], [0, 10, 2000, 500], [1, 10, 500, 131], [2, 10, 1200, 1000]]
parDict = {'SA_d':0, 'SA_ectopD':1, 'VRG_d':2, 'TURI':3, 'TAVI':4, 'TLRI':5}
parNameC = "SA_d"
parNameA = "TURI"
parNameAA = "TAVI"
parNameF = ["TURI", "TAVI"]

def get_my_string(fp):
    f = open(fp, 'r')
    string = str(f.read())
    f.close()
    return string

def GetReward(cname, modelname, propName):
    cmd=cname
    cmd += " "+modelname
    cmd += " "+propName
    #cmd += format(" --const \"SA_d=%f\""%x)
    cmd += " --batch 100 --max-run 5000 --njob 2"
    print(cmd+"\n")
    os.system(cmd)
    os.system("grep \"Estimated value\" Result_waitDepleted.res | sed \"s/Estimated value:\t//g\" > tmpResult")
    v = eval(get_my_string("tmpResult"))
    return v

#$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
os.system("rm "+constfile)
os.system("rm "+modelNameAllGrml)
#$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

XPace = np.array([0])
#YPace = np.array([0])
XYPace = np.array([0])

# Open Matlab with no GUI
subprocess.Popen([matlabPath, "-nodisplay", "-nosplash", "-nodesktop", "-r",simmPar])
py2matFile = open(py2mat,'w')
mat2pyFile = open(mat2py,'r')

iters = 0
while len(XPace)<N_INITIAL_SAMPLES:

	# Matlab code
	pipeline = mat2pyFile.readline()

	print pipeline

	if pipeline=='':
		continue
		
	parValueArduino = int(pipeline.split(',')[0])
	parValueArduinoAA = int(pipeline.split(',')[1])	

	# Set the heart parameter
	parVals[parDict[parNameC]][3] = 1500 # SA_d = 1500

	# Save energy readings
	fileconst = open(constfile, 'w+')
	fileconst.write(parNameA+" = "+str(parVals[parDict[parNameA]][3])+"\n")
	fileconst.write(parNameAA+" = "+str(parVals[parDict[parNameAA]][3])+"\n")
	fileconst.write(parNameC+" = "+str(parVals[parDict[parNameC]][3])+"\n")
	fileconst.close()
		
	print "Start iteration: "+str(iters)

	os.system(modelConvertName+" "+fullmodelName+"  --add-reward --grml")
	energyValue = -1.0*(GetReward(cosmosName, modelNameAllGrml, propName))

	# Write to pipe (Matlab)
	py2matFile.write(str(energyValue)+'\n\n')
	py2matFile.flush()

	XPace = np.vstack((XPace,parVals[parDict[parNameA]][3]))
	XYPace = np.vstack((XYPace,parVals[parDict[parNameAA]][3]))
	#YPace = np.vstack((YPace,energyValue))

	collectedSamples = []
	iters = iters + 1

