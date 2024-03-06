import example

# Create an instance of MyMap
my_map_obj = example.MyMap()

# Add values to the map
my_map_obj = {example.ENUM_VALUE_1:10, example.ENUM_VALUE_2:20}

# Access values from the map
print(dict(my_map_obj))
print(my_map_obj[example.ENUM_VALUE_1])  # Output: 10
print(my_map_obj[example.ENUM_VALUE_2])  # Output: 20