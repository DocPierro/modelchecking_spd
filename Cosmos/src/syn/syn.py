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
import GPy
import math
from scipy.special import erf
import matplotlib
from IPython.display import display
from matplotlib import pyplot as plt

from fparams import isParamValuationFeasible

exitFlag = 1
stDataColl = 0
collectedSamples = []
useVM = 0
logTime = 10 # in Seconds
constfile = "const.m"
kernel = GPy.kern.RBF(input_dim=1, variance=1., lengthscale=1.)

N_INITIAL_SAMPLES = 20
N_OPT_STEPS = 1000
resultfile = "result.m"
tmpconstfile = "tmpconst.m"

XMAX = 1000.0
YMAX = 20000.0

def get_my_string(fp):
    f = open(fp, 'r')
    string = str(f.read())
    f.close()
    return string

def GetReward():
    cmd="Cosmos"
    cmd += " HeartModelAll.slx"
    cmd += " prop.lha"
    #cmd += format(" --const \"SA_d=%f\""%x)
    cmd += " --max-run 10 --batch 0 -v 0 --njob 2"
    print(cmd+"\n")
    os.system(cmd)
    os.system("grep -A 1 \"Total:\" Result.res | grep \"Estimated value\" | sed \"s/Estimated value:\t//g\" > tmpResult")
    v = eval(get_my_string("tmpResult"))
    return v

if len(sys.argv) > 1:
	useVM = 1


def SetArduinoParameter(handle, parID, parValue):
		headerID = 0xF5

		print "Sending Arduino parameter ID: "+str(parID)+" value: "+str(parValue)

		if SetParameter(handle, headerID, parID, parValue)!=1:
			return 0

		return 1

def SetClientParameter(handle, parID, parValue):
		headerID = 0xF4

		print "Sending Client parameter ID: "+str(parID)+" value: "+str(parValue)

		if SetParameter(handle, headerID, parID, parValue)!=1:
			return 0

		return 1

def SetParameter(handle, headerID, parID, parValue):
		parStr = struct.pack('BBI',headerID, parID, parValue)

		# Send parameter
		handle.sendall(parStr)

		buftmp = bytearray(1)
		buflen = handle.recv_into(buftmp,1)

		if buftmp[0]!=0xF6:
			print "Wrong return value"
			return 0

		return 1

def Save2DArray(handle, arr):
	for idx in range(len(arr)):
		handle.write(str(arr[idx][0])+" "+str(arr[idx][1])+"\n")

def SaveMarkersToFile(handle, dta):
	if dta==0:
		handle.write(str(0)+" "+str(0))
	elif dta==1:
		handle.write(str(1)+" "+str(0))
	elif dta==2:
		handle.write(str(0)+" "+str(1))
	elif dta==3:
		handle.write(str(1)+" "+str(1))

def SaveToFile(handle, dta):
	SaveMarkersToFile(handle, dta)
	handle.write("\n")

def SaveToFileAll(handle, dta, current):
	SaveMarkersToFile(handle, dta)
	handle.write(" "+str(current))
	handle.write("\n")

def GetMinCurrent(pmData):
	currList = [it[0] for it in pmData]

	return min(currList)

def GetSumCurrent(pmData, monitorSamplingFreq):
	sumCurrent = 0
	prev = pmData[0][1]

	it_beg = 0

	for it in range(1,len(pmData)):
		if pmData[it][1]!=2 and prev==2:
			it_beg = it
			break

	cnt = 0		
	for it in range(it_beg,len(pmData)):
		if pmData[it][1]==2 and cnt>=20:
			break
		elif pmData[it][1]==2 and cnt<20: 	
			cnt = cnt + 1
		elif prev==2 and pmData[it][1]!=2:
			cnt = 0
		sumCurrent = sumCurrent + pmData[it][0]
		prev = pmData[it][1]

	
	return sumCurrent

def GetEnergyReadings(pmData, monitorSamplingFreq, tranListTimes, minCurrent):
	cummCurrentList = []

	sumCurrent = 0
	sampleCnt = 1
	prev = pmData[0][1]
	
	timeList = [item[1] for item in tranListTimes]
	cummCurrentList = [[item[0],item[1],0] for item in tranListTimes]

	it_beg = 0

	for it in range(1,len(pmData)):
		if pmData[it][1]!=2 and prev==2:
			it_beg = it
			break

	tmpSampleCnt = 0
	timeListIdx = 0

	for it in range(it_beg,len(pmData)):
		stime = sampleCnt/monitorSamplingFreq
		sumCurrent = sumCurrent + pmData[it][0]
		tmpSampleCnt = tmpSampleCnt + 1

		if timeListIdx>=len(timeList):
			break

		if stime>=timeList[timeListIdx]:
			cummCurrentList[timeListIdx][2] = sumCurrent - tmpSampleCnt * minCurrent
			timeListIdx = timeListIdx + 1
			sumCurrent = 0
			tmpSampleCnt = 0

		sampleCnt = sampleCnt + 1

	
	filedta = open('markers.txt', 'w+')

	for it in range(it_beg,len(pmData)):
		SaveToFileAll(filedta, pmData[it][1], pmData[it][0])

	filedta.close()
	return cummCurrentList

def GetTotalCurrents(handle, esamples, monitorSamplingFreq):
		cummCurrentList = []

		# Send get list of IDs
		handle.sendall('\xF3')

		bufSize = bytearray(4)
		buflen = handle.recv_into(bufSize, 4)

		if buflen!=4:
			print "Wrong packet length"
			return cummCurrentList

		bufSize = struct.unpack("I",bufSize)

		print "Packet size: "+str(bufSize[0])

		bufIDs = bytearray()
		buftmp = bytearray(2048)
		bytes_recd = 0

		while bytes_recd < bufSize[0]:
			chunkLen = handle.recv_into(buftmp,2048)
			bufIDs[bytes_recd:] = buftmp[:chunkLen]
			bytes_recd = bytes_recd + chunkLen

		if 	bytes_recd != bufSize[0]:
			print "Wrong data"
			return cummCurrentList

		nTranCnt = 	bufSize[0]/8
		print "Number of transitions received: "+str(nTranCnt)

		tTimeList = []
		for idx in range(0,nTranCnt):
			tID = bufIDs[idx*8]
			tTime = struct.unpack("I",bufIDs[idx*8+1:idx*8+1+4])
			tTimeList.append([tID, tTime[0]])
			#print str(tTime[0])+" "+str(bufIDs[idx*8])

		toRemove = []	
		for idx in range(len(tTimeList)-1):
			if tTimeList[idx][1]==tTimeList[idx+1][1]:
				toRemove.append(tTimeList[idx])

		for rem in toRemove:
			tTimeList.remove(rem)

		minCurrent = GetMinCurrent(esamples)
		cummCurrentList = GetEnergyReadings(esamples, monitorSamplingFreq, tTimeList, minCurrent)

		return cummCurrentList

def SaveDistribution(handle, dData):
		dIdCurr = {}
		for key, val in dData:
			dIdCurr.setdefault(key, []).append(val)

		keyRange = range(53, 59)

		for key in keyRange:
			if key not in dIdCurr:
				handle.write("T"+str(key)+"_min"+" = "+str(0)+"\n")
				handle.write("T"+str(key)+"_max"+" = "+str(0)+"\n")
				handle.write("T"+str(key)+"_mean"+" = "+str(0)+"\n")
				handle.write("T"+str(key)+"_var"+" = "+str(0)+"\n")
				handle.write("T"+str(key)+"_list"+" = "+str([0])+"\n")
			else:
				handle.write("T"+str(key)+"_min"+" = "+str(min(dIdCurr[key]))+"\n")
				handle.write("T"+str(key)+"_max"+" = "+str(max(dIdCurr[key]))+"\n")
				handle.write("T"+str(key)+"_mean"+" = "+str(np.mean(dIdCurr[key]))+"\n")
				handle.write("T"+str(key)+"_var"+" = "+str(np.var(dIdCurr[key]))+"\n")
				handle.write("T"+str(key)+"_list"+" = "+str(dIdCurr[key])+"\n")

def normpdf(x):
	return np.exp(-x*x/2)/np.sqrt(2*math.pi)

def normcdf(x):
	return (1+ erf(x/np.sqrt(2)))/2

def findMax(m, X2, minv, maxv, fmin):
	mu,s2 = m.predict(X2)
	t = np.argmax((fmin-mu) * normcdf( (fmin-mu)/np.sqrt(s2) ) + np.sqrt(s2)*normpdf( (fmin-mu)/np.sqrt(s2) ))
	#t2 = minv + (maxv - minv)*t/len(X2)
	return X2[t]

class PowerMonitorThread (threading.Thread):
	def __init__(self, monitor):
		threading.Thread.__init__(self)
		self.monitor = monitor
	def run(self):
		print "Starting PowerMonitor Thread\n"
		self.monitor.StartDataCollection()

		while exitFlag:
			samples = self.monitor.CollectData()
			if stDataColl:
				collectedSamples.extend(samples)

				#print "\n".join(map(str, samples))

		self.monitor.StopDataCollection()

		print "Ending PowerMonitor Thread\n"

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

############################
#X = np.random.uniform(-3000.,3000.,(20,1))
#Y = np.sin(X/1000) + np.random.randn(20,1)*0.05
#kernel = GPy.kern.RBF(input_dim=1, variance=1., lengthscale=1.)
#m = GPy.models.GPRegression(X,Y,kernel)
#m.optimize_restarts(num_restarts = 10)
#m.plot()
#display(m)
#plt.savefig(format('gaussfig0'))

############################
HOST = 'localhost'			# The remote host
PORT = 27778				# The same port as used by the server

s = connectToSocket(HOST,PORT);


#s.setblocking(0)

idCurr = []
# 0 = ID, 1 = min, 2 = max, 3 = set value
parVals = [[0, 1000, 2000, 1000], [1, 20000, 10400], [2, 100, 30000, 3000], [0, 10, 2000, 500], [1, 10, 500, 131], [2, 200, 1200, 1000]]
parDict = {'SA_d':0, 'SA_ectopD':1, 'VRG_d':2, 'TURI':3, 'TAVI':4, 'TLRI':5}

XPace = np.array([0])
YPace = np.array([0])
XYPace = np.array([0])

#parNameC = "SA_d"
#parNameA = "TURI"
#parNameF = ["TLRI", "TAVI"]

parNameC = "SA_d"
parNameA = "TURI"
parNameF = ["TLRI", "TAVI", "TURI"]

if useVM==0:

	print "Preparing the PowerMonitor device..."
	time.sleep(4);

	print "Get initial values"
	logTime = 60
	for iters in range(0, N_INITIAL_SAMPLES):

		# Generate random parameter for pacemaker	
		parValueArduino = random.randint(parVals[parDict[parNameA]][1], parVals[parDict[parNameA]][2])

		# For testing
		#parValueArduino = 150

		tmpParArduino = parVals[parDict[parNameA]][3]

		parVals[parDict[parNameA]][3] = parValueArduino

		parll = [parVals[parDict[parNameF[0]]][3]-parVals[parDict[parNameF[1]]][3], parVals[parDict[parNameF[2]]][3]]
		if isParamValuationFeasible(parll)!=1:
			print "Pacemaker parameter not feasible: "+str(parll)
			parVals[parDict[parNameA]][3] = tmpParArduino
			continue

		if SetArduinoParameter(s, parVals[parDict[parNameA]][0], parVals[parDict[parNameA]][3])!=1:
			break

		# Generate random parameter for Client
		parVals[parDict[parNameC]][3] = random.randint(parVals[parDict[parNameC]][1], parVals[parDict[parNameC]][2])

		# For testing purposes
		#parValueClient = 2000

		if SetClientParameter(s, parVals[parDict[parNameC]][0], parVals[parDict[parNameC]][3])!=1:
			break;

		# Save energy readings
		fileconst = open(constfile, 'w+')
		fileconst.write(parNameA+" = "+str(parVals[parDict[parNameA]][3])+"\n")
		fileconst.write(parNameC+" = "+str(parVals[parDict[parNameC]][3])+"\n")
			
		#isParamValuationFeasible(param)	
		print "Start iteration: "+str(iters)

		# Start iteration
		s.sendall('\xF0')
		stDataColl = 1

		time.sleep(logTime);

		# Stop iteration
		s.sendall('\xF1')
		time.sleep(1);
		stDataColl = 0

		print "Stopped collecting data"

		cummCurrentList = GetTotalCurrents(s, collectedSamples, monItems['sampleRate'])

		if len(cummCurrentList)==0:
			break

		# Save energy readings to list	
		for item in cummCurrentList:
			idCurr.append([item[0],item[2]])

		SaveDistribution(fileconst, idCurr)
		fileconst.close()

		#energyValue = GetReward()
		energyValue = GetSumCurrent(collectedSamples, monItems['sampleRate'])

		XPace = np.vstack((XPace,parVals[parDict[parNameA]][3]))
		XYPace = np.vstack((XYPace,parVals[parDict[parNameC]][3]))
		YPace = np.vstack((YPace,energyValue))

		print XPace
		print YPace
		os.system("rm "+constfile)

		collectedSamples = []


	print "Initial sample:"	

	XPace = XPace[1:len(XPace)]
	YPace = YPace[1:len(YPace)]
	XYPace = XYPace[1:len(XYPace)]

	XYPace = np.hstack((XYPace,YPace))

	# For testing purposes	
	tmpfileconst = open(tmpconstfile, 'w+')
	Save2DArray(tmpfileconst, XYPace)	
	tmpfileconst.close()


	print "Optimize parameters"
	logTime = 60

	# Store the initial values of parameters
	rfhandle = open(resultfile, 'w+')
	rfhandle.close()

	rfhandle = open(resultfile, 'a')
	rfhandle.write("paramp = ["+"\n")

	for idx in range(len(XPace)):
		rfhandle.write(str(XPace[idx])+" "+str(YPace[idx])+";\n")

	rfhandle.close()

	for iters in range(0, N_OPT_STEPS):

		m = GPy.models.GPRegression(XPace/float(XMAX),YPace/float(YMAX),kernel)
		m.optimize_restarts(num_restarts = 20)

		Xin = np.linspace(parVals[parDict[parNameA]][1], parVals[parDict[parNameA]][2],num=1000).reshape((1000,1))

		Xf = np.array([0])

		for idx in range(len(Xin)):
			#parll = [parVals[parDict[parNameF[0]]][3]-parVals[parDict[parNameF[1]]][3], Xin[idx]]
			parll = [parVals[parDict[parNameF[0]]][3]-parVals[parDict[parNameF[1]]][3], Xin[idx]]
			if isParamValuationFeasible(parll)==1:
				Xf = np.vstack((Xf,Xin[idx]))

		Xf = Xf[1:len(Xf)]

		

		parValueArduino = findMax(m, Xf/XMAX, parVals[parDict[parNameA]][1]/XMAX, parVals[parDict[parNameA]][2]/XMAX, min(YPace/YMAX))

		tmpParArduino = parVals[parDict[parNameA]][3]

		parVals[parDict[parNameA]][3] = int(parValueArduino*XMAX)

		parll = [parVals[parDict[parNameF[0]]][3]-parVals[parDict[parNameF[1]]][3], parVals[parDict[parNameF[2]]][3]]
		if isParamValuationFeasible(parll)!=1:
			print "Pacemaker parameter not feasible: "+str(parll)

			parVals[parDict[parNameA]][3] = tmpParArduino
			continue

		
		if SetArduinoParameter(s, parVals[parDict[parNameA]][0], parVals[parDict[parNameA]][3])!=1:
			break

		# Generate random parameter for Client
		parVals[parDict[parNameC]][3] = random.randint(parVals[parDict[parNameC]][1], parVals[parDict[parNameC]][2])

		# For testing purposes
		#parValueClient = 2000

		if SetClientParameter(s, parVals[parDict[parNameC]][0], parVals[parDict[parNameC]][3])!=1:
			break;

		# Save energy readings
		fileconst = open(constfile, 'w+')
		fileconst.write(parNameA+" = "+str(parVals[parDict[parNameA]][3])+"\n")
		fileconst.write(parNameC+" = "+str(parVals[parDict[parNameC]][3])+"\n")
			
		#isParamValuationFeasible(param)	
		print "Start iteration: "+str(iters)

		# Start iteration
		s.sendall('\xF0')
		stDataColl = 1

		time.sleep(logTime);

		# Stop iteration
		s.sendall('\xF1')
		time.sleep(1);
		stDataColl = 0

		print "Stopped collecting data"

		cummCurrentList = GetTotalCurrents(s, collectedSamples, monItems['sampleRate'])

		if len(cummCurrentList)==0:
			break

		# Save energy readings to list	
		for item in cummCurrentList:
			idCurr.append([item[0],item[2]])

		SaveDistribution(fileconst, idCurr)
		fileconst.close()

		#energyValue = GetReward()
		energyValue = GetSumCurrent(collectedSamples, monItems['sampleRate'])

		XPace = np.vstack((XPace,parVals[parDict[parNameA]][3]))
		YPace = np.vstack((YPace,energyValue))

		os.system("rm "+constfile)

		collectedSamples = []

		m.plot()
		plt.plot(XPace,YPace,'bo')
		plt.xlabel('$\mathrm{TURI}$')
		plt.ylabel('$\mathrm{Energy}$')
		display(m)
		display(plt)
		plt.savefig(format('gaussfig%i'%iters))

		# Save to file
		rfhandle = open(resultfile, 'a')
		rfhandle.write(str(parValueArduino)+" "+str(energyValue)+";\n")
		rfhandle.close()

	s.sendall('\xF2')
	exitFlag = 0
	
	# Save to file
	rfhandle = open(resultfile, 'a')
	rfhandle.write("];"+"\n")
	rfhandle.close()

else:
	key = ''

	# Generate random parameter
	headerID = 0xF4
	parID = 1
	#parValue = random.randint(1, 2000)
	parValue = 2000

	print "Sending parameter ID: "+str(parID)+" value: "+str(parValue)

	parStr = struct.pack('BBI',headerID, parID, parValue)

	# Send parameter to Client
	s.sendall(parStr)

	buftmp = bytearray(1)
	buflen = s.recv_into(buftmp,1)

	if buftmp[0]!=0xF6:
		print "Wrong return value"
		s.close()
		exit()

	while key!='a':
		print "Press a key: "
		key = sys.stdin.read(1)

		if key=='w':
			s.sendall('\xF0')
			stDataColl = 1

		elif key=='s':
			s.sendall('\xF1')
			time.sleep(1);
			stDataColl = 0


			print "Stopped collecting data"

			# Send get list of IDs
			s.sendall('\xF3')

			bufSize = bytearray(4)
			buflen = s.recv_into(bufSize, 4)

			if buflen!=4:
				print "Wrong packet length"
				break

			bufSize = struct.unpack("I",bufSize)

			print "Packet size: "+str(bufSize[0])

			bufIDs = bytearray()
			buftmp = bytearray(2048)
			bytes_recd = 0

			while bytes_recd < bufSize[0]:
				chunkLen = s.recv_into(buftmp,2048)
				bufIDs[bytes_recd:] = buftmp[:chunkLen]
				bytes_recd = bytes_recd + chunkLen

			if 	bytes_recd != bufSize[0]:
				print "Wrong data"
				break

			print binascii.hexlify(bufIDs)

			collectedSamples = []

		elif key=='a':
			s.sendall('\xF2')

s.close()
