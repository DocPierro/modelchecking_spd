#!env python
import monsoon
import sys
import threading
import time
import socket
import tty, termios
import struct
import binascii
import random
import ctypes
import numpy as np
import os
import math
import subprocess
from battery import * 
from comm import * 

idCurr = []
useVM = 0
useMATLABGUI = 1
HOST = 'localhost'			# The remote host
PORT = 27778				# The same port as used by the serve

logTime = 20 # in Seconds

n_samples = 20

constfile = "const.m"
batterycppfile = "exbat.cpp"
resultfile = "result.m"
markersFileName = 'markers.txt'
mat2py = "/Users/paoletti/work/gitFolders/pacemaker/PaceMakerModel/mat2py.dat"
py2mat = "/Users/paoletti/work/gitFolders/pacemaker/PaceMakerModel/py2mat.dat"

matlabPath = "/Applications/MATLAB_R2014a.app/bin/matlab"
#simmPar = "run('~/Projects/Cosmos/src/syn/sumo-toolbox_DDD/experiments/PM_TURI_TAVI.m');"
simmPar = "run('~/work/gitFolders/Cosmos/src/syn/sumo-toolbox_DDD/experiments/PM_TURI_TAVI.m');"
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
parVals = [[0, 1000, 2000, 1000], [1, 20000, 10400], [2, 100, 30000, 3000], [0, 10, 2000, 500], [1, 10, 500, 131], [2, 10, 1200, 1000], [6, 10, 70, 40]]
parDict = {'SA_d':0, 'SA_ectopD':1, 'VRG_d':2, 'TURI':3, 'TAVI':4, 'TLRI':5, 'AV_Vt':6}
# parNameC = "SA_d"
parNameC = "AV_Vt"
parNameA = "TURI"
parNameAA = "TAVI"
parNameF = ["TURI", "TAVI"]

# Set the heart parameter
# parVals[parDict[parNameC]][3] = 1500 # SA_d = 1500 (bradycardia)
parVals[parDict[parNameC]][3] = 10 # AV_Vt = -10 (wenckheback)

def get_my_string(fp):
    f = open(fp, 'r')
    string = str(f.read())
    f.close()
    return string


#$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
os.system("rm "+constfile)
os.system("rm "+modelNameAllGrml)
#$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

XPace = np.array([0])
YPace = np.array([0])
XYPace = np.array([0])

# Open Matlab with no GUI
if useMATLABGUI==1:
	subprocess.Popen([matlabPath, "-nosplash", matlabPath,"-r",simmPar])
else:	
	subprocess.Popen([matlabPath, "-nodisplay", "-nosplash", "-nodesktop", "-r",simmPar])
py2matFile = open(py2mat,'w')
mat2pyFile = open(mat2py,'r')

iters = 0

if useVM==0:
	mon = monsoon.Monsoon("/dev/tty.usbmodem1411")

	mon.SetVoltage(4.55)
	mon.SetUsbPassthrough(0)

	monItems = mon.GetStatus()
	items = sorted(monItems.items())

	print "\n".join(["%s: %s" % item for item in items])

	mon.StopDataCollection()

	# Create new threads
	threadMonitor = PowerMonitorThread(mon)

	# Start new Threads
	threadMonitor.start()

	s = connectToSimulator(HOST,PORT);

	print "Preparing the PowerMonitor device..."
	time.sleep(4);




while len(XPace)<n_samples:

	# Matlab code
	pipeline = mat2pyFile.readline()

	print pipeline

	if pipeline=='':
		continue
		
	parValueArduino = int(pipeline.split(',')[0])
	parValueArduinoAA = int(pipeline.split(',')[1])	
	
	parVals[parDict[parNameA]][3] = parValueArduino
	parVals[parDict[parNameAA]][3] = parValueArduinoAA

	if useVM==0:
		# set first controller parameter
		if SetArduinoParameter(s, parVals[parDict[parNameA]][0], parVals[parDict[parNameA]][3])!=1:
			break
		# set second controller parameter
		if SetArduinoParameter(s, parVals[parDict[parNameAA]][0], parVals[parDict[parNameAA]][3])!=1:
			break
		# set client parameter
		if SetClientParameter(s, parVals[parDict[parNameC]][0], parVals[parDict[parNameC]][3])!=1:
			break;



	# Save energy readings
	fileconst = open(constfile, 'w+')
	fileconst.write(parNameA+" = "+str(parVals[parDict[parNameA]][3])+"\n")
	fileconst.write(parNameAA+" = "+str(parVals[parDict[parNameAA]][3])+"\n")
	fileconst.write(parNameC+" = "+str(parVals[parDict[parNameC]][3])+"\n")
	# fileconst.close()
		
	if useVM==0:
		# Start iteration
		s.sendall('\xF0')
		threadMonitor.setStDataColl(1)
		time.sleep(logTime);
		# Stop iteration
		s.sendall('\xF1')
		time.sleep(1);
		threadMonitor.setStDataColl(0)

		cummCurrentList = GetTotalCurrents(s, threadMonitor.getCollectedSamples(), monItems['sampleRate'])

		if len(cummCurrentList)==0:
			break

		# Save energy readings to list	
		for item in cummCurrentList:
			# idCurr.append([item[0],item[2]])
			idCurr.append([item[0],item[1],item[2]])

		SaveBatteryDistribution(fileconst, idCurr)
		fileconst.close()

		energyValue = GetSumCurrent(threadMonitor.getCollectedSamples(), monItems['sampleRate'])
	
	else:
		os.system(modelConvertName+" "+fullmodelName+"  --add-reward --grml")
		energyValue = -1.0*(GetReward(cosmosName, modelNameAllGrml, propName))

	XPace = np.vstack((XPace,parVals[parDict[parNameA]][3]))
	XYPace = np.vstack((XYPace,parVals[parDict[parNameAA]][3]))
	YPace = np.vstack((YPace,energyValue))

	print XPace
	print YPace
	os.system("rm "+constfile)

	threadMonitor.resetCollectedSamples()
	# collectedSamples = []

	# Write to pipe (Matlab)
	py2matFile.write(str(energyValue)+'\n\n')
	py2matFile.flush()

	iters = iters + 1

s.close()
