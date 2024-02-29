import person

# 创建一个Person对象
p = person.Person("Alice", 25)
print(p.name)   # 输出 "Alice"
print(p.age)    # 输出 25

# 创建一个Person对象的shared_ptr
p_ptr = person.createPerson("Bob", 30)
print(p_ptr.name)   # 输出 "Bob"
print(p_ptr.age)    # 输出 30


r = person.dumpString(p)
print(r)    # 输出 Name: Alice, Age: 25
r = person.dumpString(p_ptr)
print(r)    # 输出 Name: Bob, Age: 30
