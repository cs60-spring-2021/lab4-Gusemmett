# CS60 Spring 2021, Lab 4
###### Angus Emmett
###### GitHub username: gusemmett

## Datastructure
The way that I store the DVT is with a dictionary of dictionaries. Where the outer dict stores the from node and the inner dict stores the to nodes and their costs.
```
{
	from_node_1 : {to_node_1:cost_1, to_node_2:cost_2, to_node_3:cost_3},
	from_node_2 : {to_node_1:cost_1, to_node_2:cost_2, to_node_3:cost_3},
	from_node_3 : {to_node_1:cost_1, to_node_2:cost_2, to_node_3:cost_3}
}
```

## Protocol
When sending DVTs to other nodes the DVTs are converted from the above structure to a string in the following format. 
```
node_from_1 node_to_1:cost_1 node_to_2:cost_2 ... node_to_n:cost_n
node_from_2 node_to_1:cost_1 node_to_2:cost_2 ... node_to_n:cost_n
```

