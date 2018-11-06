import sys,os
m = int(sys.argv[1])
Specfile = open("spec.blif","w+")
Specfile.write("#.top matrix%s" %(m)+"\n")
Specfile.write(".model matrix%s" %(m)+"\n")
for i in xrange(1,m+1):
	Specfile.write(".inputs Is%s" % (i)+"\n")
	Specfile.write(".inputs")
	for j in xrange(1,m+1):
		Specfile.write(" w%s_%s" % (i,j))
	Specfile.write("\n");
Specfile.write(".outputs")
for i in xrange(1,m+1):
	Specfile.write(" Istim%s" %(i))
Specfile.write("\n")
n=m
s = ""
l=[]
po = 1
while n>0:
	if(n%2==1):
		s=s+"1"
		l.append(po)
	else:
		s=s+"0"
	n=n/2
	po=po*2
# print(s)
# print(l)
for x in xrange(1,m+1):
	Specfile.write("\n")
	for y in xrange(1,m+1):
		Specfile.write(".subckt and in0=Is%s in1=w%s_%s out=Is%sw%s_%s\n" %(y,x,y,y,x,y))
	Specfile.write(".subckt xor%s" %(m))
	for z in xrange(0,m):
		Specfile.write(" in%s=Is%sw%s_%s" % (z,z+1,x,z+1))
	Specfile.write(" out=Istim%s\n" %(x))
Specfile.write(".end\n\n")

Specfile.write(".model and\n.inputs in0 in1\n.outputs out\n.names in0 in1 out\n11 1\n.end\n\n")
if(m==2**(len(s)-1)):
	pass
else:
	Specfile.write(".model xor%s\n" %(m))
	Specfile.write(".inputs")
	for x in xrange(0,m):
		Specfile.write(" in%s" %(x))
	Specfile.write("\n.outputs out\n")
	if l[0]==1:
		sub = len(l)-1
		count=0
		o=0
		for y in xrange(0,sub):
			Specfile.write(".subckt")
			Specfile.write(" xor%s" %(l[y+1]))
			for z in xrange(0,l[y+1]):
				Specfile.write(" in%s=in%s" % (z,count))
				count+=1
			Specfile.write(" out=o%s\n" % (y+1))
			o=y+1
		Specfile.write(".subckt xor2 in0=in%s in1=o1 out=o%s\n" % (count,o+1))
		oc = 2
		o+=1
		for y in xrange(0,sub-2):
			Specfile.write(".subckt xor2 in0=o%s in1=o%s out=o%s\n" % (oc,o,o+1))
			oc+=1
			o+=1
		Specfile.write(".subckt xor2 in0=o%s in1=o%s out=out\n" % (oc,o))
		Specfile.write(".end\n\n")
	else:
		sub = len(l)
		count=0
		o=0
		for y in xrange(0,sub):
			Specfile.write(".subckt")
			Specfile.write(" xor%s" %(l[y]))
			for z in xrange(0,l[y]):
				Specfile.write(" in%s=in%s" % (z,count))
				count+=1
			Specfile.write(" out=o%s\n" % (y+1))
			o=y+1
		oc=1
		for y in xrange(0,sub-2):
			Specfile.write(".subckt xor2 in0=o%s in1=o%s out=o%s\n" % (oc,o,o+1))
			oc+=1
			o+=1
		Specfile.write(".subckt xor2 in0=o%s in1=o%s out=out\n" % (oc,o))
		Specfile.write(".end\n\n")
		
for x in xrange(1,len(s)-1):
	i = 2**(len(s)-x)
	Specfile.write(".model xor%s\n" %(i))
	Specfile.write(".inputs")
	for y in xrange(0,i):
		Specfile.write(" in%s" %(y))
	Specfile.write("\n.outputs out\n")
	for z in xrange(0,2):
		Specfile.write(".subckt xor%s" % (i/2))
		for in1 in xrange(0,i/2):
			Specfile.write(" in%s=in%s" % (in1,in1+z*(i/2)))
		Specfile.write(" out=o%s\n" % (z+1))
	Specfile.write(".subckt xor2 in0=o1 in1=o2 out=out\n.end\n\n")
Specfile.write(".model xor2\n.inputs in0 in1\n.outputs out\n.names in0 in1 out\n10 1\n01 1\n.end\n")
Specfile.close()
#Datfile = open("data.txt","w+")
#m=int(sys.argv[2])
#for x in xrange(0,m):
#	if x>0:
#		Datfile.write("\n")
#	for y in xrange(0,m):
#		if(y>0):
#			Datfile.write(" ") 
#		if(y==(x+1)%m):
#			Datfile.write("1")
#		else:
#			Datfile.write("0")
#Datfile.close()
#os.system("../bin/gen spec.blif data.txt out%s.blif" %(int(sys.argv[3])))
