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


def SaveBatteryParams(handle, pardict):

	for key in pardict.keys():
		handle.write("double "+key+" = "+str(pardict[key])+";\n")

	return

def SaveBatteryFileHeader(handle, gK, gc, gC, gIR, gAvgReward):

	handle.write("\n");

	handle.write("double gK"+" = "+str(gK)+";\n");
	handle.write("double gc"+" = "+str(gc)+";\n");
	handle.write("double gC"+" = "+str(gC)+";\n");
	handle.write("double gIR"+" = "+str(gIR)+";\n");
	handle.write("double gAvgReward"+" = "+str(gAvgReward)+";\n");

	handle.write("\n");

	handle.write("double gCurrentReward = 0.0;\n");
	handle.write("double gTime = 0.0;\n");
	handle.write("double gY1 = (1-gc)*gC;\n");
	handle.write("double gY2 = gc*gC;\n");

	handle.write("\n");

	handle.write("struct TimeReward\n");
	handle.write("{\n");
	handle.write("\tdouble time;\n");
	handle.write("\tdouble reward;\n");
	handle.write("};\n");

	handle.write("\n");

	handle.write("struct RetValTR\n");
	handle.write("{\n");
	handle.write("\tTimeReward * pt;\n");
	handle.write("\tunsigned int size;\n");
	handle.write("};\n");
	handle.write("\n");
	return 

def SaveBatteryDistribution(handle, dData):

	dIdCurr = {}
	for key, valone, valtwo in dData:
		dIdCurr.setdefault(key, []).append([valone, valtwo])

	for key in dIdCurr:
		handle.write("TimeReward tr"+str(key)+"[] = { ");
		llen = len(dIdCurr[key])
		cnt = 0;
		ll = dIdCurr[key]
		for elem in ll:
			handle.write("{"+str(elem[1])+", "+str(elem[0])+"}");
			cnt = cnt + 1
			if cnt!=llen:
				handle.write(", ");
		handle.write("};\n");
	

	handle.write("\n")	
	# handle.write("RetValTR RetStruct(int id)\n")
	# handle.write("{\n")
	# handle.write("\tRetValTR ret = {NULL,0};\n")

	# handle.write("\n")
	# handle.write("\tswitch(id) {\n")

	handle.write("double magicUDDD(vector<double> const& param,unsigned int rand){\n")
	handle.write("\tRetValTR ret = {NULL,0};\n")
	handle.write("\tdouble timeVal = 0.0;\n")
	handle.write("\n")
	handle.write("\tswitch((int)param[0]) {\n")

	for key in dIdCurr:
		handle.write("\t\tcase "+str(key)+":\n")
		handle.write("\t\t\tret.pt = & tr"+str(key)+"[0];\n")
		handle.write("\t\t\tret.size = sizeof(tr"+str(key)+")/(sizeof(double)*2);\n")
		handle.write("\t\t\tbreak;\n")

	handle.write("\t}\n")
	handle.write("\ttimeVal = ret.pt[idx].time;\n")
	handle.write("\tgCurrentReward = ret.pt[idx].reward/timeVal;\n")
	handle.write("\n")
	handle.write("\treturn timeVal;\n")
	handle.write("}\n")
	# handle.write("\n")
	# handle.write("\treturn ret;\n")
	# handle.write("}\n")


	SaveBatteryFooter(handle)	
	return


def SaveBatteryFooter(handle):
	handle.write("\n");

	handle.write("double y1fun(double c, double kp, double k, double t, double y0, double y10, double y20, double I)\n")
	handle.write("{\n")
	#handle.write("\tcerr<<\"Battery state: \"<<y10<<\" \"<<y20<<\" \"<<I<<\" \"<<t<<endl;")
	handle.write("\n")
	handle.write("\treturn y10*exp(-kp*t)+(y0*kp*c-I)*(1-exp(-kp*t))/kp-I*c*(kp*t-1+exp(-kp*t))/kp;\n")
	handle.write("}\n")

	handle.write("\n");

	handle.write("double y2fun(double c, double kp, double k, double t, double y0, double y10, double y20, double I)\n")
	handle.write("{\n")
	#handle.write("\tcerr<<\"Battery state: \"<<y10<<\" \"<<y20<<\" \"<<I<<\" \"<<t<<endl;")
	handle.write("\n")
	handle.write("\treturn y20*exp(-kp*t)+y0*(1-c)*(1-exp(-kp*t))-I*(1-c)*(kp*t-1+exp(-kp*t))/kp;\n")
	handle.write("}\n");

	handle.write("\n");

	handle.write("void enterActive(int id, double time)\n")
	handle.write("{\n")

	handle.write("\tif (time==gTime || gCurrentReward==0.0) return;\n")
	handle.write("\n");
	handle.write("\tgY1 = y1fun(gc, gK/(gc*(1-gc)), gK, (time-gTime)/1000.0, gY1+gY2, gY1, gY2, gIR);\n")
	handle.write("\tgY2 = y2fun(gc, gK/(gc*(1-gc)), gK, (time-gTime)/1000.0, gY1+gY2, gY1, gY2, gIR);\n")
	handle.write("\tgTime = time;\n")
	handle.write("}\n")

	handle.write("\n");

	handle.write("void enterIdle(int id, double time)\n")
	handle.write("{\n")
	handle.write("\tif (time==gTime || gCurrentReward==0.0) return;\n")
	handle.write("\n");	
	handle.write("\tgY1 = y1fun(gc, gK/(gc*(1-gc)), gK, (time-gTime)/1000.0, gY1+gY2, gY1, gY2, gCurrentReward);\n")
	handle.write("\tgY2 = y2fun(gc, gK/(gc*(1-gc)), gK, (time-gTime)/1000.0, gY1+gY2, gY1, gY2, gCurrentReward);\n")
	handle.write("\tgTime = time;\n")
	handle.write("}\n")

	handle.write("\n")

	handle.write("bool isBatterieDepleted(void)\n")
	handle.write("{\n")
	handle.write("\treturn (gY1<0.0);\n")
	handle.write("}\n")

	handle.write("\n")

	handle.write("bool ResetBatteryState(void)\n")
	handle.write("{\n")
	handle.write("\tgCurrentReward = 0.0;\n");
	handle.write("\tgTime = 0.0;\n");
	handle.write("\tgY1 = (1-gc)*gC;\n");
	handle.write("\tgY2 = gc*gC;\n");
	handle.write("\tsrand (time(NULL));\n")
	handle.write("}\n")

	handle.write("\n")

	return