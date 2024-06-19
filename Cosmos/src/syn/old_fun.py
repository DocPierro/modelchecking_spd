def GetRisingEdge(pmData):
	cnt = 0
	prev = pmData[0][1]
	for it in range(1,len(pmData)):

		if prev==0 and 	pmData[it][1]==1:
			cnt = cnt + 1
		prev = pmData[it][1]	
	return cnt

def ProcessPowerMonitorData(pmData, monitorSampligFreq):
	currentTran = []

	#firstPosM2 = 0
	#firstPosM1 = 0
	filedta = open('markers.txt', 'w+')
	sumCurrent = 0
	sampleCnt = 1
	prev = pmData[0][1]
	state = 0
	prev_time = 0.0

	SaveToFile(filedta, prev)

	for it in range(1,len(pmData)):
		sampleCnt = sampleCnt + 1

		SaveToFile(filedta, pmData[it][1])

		#print str(state)+":"+str(sampleCnt/monitorSampligFreq)

		if state==0:
			if pmData[it][1]==2 and prev==2:
				state = 0
			elif pmData[it][1]==0 and prev==2:
				state = 1
				sumCurrent = sumCurrent + pmData[it][0]
			elif pmData[it][1]==1 and prev==2:	
				state = 2
				sumCurrent = sumCurrent + pmData[it][0]

		if state==1:
			if pmData[it][1]==0 and prev==0:
				state = 1
				sumCurrent = sumCurrent + pmData[it][0]
			elif pmData[it][1]==1 and prev==0:
				state = 2
				sumCurrent = sumCurrent + pmData[it][0]
			elif pmData[it][1]==2 and prev==0:
				state = 1
			elif pmData[it][1]==3 and prev==0:
				state = 6
				sumCurrent = sumCurrent + pmData[it][0]

		if state==2:
			if pmData[it][1]==0 and prev==1:
				state = 3
				sumCurrent = sumCurrent + pmData[it][0]
			elif pmData[it][1]==1 and prev==1:
				state = 2
				sumCurrent = sumCurrent + pmData[it][0]
			elif pmData[it][1]==2 and prev==1:
				state = 5
			elif pmData[it][1]==3 and prev==1:
				state = 6
				sumCurrent = sumCurrent + pmData[it][0]

		if state==3:
			if pmData[it][1]==0 and prev==0:
				state = 3
				sumCurrent = sumCurrent + pmData[it][0]
			elif pmData[it][1]==1 and prev==0:
				state = 3
				stime = sampleCnt/monitorSampligFreq

				if stime-prev_time>5.0:
					currentTran.append((sumCurrent,stime))

				prev_time = stime	
				#print str(stime)+"  "+str(sumCurrent)
				sumCurrent = pmData[it][0]
			elif pmData[it][1]==2 and prev==0:
				state = 5
			elif pmData[it][1]==3 and prev==0:
				state = 4
				sumCurrent = sumCurrent + pmData[it][0]

		if state==4:
			if (pmData[it][1]==0 and prev==3) or (pmData[it][1]==0 and prev==2):
				state = 1
				stime = sampleCnt/monitorSampligFreq

				if stime-prev_time>5.0:
					currentTran.append((sumCurrent,stime))

				prev_time = stime	
				#print str(stime)+"  "+str(sumCurrent)
				sumCurrent = pmData[it][0]

			#elif pmData[it][1]==2 and prev==3:
			#	state = 5
			elif (pmData[it][1]==2 and prev==3) or (pmData[it][1]==2 and prev==2):
				state = 4
				sumCurrent = sumCurrent + pmData[it][0]
			elif pmData[it][1]==3 and prev==3:
				state = 4
				sumCurrent = sumCurrent + pmData[it][0]

		if state==5:
			state = 5

		if state==6:
			if (pmData[it][1]==0 and prev==3) or (pmData[it][1]==0 and prev==2):
				state = 1
				sumCurrent = sumCurrent + pmData[it][0]
			elif (pmData[it][1]==1 and prev==3):
				state = 2
				sumCurrent = sumCurrent + pmData[it][0]
			elif pmData[it][1]==2 and prev==3:
				state = 6
			elif (pmData[it][1]==3 and prev==3) or (pmData[it][1]==2 and prev==2):
				state = 6
				sumCurrent = sumCurrent + pmData[it][0]

		prev = pmData[it][1]

	filedta.close()

	return currentTran
