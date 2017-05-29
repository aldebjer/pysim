import colorama
colorama.init()

from jinja2 import Environment, PackageLoader
env = Environment(
    loader=PackageLoader('pysim', 'templates'),
)

def get_list(text = None):
    items = []
    while True:
        item = {}
        if text is None:
            print("\033[31;1m"+"State Name: "+"\33[0m",end="")
            item["state_name"] = input()
            if len(item["state_name"])==0:
                print("\033[1A                              ")
                break
            print("\033[31;1m"+"Der Name: "+"\33[0m",end="")
            item["der_name"] = input()
        else:
            print("\033[31;1m"+text+"\33[0m",end="")
            item = {}
            item["name"] = input()
            if len(item["name"])==0:
                print("\033[1A                              ")
                break
        print("\033[31;1m"+"Description: "+"\33[0m",end="")
        item["description"] = input()

        print("\033[31;1m"+"Type: "+"\33[0m",end="")
        item["type_int"] = int(input())
        print(item["type_int"])

        if item["type_int"] == 1:
            item["type_name"] = "double"
            item["init_text"] = "0.0"
        elif item["type_int"] == 2:
            item["type_name"] = "pysim::vector"
            print("\033[31;1m"+"rows: "+"\33[0m",end="")
            item["init_text"] = "{}, 0.0".format(input())
        elif item["type_int"] == 3:
            item["type_name"] = "Eigen::MatrixXd"
            print("\033[31;1m"+"rows: "+"\33[0m",end="")
            rows = input()
            print("\033[31;1m"+"columns: "+"\33[0m",end="")
            columns = input()
            item["init_text"] = "Eigen::MatrixXd::Zero({rows}, {cols})".format(rows = rows, cols = columns)
        else:
            raise IOError("Illegal type value")
        items.append(item)
    return items

print("\033[31;1m"+"System name: "+"\33[0m",end="")
system_name = input()
print("""
Types: 1. double
       2. vector
       3. matrix
""")
inputs = get_list("Input :")
outputs = get_list("Output :")
states = get_list()

template = env.get_template('system_template.hpp')
text = template.render(systemname=system_name, states = states, inputs=inputs, outputs=outputs)
with open("{}.hpp".format(system_name),'w') as f:
    f.write(text)

template = env.get_template('system_template.cpp')
text = template.render(systemname=system_name, states = states, inputs=inputs, outputs=outputs)
with open("{}.cpp".format(system_name),'w') as f:
    f.write(text)
