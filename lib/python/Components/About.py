from Tools.Directories import resolveFilename, SCOPE_SYSETC
from enigma import getEnigmaVersionString
from os import popen, stat
import sys
import os
import time


class About:
	def __init__(self):
		pass

	def getVersionString(self):
		return self.getImageVersionString()

	def getImageVersionString(self):
		try:
			st = stat('/usr/lib/ipkg/status')
			tm = time.localtime(st.st_mtime)
			if tm.tm_year >= 2011:
				return time.strftime("%b %e %Y %H:%M:%S", tm)
		except:
			pass

		return "unavailable"

	def getEnigmaVersionString(self):
		enigma_version = getEnigmaVersionString()
		if '-(no branch)' in enigma_version:
			enigma_version = enigma_version [:-12]
		return enigma_version

	def getKernelVersionString(self):
		try:
			result = popen("uname -r","r").read().strip("\n").split('-')
			kernel_version = result[0]
			return kernel_version
		except:
			pass

		return "unknown"
		
	def getHardwareTypeString(self):
		try:
			if os.path.isfile("/proc/stb/info/boxtype"):
				return open("/proc/stb/info/boxtype").read().strip().upper() + " (" + open("/proc/stb/info/board_revision").read().strip() + "-" + open("/proc/stb/info/version").read().strip() + ")"
			if os.path.isfile("/proc/stb/info/vumodel"):
				return "VU+" + open("/proc/stb/info/vumodel").read().strip().upper() + "(" + open("/proc/stb/info/version").read().strip().upper() + ")" 
			if os.path.isfile("/proc/stb/info/model"):
				return open("/proc/stb/info/model").read().strip().upper()
		except:
			pass
		return _("unavailable")

	def getImageTypeString(self):
		try:
			return open("/etc/issue").readlines()[-2].capitalize().strip()[:-6]
		except:
			pass
		return _("undefined")


about = About()
