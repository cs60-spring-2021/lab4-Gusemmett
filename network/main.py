import socket
from threading import Thread
import time
from utils import *

SERVER = 'localhost'
PORT = 60561
BUFFER_SIZE = 2048



class DVR():
	def __init__(self, server, port, topology_file):
		#Opening TCP connection and accepting connection from local overlay
		self.soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.soc.connect((server, port))
		self.hostname = socket.gethostname()

		self.table = {}

		self.init_dv(topology_file)

		self.timeStop = time.time() + 60


		# Creating recieve and sending threads
		recv_thread = Thread(target=self.recv, args=() )
		recv_thread.start()

		send_thread = Thread(target=self.send, args=() )
		send_thread.start()



	def recv(self):
		while self.timeStop > time.time():
			try:
				self.soc.settimeout(10)
				data = (self.soc.recv(BUFFER_SIZE)).decode('utf-8')
			except:
				continue
			
			node_table = str_to_table(data)

			self.update_table(node_table)
			self.recalculate_table()
			print_table({self.hostname:self.table[self.hostname]})




	def send(self):
		while self.timeStop > time.time():
			if time.time() % 5 == 0:
				self.soc.sendall(bytes(table_to_str(self.table), 'utf-8'))



	def update_table(self, node_table):
		has_changed = False
		# Updates the table to take the smaller values
		for host, row in self.table.items():
			for k in row.keys():
				if row[k] > node_table[host][k]:
					row[k] = node_table[host][k]
					has_changed = True
		return has_changed


	def recalculate_table(self):

		for target_host, cost in self.table[self.hostname].items():
			best_cost = cost
			if target_host != self.hostname:

				for neighbour_host, row in self.table.items():
					n2g = row[target_host] # neighbour to target
					m2n = self.table[self.hostname][neighbour_host] # me to neighbour
					if best_cost > n2g + m2n:
						best_cost = n2g + m2n
			self.table[self.hostname][target_host] = best_cost



	def init_dv(self, topology_file):
		fp = open(topology_file, "r")

		seen = set([])
		self.table[self.hostname] = {}
		
		for i, line in enumerate(fp):
			words = line.split(" ")

			node0 = words[0].split(".")[0]
			node1 = words[1].split(".")[0]
			cost = int(words[2].strip())

			if node0 == self.hostname:
				self.table[node0][node1] = cost
			elif node1 == self.hostname:
				self.table[node1][node0] = cost
			

			for n in [node0, node1]:
				if n not in self.table:
					self.table[n] = {}

		hosts = list(self.table.keys())
		# Filling in inf costs
		for _, row in self.table.items():
			for k in hosts:
				if k not in row:
					row[k] = 9999
		self.table[self.hostname][self.hostname] = 0
			
		fp.close()







if __name__ == '__main__':
	dvr = DVR(SERVER, PORT, "../topology.dat")

