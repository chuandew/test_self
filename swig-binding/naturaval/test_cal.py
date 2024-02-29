import cal

foo=cal.myStruct()
foo.arr1=1,2,3
print(foo.arr1)
# foo.arr2=1,2,3 #oops ERROR
foo.arr2=cal.FloatVector([1,2,3])
print(foo.arr2)
print(list(foo.arr2))