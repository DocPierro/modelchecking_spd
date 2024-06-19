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


class PowerMonitorThread (threading.Thread):
	def __init__(self, monitor):
		threading.Thread.__init__(self)
		self.monitor = monitor
		self.exitFlag = 1
		self.stDataColl = 0
		self.collectedSamples = []

	def setExitFlag(self,val):
		self.exitFlag = val

	def setStDataColl(self,val):
		self.stDataColl = val

	def resetCollectedSamples(self):
		self.collectedSamples = []

	def getCollectedSamples(self):
		return self.collectedSamples

	def run(self):
		print "Starting PowerMonitor Thread\n"
		self.monitor.StartDataCollection()

		while self.exitFlag:
			samples = self.monitor.CollectData()
			if self.stDataColl:
				self.collectedSamples.extend(samples)

				#print "\n".join(map(str, samples))

		self.monitor.StopDataCollection()

		print "Ending PowerMonitor Thread\n"

# HOST = 'localhost'			# The remote host
# PORT = 27778				# The same port as used by the server
def connectToSimulator(HOST,PORT):
		time.sleep(4);

		if len(sys.argv) > 1:
			PORT = int(sys.argv[1])

		s = None
		for res in socket.getaddrinfo(HOST, PORT, socket.AF_UNSPEC, socket.SOCK_STREAM):
			af, socktype, proto, canonname, sa = res
			try:
				s = socket.socket(af, socktype, proto)
			except socket.error as msg:
				s = None
				continue
			try:
				s.connect(sa)
			except socket.error as msg:
				s.close()
				s = None
				continue
			break
		if s is None:
			print 'could not open socket'
			sys.exit(1)

		return s

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


