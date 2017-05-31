#include "{{systemname}}.hpp"

#include "factory.hpp"

std::string {{systemname}}::getDocs(){
    return std::string("");
}

{{systemname}}::{{systemname}}(void):{% set comma = joiner(",") %}{% for state in states %}{{comma()}}
    {{state.state_name}}({{state.init_text}}),
    {{state.der_name}}({{state.init_text}}){% endfor %}{% for input in inputs %}{{comma()}}
    {{input.name}}({{input.init_text}}){% endfor %}{% for output in outputs %}{{comma()}}
    {{output.name}}({{output.init_text}}){% endfor %}
{
    {% for state in states %}STATE({{state.state_name}}, {{state.der_name}}, "{{state.description}}")
    {% endfor %}
    {% for input in inputs %}INPUT({{input.name}}, "{{input.description}}")
    {% endfor %}
    {% for output in outputs %}OUTPUT({{output.name}}, "{{output.description}}")
    {% endfor %}

    //Initialization code below here
}

//This function is called for each evaluation
void {{systemname}}::doStep(double time){
}

REGISTER_SYSTEM({{systemname}});