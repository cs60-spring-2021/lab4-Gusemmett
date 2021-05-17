def print_table(table):
	print("------------------- DV TABLE -------------------")
	for host, row in table.items():

		lst = []
		for k, v in row.items():
			lst.append(f"-- From Node: {host} to Node: {k} -- cost: {v}")

		for x in lst:
			print(x)

	print("------------------------------------------------\n")

def table_to_str(table):
	strn = ""
	for key, value in table.items():
		row_strn = f"{key} "
		for host, cost in value.items():
			row_strn += f"{host}:{cost} "
		strn += f"{row_strn.strip()}\n"

	return strn.strip()

def str_to_table(strn):
	table = {}
	for row_str in strn.split("\n"):
		row_dict = {}
		kvs = row_str.split(" ")
		for hc in kvs[1:]:
			try:
				row_dict[hc.split(":")[0]] = int(hc.split(":")[1])
			except:
				pass
		table[kvs[0]] = row_dict

	return table

# babylon4.thayer.dartmouth.edu babylon2.thayer.dartmouth.edu 5
# babylon4.thayer.dartmouth.edu babylon3.thayer.dartmouth.edu 1
# babylon3.thayer.dartmouth.edu babylon2.thayer.dartmouth.edu 1
