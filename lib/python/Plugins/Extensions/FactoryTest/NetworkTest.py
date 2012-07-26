import os
import re

class NetworkTest:
	def __init__(self):
		self.testserver = "8.8.8.8"
		
	def monitoring(self):  
	    cmd = "ping -c 5 %s" % self.testserver
	    p = os.popen(cmd)  
	    info = p.read()  
	    print "info read",info
	    
	    p.close()  
	    re_lost_str = '(\d+)% packet loss'  
	    lost_packet = int(re.search(re_lost_str, info).group(1))  
	    print "lost package is :",lost_packet
	    if lost_packet == 100:
	    	print "device Error"
	        return False
		return True