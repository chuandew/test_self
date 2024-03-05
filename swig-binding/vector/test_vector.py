import vector

v = vector.CustomeVector()
print(v)

util = vector.VectorUtil()
print(util)

util.add(v,3)
print(list(v))

v[0].display()
util.display(v)

n=vector.CustomeVector([vector.Vector(3,3),vector.Vector(2,2),vector.Vector(1,1)])
util.display(n)

util.display([vector.Vector(3,3),vector.Vector(2,2),vector.Vector(1,1)]) # display must const vector& or vector
# util.display([(3,3),(2,2),(1,1)])
