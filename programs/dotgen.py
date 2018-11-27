import sys,os
from copy import deepcopy
from pprint import pprint
from operator import itemgetter
ADDR = 2
def convToString(elem):
	st=""
	if(isinstance(elem,list)):
		st="f("+convToString(elem[0][0])+","+convToString(elem[1][0])+","+convToString(elem[2][0])+")"
	else:
		st=elem
	return st
def get_time(elem):
	if(elem[:5]=="Istim"):
		return 1000000.0
	elif(elem[2:5]=="out"):
		return 999999.0
	elif(elem[2:5]=="reg"):
		return int(elem[7:elem.find('c')])+.1*0+.01*int(elem[elem.find('c')+1:elem.find('r',elem.find('r')+1)])+.00001*int(elem[elem.find('r',elem.find('r')+1)+1:])
	elif(elem[2:5]=="con"):
		return int(elem[7:elem.find('f')])+.1*2
	elif(elem[2:5]=="lut"):
		return int(elem[11:elem.find('c')])+.1*1
def parse_wire(line_con,table,lt):
	global ADDR
	if ((len(line_con) == 2)):
		if((line_con[1][2:5]=="reg") or (line_con[1][2:5]=="out") or (line_con[0][2:5]=="reg") or (line_con[0][2:5]=="out") or (line_con[1][2:5]=="con") or (line_con[0][2:5]=="con")):
			if(table.has_key(line_con[1])):
				if(not line_con[0][0]=="_"):
					pass
				else:
					if(table.has_key(line_con[0])):
						lt[table[line_con[1]]][0]=lt[table[line_con[0]]][0]
					else:
						table[line_con[0]]=table[line_con[1]]
			else:
				if(not line_con[0][0]=="_"):
					lt.append([line_con[0]])
					table[line_con[1]]=ADDR
					ADDR+=1
				else:
					if(table.has_key(line_con[0])):
						table[line_con[1]]=table[line_con[0]]
					else:
						lt.append([""])
						table[line_con[0]]=ADDR
						table[line_con[1]]=ADDR
						ADDR+=1
	else:
		if(line_con[0][2:5]=="reg"):
			if(table.has_key(line_con[0])):
				table[line_con[0]]=0
			else:
				table[line_con[0]]=0
def parse_ckt(line_con,table,lt):
	global ADDR
	lut_name = line_con[-1][4:]
	if(table.has_key(lut_name)):
		lt[table[lut_name]][0]=[]
		for i in xrange(0,3):
			k0 = line_con[i][4:]
			if(table.has_key(k0)):
				lt[table[lut_name]][0].append(lt[table[k0]])
			else:
				print(k0)
	else:
		lt.append([[]])
		table[lut_name]=ADDR
		ADDR+=1
		for i in xrange(0,3):
			k0 = line_con[i][4:]
			if(table.has_key(k0)):
				lt[table[lut_name]][0].append(lt[table[k0]])
			else:
				print(k0)

lt = [['0'],['1']]
Outfile = open(sys.argv[1])
connections = []
content = [x.strip('\n') for x in Outfile]
model = 0
count = 7
table = {}
table["__zero"] = 0
table["__one"] = 1
# rem = []
for line in content[7:]:
	line_con = line.split(' ')
	count+=1
	# print(table)
	if (line_con[0] == ".end"):
		break
	elif(line_con[0] == ".names"):
		parse_wire(line_con[1:],table,lt)
	elif(line_con[0] == ".subckt"):
		parse_ckt(line_con[2:],table,lt)
# pprint(table)
# pprint(lt)
d={}
chan = {}
rem = []
for k, val in table.iteritems():
	d[k]=lt[val]
	chan[k] = deepcopy(d[k])
	chan[k] = convToString(chan[k][0])
	rem.append(k)
# pprint(d)
rem.sort(key=lambda x:get_time(x))
# pprint(chan)
reg_String = "__reg_t%dc%dr%d"
# lut_inp = "lut_in_t%dc%d_%s"
lut_String = "lut_out_t%dc%dn0"
Graphfile = open("Graph_"+sys.argv[1]+".gv","w+")
Graphfile.write("digraph G {\n")
for chip in xrange(0,int(sys.argv[3])):
	Graphfile.write("\tsubgraph cluster%d {\n" % chip)
	for time in xrange(0,int(sys.argv[2])):
		tt="%s|{" % (time)
		for reg in xrange(0,int(sys.argv[4])):
		# ranking = "\t\t{rank = same; "+lut_inp % (time,chip,1)+";"+lut_inp % (time,chip,2)+";"+lut_inp % (time,chip,3) + ";}\n"
			if(reg%4==0):
				tt+="{"
			tt += "<f%s> " % (reg)+chan[reg_String % (time,chip,reg)]+"|"
			if(reg%4==3):
				tt=tt[:-1]
				tt+="}|"				
		if(tt[-1]=="|"):
			tt=tt[:-1]
			if(not tt[-1]=="}"):
				tt+="}"
		tt+="}"
		# Graphfile.write("\t\t"+lut_inp % (time,chip,1)+" [shape=box,label="+"\""+chan["__"+lut_String % (time,chip)]+"\""+"];\n")
		# Graphfile.write("\t\t"+lut_inp % (time,chip,2)+" [shape=box,label="+"\""+chan["__"+lut_String % (time,chip)]+"\""+"];\n")
		# Graphfile.write("\t\t"+lut_inp % (time,chip,3)+" [shape=box,label="+"\""+chan["__"+lut_String % (time,chip)]+"\""+"];\n")
		Graphfile.write("\t\t"+"t%sc%s" % (time,chip)+" [shape=record,label="+"\""+tt+"\""+"];\n")
		Graphfile.write("\t\t"+lut_String % (time,chip)+" [shape=box,label="+"\""+chan["__"+lut_String % (time,chip)]+"\""+"];\n")
		# Graphfile.write(ranking)
		# Graphfile.write("\t\t"+lut_inp % (time,chip,1)+"->"+lut_String % (time,chip)+" [weight=100];\n")
		# Graphfile.write("\t\t"+lut_inp % (time,chip,2)+"->"+lut_String % (time,chip)+" [weight=100];\n")
		# Graphfile.write("\t\t"+lut_inp % (time,chip,3)+"->"+lut_String % (time,chip)+" [weight=100];\n")
		Graphfile.write("\t\t"+"t%sc%s" % (time,chip)+"->"+lut_String % (time,chip)+" [style = invis,weight=200];\n")
		if(time>0):
			Graphfile.write("\t\t"+lut_String % (time-1,chip)+"->"+lut_String % (time,chip)+" [style = invis,weight=200];\n")
			Graphfile.write("\t\t"+lut_String % (time-1,chip)+"->"+"t%sc%s" % (time,chip)+" [style = invis,weight=200];\n")
			# Graphfile.write("\t\t"+lut_String % (time-1,chip)+"->"+lut_inp % (time,chip,2)+" [style = invis,weight=200];\n")
			# Graphfile.write("\t\t"+lut_String % (time-1,chip)+"->"+lut_inp % (time,chip,3)+" [style = invis,weight=200];\n")
	Graphfile.write("\t}\n")
for time in xrange(0,int(sys.argv[2])-1):
	for chip in xrange(0,int(sys.argv[3])):
		nchip = (chip+1)%int(sys.argv[3])
		Graphfile.write("t%sc%s" % (time,chip)+"->"+"t%sc%s" % (time+1,nchip)+" [label=\""+chan["__con_t%sfm%sto%sk0" % (time,chip,nchip)]+"\",constraint=false,color=red];\n")
Graphfile.write("}\n")
Graphfile.close()
os.system("dot -Tpdf Graph_%s.gv -o %s.pdf" % (sys.argv[1],sys.argv[5]))